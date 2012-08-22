#include "ide.h"
#include "low-io.h"
#include "idt.h"
#include "kheap.h"
#include "timer.h"
#include "types.h"
#include "mutex.h"
#include "pci.h"
#include "string.h"
#include "stdio.h"
#include "atarw.h"
#include "helper.h"
#include "drive.h"
//#include "fat.h"

static int ide_drv[4]={0,0,0,0}; // just for our convenience we limit to only 1 IDE card hence 4 drives
				// this array will hold 1 for a detected drive 0 for not detected.
				// after the initialization we will populate the actual hw data

slot *slots; // our physical drives(HDD)

pci_ata *pci_ata_populate_card(pci_cfg_t *pci_dev)
{
	pci_ata *ata_dev = (pci_ata *)kmalloc(sizeof(pci_ata));
	if(!ata_dev)
	{
		kprintf("Error: No memory for allocating ATA card info\n");
		return NULL;	
	}
	ata_dev->chan0.base_reg = pci_dev_get_bar(pci_dev,0);
	ata_dev->chan0.ctrl_reg = pci_dev_get_bar(pci_dev,1);
	ata_dev->chan0.bmide = pci_dev_get_bar(pci_dev,4);
	ata_dev->chan1.base_reg = pci_dev_get_bar(pci_dev,2);
	ata_dev->chan1.ctrl_reg = pci_dev_get_bar(pci_dev,3);
	if(ata_dev->chan0.base_reg == 0xffff || ata_dev->chan0.ctrl_reg == 0xffff || ata_dev->chan0.bmide == 0xffff)
	{
		kprintf("Error in PCI config space\n");
		return NULL;	
	}
	ata_dev->chan0.base_reg &= 0xfffe;
	ata_dev->chan0.ctrl_reg &= 0xfffe;
	ata_dev->chan0.ctrl_reg -= 4;
	ata_dev->chan0.bmide &= 0xfffe;
	ata_dev->chan1.base_reg &= 0xfffe;
	ata_dev->chan1.ctrl_reg &= 0xfffe;
	ata_dev->chan1.ctrl_reg -= 4;
	ata_dev->chan1.bmide = ata_dev->chan0.bmide + 8;
	ata_dev->chan0.nIEN = 0;
	ata_dev->chan1.nIEN = 0;
	if(ata_dev->chan0.base_reg == 0)
	{
		kprintf("PCI ATA doesnt provide specific registers !!! Defaulting ISA ports...\n");
		ata_dev->chan0.base_reg = ATA_BASE_PRI;
		ata_dev->chan0.ctrl_reg = ata_dev->chan0.base_reg + DEV_CTRL_REG;
		ata_dev->chan1.base_reg = ATA_BASE_SEC;
		ata_dev->chan1.ctrl_reg = ata_dev->chan1.base_reg + DEV_CTRL_REG;
	}
	return ata_dev;
}
// is_pci_ide returns true if there is a pci_ide controller
// this checks the pci_list which we have created during pci scan
pci_cfg_t *is_pci_ide()
{
	pci_cfg_t *ata_dev = pci_get_dev(0x01,0x01);
	if(!ata_dev)
	{
		kprintf("No PCI ATA controller found\n");
		return NULL;	
	}
	return ata_dev;
}

	
// detect the master 
bool detect_master(unsigned short port)
{
	int tmp;
	outportb(port + DRV_HD_REG, 0xA0);	// Set drive
	pio_wait_busy(port);	
	tmp = inportb(port+STATUS_REG);	// Read status
	if (tmp & STA_DRDY)
		return true;
	else
		return false;
}
//detect slave
bool detect_slave(unsigned short port)
{
	int tmp;
	outportb(port + DRV_HD_REG, 0xB0);	// Set drive
	pio_wait_busy(port);	
	tmp = inportb(port+STATUS_REG);	// Read status
	if (tmp & STA_DRDY)
		return true;
	else
		return false;
}

