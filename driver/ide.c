#include "ide.h"
#include "low-io.h"
#include "idt.h"
#include "stdio.h"
#define MASTER 0
#define SLAVE 1
int ide_drv[4]={0};
unsigned char ide_interrupt_14=0;
unsigned char rd_buf[512]={0};
ide_t ide0,ide1,ide2,ide3; //maximum 4 physical disks on 2 IDE controller
//swaps bytes in a short int
unsigned short bswap16(unsigned short arg)
{
	return ((arg << 8) & 0xFF00) |
		((arg >> 8) & 0xFF00);
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
void populate_ide_disk(unsigned short which, unsigned short unit,ide_t *ide) //which = ATA_BASE_PRI/SEC, unit = MASTER/SLAVE
{
	ide->blkdev.io.dma=0;
	ide->blkdev.io.adr[0]=which;
	ide->blkdev.io.adr[1]=which+DEV_CTRL_REG;
	ide->blkdev.io.span[0]=8;
	ide->blkdev.io.span[1]=1;
	if(which==0x1f0)
	{
		ide->blkdev.io.irq=0x4000;
		
	}
	else
	{
		ide->blkdev.io.irq=0x8000;
	}
	if(unit==MASTER)
		ide->blkdev.unit=0x0a;
	else
		ide->blkdev.unit=0xB0;
}
int wait_ready(unsigned short port,unsigned int timeout)
{
	unsigned char stat;
	outportb(port+ERR_REG,0);
	while(timeout)
	{
		stat=ata_read_status(port);
		if(!stat) return 1;
		if((stat & (STA_DRDY|STA_BSY))==STA_DRDY) return 1;
		timeout--;
	}
	return -1;
}
int ide_status_ready(ide_t *ide,unsigned int timeout)
{
	return wait_ready(ide->blkdev.io.adr[0],timeout);
}
int ide_select(ide_t *ide)
{
	ata_write_head(ide->blkdev.io.adr[0],ide->blkdev.unit);
	sleep(4);
	return ide_status_ready(ide,1000);
}
int wait_interrupt(unsigned short int_mask,unsigned int timeout)
{
	unsigned short interrupted=0;
	while(timeout--)
	{
		if(ide_interrupt_14!=0)
		{
			interrupted=1;
			break;
		}
		sleep(10);
	}
	if(!interrupted)
		return -1;
	else
		return 1;
}
//populates parameters after detecting 
int identify(ide_t *dev)
{
	unsigned char temp1=0, temp2=0, id_cmd=0, buf[512]="\0", swap_chars=0;
	unsigned short ioadr, temp, id_delay;	
	kprintf("Identify\n");
	temp1=inportb(dev->blkdev.io.adr[0]+ SECT_CNT_REG);
	temp2=inportb(dev->blkdev.io.adr[0]+ LBA_LOW_REG);
	if(temp1 != 0x01 || temp2 != 0x01)
	{
		kprintf("no drive on interface 0x%x\n",dev->blkdev.io.adr[0]);
		return -1;
	}
	temp1 = inportb(dev->blkdev.io.adr[0] + LBA_MID_REG);
	temp2 = inportb(dev->blkdev.io.adr[0] + LBA_HI_REG);
	temp = inportb(dev->blkdev.io.adr[0] + STATUS_REG);
	if(temp1 == 0x14 && temp2 == 0xEB)
	{
		kprintf("PATAPI CD forget it!!!\n");
	}
	else if(temp1 == 0x69 && temp2 == 0x96)
	{
		kprintf("SATAPI CD forget it!!!\n");
	}
	else if(temp1 == 0 && temp2 == 0 && temp != 0)
	{
		kprintf("PATA drive, ");
		id_cmd = ATA_CMD_ID;
		id_delay = 30000;	/* WAIT_ID */
		dev->blkdev.bytes_per_blk = 512;
	}
	else if(temp1==0x3c && temp2 == 0xc3 && temp != 0)
	{
		kprintf("SATA drive, ");
		id_cmd = ATA_CMD_ID;
		id_delay = 30000;
		dev->blkdev.bytes_per_blk=512;
	}
	else
	{	kprintf("unknown drive type temp1= %d temp= %d\n");
		return -1;
	}
	outportb(dev->blkdev.io.adr[0]  + CMD_REG, id_cmd);
	sleep(10);
	if(wait_ready(dev->blkdev.io.adr[0],30000)!=1)
	{
		kprintf("WARNING: timeout on waiting\n");
		kprintf("Continuing furher is erroneous\n");
	}
	insw(dev->blkdev.io.adr[0]+ DATA_REG, (unsigned short *)buf, 256);/*read ATAPI xfer count */
	for(temp = 54; temp < 94; temp += 2)
	{
		//kprintf("%c",(char)buf[(temp + 0) ^ swap_chars]);
		//kprintf("%c",(char)buf[(temp + 1) ^ swap_chars]);
	}
	dev->blkdev.cyls = read_le16(buf + 2);
	dev->blkdev.heads = read_le16(buf + 6);
	dev->blkdev.sectors = read_le16(buf + 12);
	kprintf("\n CHS=%d:%d:%d\n",dev->blkdev.cyls,dev->blkdev.heads,dev->blkdev.sectors);
	if((buf[99] & 1) != 0)
	{
		kprintf("DMA, ");
		dev->has_dma = 1;
	}
	if((buf[99] & 2) != 0)
	{
		kprintf("LBA, ");
		dev->has_lba = 1;
	}
	if(((buf[119] & 1) != 0) && (buf[118] != 0))
	{
		temp = buf[94];
		kprintf("mult_count= %d\n", temp);
	}
	else
		temp = 1;
	dev->mult_count = temp;
	kprintf("%d",(read_le16(buf + 42) >> 1),"K cache\n");
	return 1;
}
//port = 1f0 for primary controller detection and 170 for secondary controller	
int detect_cntrlr(unsigned short port)
{
	unsigned char temp1,temp2;
	outportb(port+LBA_LOW_REG,0x55);
	outportb(port+LBA_MID_REG,0xAA);
	temp1=inportb(port+LBA_LOW_REG);
	temp2=inportb(port+LBA_MID_REG);
	if((temp1==0x55)&&(temp2==0xAA))
	{
		outportb(port+DEV_CTRL_REG,0x08|0x04|0x02);	//soft reset ????	
		sleep(10);
		outportb(port + DEV_CTRL_REG, 0x08|0x02);
		sleep(10);
		return 1;
	}
	else
		return 0;
}
void detect_ide()
{
	kprintf("Detecting Primary Controller\n");
	if(detect_cntrlr(ATA_BASE_PRI)==1)
	{
		kprintf("Primary controller found\n");
		ide_interrupt_14=0;
		irq_install_handler(14,ide_handler);
		populate_ide_disk(ATA_BASE_PRI, MASTER,&ide0);
		populate_ide_disk(ATA_BASE_PRI, SLAVE,&ide1);
		if(identify(&ide0)!=1)
			kprintf("Problem in identifying IDE Primary master\n");
		if(identify(&ide1)!=1)
			kprintf("Problem in identifying IDE Primary slave\n");
		
		//irq_install_handler(7,ide_read_handler);
	}
	else
		kprintf("No primary controller\n");
	
	kprintf("Detecting Secondary Controller\n");
	if(detect_cntrlr(ATA_BASE_SEC)==1)
		kprintf("secondary controller found\n");
	else
		kprintf("No secondary controller\n");
}
void ide_handler(regs *r)
{
	ide_interrupt_14 |= 0x4000;
	kprintf("IDE interrupt\n");
}
void ide_read_handler(regs *r)
{
	kprintf("Read handler\n");
}


static inline read_sect(unsigned char port,unsigned char *buf)
{
	insw(port+DATA_REG,buf,256);
}
void read_sector(unsigned short port,unsigned int blk,char *read_buf)
{
	unsigned char head,lo,mid,hi,tmp;	
	int i=0;
	while(!wait_ready(port,1000));
	outportb(port+FEATURE_REG,0);

	head=blk>>24;
	head=head&0x0f;
	hi=blk>>16;
	hi=hi&0xff;
	mid=blk>>8;
	mid &=0xff;
	lo=blk&0xff;
	outportb(port+LBA_LOW_REG,lo);
	outportb(port+LBA_MID_REG,mid);
	outportb(port+LBA_HI_REG,hi);
	outportb(port+SECT_CNT_REG,1);
	//ata_write_lba(port,blk);
	outportb(port+DRV_HD_REG,head);
	//ata_write_head(port,head);
	outportb(port+CMD_REG,ATA_CMD_READ);
	//ata_write_cmd(port,0x20);
	while (!(inportb(port+STATUS_REG) & STA_DRQ));
	for (i = 0; i < 256; i++)
	{
		tmp = inportw(port+DATA_REG);
		read_buf[i * 2] =  tmp;
		read_buf[(i*2)+1] = (tmp >> 8);
	}
	//read_sect(port,read_buf);
	//ata_write_cmd(port,0x21);
}
#define BPERL 16
void dump(void *data_p, unsigned count)
{
	unsigned char *data = (unsigned char *)data_p;
	unsigned i, j;

	while(count != 0)
	{
		for(i = 0; i < BPERL; i++)
		{
			if(count == 0)
				break;
			kprintf("%x ",data[i]);
			count--;
		}
		kprintf("\t");
		for(j = 0; j < i; j++)
		{
			if(data[j] < ' ')
				kprintf(".");
			else
				kprintf("%x",data[j]);
		}
		kprintf("\n");
		data += BPERL;
	}
}