// reset the drives on this controller
// call this if some drive behave insane
// or in the begining of driver
bool reset_controller(unsigned short port)
{
	pio_outbyte(port + DEV_CTRL_REG,ATA_CTL_SRST|ATA_CTL_nIEN);
	sleep(2);
	pio_outbyte(port + DEV_CTRL_REG,ATA_CTL_nIEN);
	sleep(2);
	unsigned int timeout=300000;
	while(pio_wait_busy(port))
	{
		timeout--;
		sleep(1);
	};
	if(!timeout)
		kprintf("Device is in busy state for a long time even after reset\n");
	unsigned short err=pio_inbyte(port+ERR_REG);
	if(err)
	{
		kprintf("Warning Error flag is %0x %0x \n",err,(unsigned short)pio_inbyte(port+ALT_ST_REG));
	}
	if(timeout)
		return true;
	return false;
}
void populatte_slots();
bool detect_ide()
{
	pci_cfg_t *ata_dev = NULL;
	pci_ata *ata = NULL;
	if(!(ata_dev = is_pci_ide()))
	{
		return FALSE;
	}
	kprintf("Found PCI ATA controller:");
	//pci_dev_show_details(ata_dev, FALSE);
	ata = pci_ata_populate_card(ata_dev);
	if(!ata)
	{
		kprintf("This card is beyond my capacity\n");
		return FALSE;
	}
	kprintf("Primary   : cmd = %-08x ctrl = %-08x bmide = %-08x\n",ata->chan0.base_reg,ata->chan0.ctrl_reg,ata->chan0.bmide);
	kprintf("Secondary : cmd = %-08x ctrl = %-08x bmide = %-08x\n",ata->chan1.base_reg,ata->chan1.ctrl_reg,ata->chan1.bmide);
	kprintf("Reseting controllers\n");
	if(!reset_controller(ata->chan0.base_reg))
		kprintf("can't reset channel primary\n");
	if(!reset_controller(ata->chan1.base_reg))
		kprintf("can't reset channel secondary\n");
	//now detect the physical hdds
	if(detect_master(ata->chan0.base_reg))
		ide_drv[0]=1;
	if(detect_slave(ata->chan0.base_reg))
		ide_drv[1]=1;
	if(detect_master(ata->chan1.base_reg))
		ide_drv[2]=1;
	if(detect_slave(ata->chan1.base_reg))
		ide_drv[3]=1;
	int k;
	slot *temp=NULL,*cur_slot=NULL;
	for(k=0;k<4;k++)
	{
		//kprintf("%d ",ide_drv[k]);
		if(ide_drv[k] == 1)
		{
			kprintf("Found drive ");
			kprintf((k<2)?"primary ":"secondary ");
			kprintf((k%2)?"slave\n":"master\n");
			temp = (slot *)kmalloc(sizeof(slot));
			if(!temp)
			{
				kprintf("Error No Memory for allocating slot info\n");
				return FALSE;
			}
			temp->chanl = (chan *)kmalloc(sizeof(chan));
			if(!temp->chanl)
			{
				kprintf("Error No Memory for allocating channel info\n");
				return FALSE;
			}
			temp->exists = 1;
			temp->drv_number = k;
			if(k<2)
			{
				memcpy(temp->chanl, &ata->chan0,sizeof(chan));
				temp->ps = 0; //primary channel
			}
			else
			{
				memcpy(temp->chanl, &ata->chan1,sizeof(chan));
				temp->ps = 1;
			}
			if(k%2)
			{
				temp->ms = 1;
			}
			else
			{
				temp->ms = 0;
			}
			if(slots == NULL)
			{
				slots = temp;
				cur_slot=temp;
			}
			else
			{
				cur_slot->next = temp;
				cur_slot=cur_slot->next;
			}
			//populate_slots();
			temp = NULL;
		}
	}
	return TRUE;
}

// select a device for IO
void select_device(slot *s)
{
	unsigned short port;
	unsigned char val=0;
	port = s->chanl->base_reg + DRV_HD_REG;
	if(s->ms)
		val = 0xB0 ;
	else
		val = 0xA0 ;
	pio_outbyte(port,val);
}


void populate_slots()
{
	unsigned char id_cmd; // for Identify ATA command
	slot *temp;
	temp = slots;

	ata_ident *dat = (ata_ident *)kmalloc(sizeof(ata_ident)); //ATA identify data will be received 
	memset(dat,'\0',sizeof(ata_ident));
	while(temp)
	{
		//if(ide_drv[i]==0) continue;
		if(temp->drv_number>=4) break;
		kprintf("Selecting ");
		kprintf(temp->ps ? "Secondary ":"Primary ");
		kprintf(temp->ms ? "Slave ":"Master ");
		kprintf("drive Number : %d\n",temp->drv_number);
		
		select_device(temp);
		if(is_device_ready(temp))
		{
			kprintf("device is ready\n");
		}
		else 
		{
			kprintf("device is not ready\n");
			sleep(10);
			//goto again;
			continue;
		}
		if(is_device_busy(temp))
			sleep(10);
			kprintf("device is Busy\n");
		pio_outbyte(temp->chanl->base_reg + CMD_REG, ATA_CMD_ID); 
		unsigned char temp1 = inportb(temp->chanl->base_reg + LBA_MID_REG);
		unsigned char temp2 = inportb(temp->chanl->base_reg + LBA_HI_REG);
			
		// Now if it is pata drive it will put an error flag in flags register
		if(pio_inbyte(temp->chanl->base_reg + ERR_REG) & STA_ERR)
		{
			temp->devtype = PATA;
			id_cmd = ATA_CMD_ID; // ATA Identify
		}
		
		else
		{
			
			
			if(temp1 == 0x14 && temp2 == 0xEB)
			{
				temp->devtype = PATAPI;
				id_cmd=ATA_CMD_PID; // ATAPI identify
			}
			if(temp1 == 0x69 && temp2 == 0x96)
			{
				temp->devtype = SATAPI;
				id_cmd=ATA_CMD_PID; // we don't know how to handle it
			}
			if(temp1 == 0 && temp2 == 0 )
			{
				temp->devtype = PATA;
				id_cmd=ATA_CMD_ID; // ATA Identify
			}
			if(temp1==0x3c && temp2 == 0xc3 )
			{
				temp->devtype = SATA;
				id_cmd=0; // we don't know how to handle it
			}		
		}
		switch(id_cmd)
		{
			case ATA_CMD_ID : //we already sent it so we only will read if STA_DRQ status is present
						//pio_outbyte(temp->chanl->base_reg + CMD_REG, ATA_CMD_ID); 
					if(pio_get_status(temp->chanl->base_reg) & STA_DRQ)
					{
						pio_rep_inw(temp->chanl->base_reg,(unsigned short *)dat,256);
						IDENTIFY_TEXT_SWAP(dat->model,40);
						temp->heads = dat->discard1[6];
						temp->sectors = dat->discard1[3];
						temp->cylinders = dat->discard1[1];
						temp->sectors28= dat->lba28maxsects;
						temp->sectors48= dat->lba48maxsects;
						if((dat->capability & 1<<8) == 1<<8)
							temp->dma=1;
						if((dat->capability & 1<<9) == 1<<9)
							temp->lba=1;
						kprintf("%s\n",dat->model);
						// here read the MBR then extract the partition table
						mbr *tmbr = (mbr *)kmalloc(sizeof(mbr));
						if(!ata_r_sector(temp,0,(unsigned short *)tmbr))
							kprintf("Error reading MBR");
						else
						{
							//for the timebeing we are only intersted in partition table
							// copy it
							memcpy(temp->partition_table,tmbr->partitions,4*sizeof(partition)); 
						}
						kfree(tmbr); // so now free the allocated space.
					}
					break;
			case ATA_CMD_PID :
					pio_outbyte(temp->chanl->base_reg + CMD_REG, ATA_CMD_PID);
					pio_rep_inw(temp->chanl->base_reg,(unsigned short*)dat,256);
					IDENTIFY_TEXT_SWAP(dat->model,40);
					kprintf("%s\n",dat->model);
							
					break;
		}
		temp = temp->next;
	}
}
void display_partition_info(partition *p)
{
	if(p)
	{
		unsigned short shd=0,ssc=0,scy=0,ehd=0,esc=0,ecy=0, cyl_hi=0;
		shd = p->starting_head;
		ssc = p->starting_sec_cyl & 0x003F;
		scy = p->starting_sec_cyl;
		scy >>=6;   // 10 bits remain  the lower 2 bits are actually spill over of higher 2 bits of 10 bit
		cyl_hi= scy & 0x0003; // higher 2bits of cylinder
		scy >>=2;        // only lower 8 bits
		scy = scy | (cyl_hi<<8) ; // covert the cylinder back to normal by putting higher 2bits
		ehd = p->ending_head;
		esc = p->ending_sec_cyl & 0x003F;
		ecy = p->ending_sec_cyl;
		ecy >>=6;
		cyl_hi = scy & 0x0003;
		ecy >>=2;
		ecy = ecy | (cyl_hi<<8);
		kprintf("%c: %6d %6d %6d <-> %6d %6d %6d",((p->boot_indicator == 0x80)?'A':' '),shd,ssc,scy,ehd,esc,ecy);
		kprintf(" %10ld %10ld ",p->start_lba,p->total_sectors);
		kprintf("%02x\n",p->system_id);
	}
}
void display_slot_info()
{
	slot *temp;
	temp = slots;
	kprintf("[Displaying slot infornations]\n");
	while(temp)
	{
		kprintf(temp->ps ? "sec ":"pri ");kprintf(temp->ms ? "slv ":"mst ");
		kprintf(temp->devtype ? "SATA(PI)/PATAPI ":"PATA ");
		kprintf(temp->lba ? " LBA ":"CHS ");kprintf(temp->dma ? "DMA ":"no DMA ");
		kprintf("%d %d %d",temp->heads,temp->sectors,temp->cylinders);
		kprintf(" lba28sects= %d lba48sects= \n",temp->sectors28,temp->sectors48);
		if(temp->devtype)
		goto ml;  
		if(temp->partition_table)
		{
			// show the partition s with infos
			kprintf("================================Partition Table================================\n");
			kprintf("Sl Ac   \tsh \tss \tsc         eh\tes\tec    \tslba  \ttotal ID\n");
			int p;
			for(p=0;p<4;p++)
			{
				kprintf("[%d] ",p+1);
				display_partition_info(&temp->partition_table[p]);
			}
			kprintf("===============================================================================\n");
		}
	ml:
		// we don't know how to handle except pata so skip
		temp=temp->next;
	}
}
my_drive *drives[26]={NULL};
void scan_for_drives()
{
	slot *temp;
	temp = slots;
	int i=0;
	while(temp)
	{
		if(i>=26)
		{
			kprintf("Sorry We don't handle morethan 26 drives\n");
			break;		
		}
		if(!temp->devtype)
		{
			if(temp->partition_table)
			{
				int p;
				for(p=0; p<4; p++)
				{
					if(temp->partition_table[p].total_sectors>0)
					{
						// we have a partition size so this could be a disk
						drives[i] = my_drive_init(temp,p);
						i++;
						//kprintf("found a drive of size %ld",temp->partition_table[p].total_sectors);
					}
				}			
			}
		}
		temp = temp->next;
	}
}
void show_drive_info(int drv_num)
{
	unsigned char dump_buf[512],wrt_buf[512];
	if(drives[drv_num] != NULL)
	{
		kprintf("part = %d of ",drives[drv_num]->part_num);
		kprintf(drives[drv_num]->physical->ps ? "sec ":"pri ");
		kprintf(drives[drv_num]->physical->ms ? "slv ":"mst ");
		kprintf("size = %ld sys ID = %0x\n",drives[drv_num]->elba - drives[drv_num]->slba,drives[drv_num]->system_id);
/* FOLLOWING CODE WAS TO TEST DRIVE READ AND WRITE
		drive_read(drives[drv_num],0,dump_buf);
		memcpy(wrt_buf,dump_buf,512);
		int i;
		for(i =0; i<20; i++)
			wrt_buf[i]=0xff;
		drive_write(drives[drv_num],0,wrt_buf);
		for(i = 0; i<512; i++)
		drive_read(drives[drv_num],0,wrt_buf);
		hex_dump(wrt_buf,100);
		drive_write(drives[drv_num],0,dump_buf);
		hex_dump(dump_buf,100);
*/
	}
}
void init_ide()
{
	if(detect_ide())
	{
		populate_slots();
		//display_slot_info();
		// init a ide rw mutex here
		init_mutex(&ata_rw_mutex);
		scan_for_drives();
/* THE FOLLOWING CODE IS TO CHECK IF WE COULD ENNUMERATE DRIVES PROPERLY
		int i;
		for(i=0;i<26;i++)
			show_drive_info(i);
*/		
	}
	else
		kprintf("No PCI IDE found\n");
}

