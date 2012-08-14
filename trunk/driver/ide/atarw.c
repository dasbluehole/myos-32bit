#include "atarw.h"
#include "helper.h"
#include "timer.h"
#include "stdio.h"
//=======================raw read/write code =================
// generic function to read or write a sector
// drv is the ide drive , block is LBA , direction is read =0 write =1
mutex_t ata_rw_mutex; // mutex for our read/write function
unsigned int ata_rw_sector(slot *drv,unsigned int block,unsigned short *buf,unsigned char direction) 
{
	unsigned char sc, cl, ch, hd, cmd;
	unsigned short iobase = drv->chanl->base_reg;
	//iobase = drv->chanl->base_reg;
	if(drv->sectors28<block)
		return 0;
	/* put exclusion thigs here */
	mutex_try_lock(&ata_rw_mutex);
	//select_device(drv);
	/*{
		cout<<"select drive failed \n";
		// uninitialize exclusion here
		return 0;	
	}*/
	//stop_ata_intr(drv->chanl->ctrl_reg);
	if (drv->lba) 
	{
		sc = block & 0xff;
		cl = (block >> 8) & 0xff;
		ch = (block >> 16) & 0xff;
		hd = (block >> 24) & 0x0f;
		//if(drv->ms)
		//	hd |=(1<<4);
		//if(drv->ps)
		//	hd |=0xf0;
	} 
	else 
	{
        /* See http://en.wikipedia.org/wiki/CHS_conversion */
	        int cyl = block / (drv->heads * drv->sectors);
	        int tmp = block % (drv->heads * drv->sectors);
	        sc = tmp % drv->sectors + 1;
	        cl = cyl & 0xff;
	        ch = (cyl >> 8) & 0xff;
	        hd = tmp / drv->sectors;
	}
	if(direction == 0)
	{
		cmd= ATA_CMD_READ;
	}
	else
	{
		cmd= ATA_CMD_WRITE;	
	}
	pio_outbyte(iobase + FEATURE_REG,0); // ????
	pio_outbyte(iobase + SECT_CNT_REG, 1); //we want only one sector
	pio_outbyte(iobase + LBA_LOW_REG, sc);
	pio_outbyte(iobase + LBA_MID_REG, cl);
	pio_outbyte(iobase + LBA_HI_REG, ch);
	pio_outbyte(iobase + DRV_HD_REG, 0xE0|hd|drv->ms<<4);//(drv->ms<<4)|0xE0|hd should be passed 
						//according to http://wiki.osdev.org/ATA_PIO_Mode#28_bit_PIO
						// but it never works
	pio_outbyte(iobase + CMD_REG, cmd);
	/* The host shall wait at least 400 ns before reading the Status register.
	See PIO data in/out protocol in ATA/ATAPI-4 spec. */
	sleep(30);
	//cout<<" Reading hd= "<<(int)hd<<" sect= "<<(int)sc<<" cyl= "<<(int)cl<<"\n";
	//while(pio_inbyte(iobase+DATA_REG)==0x08);
/*	while (timeout)	
	{
		// wait for busy flag to clear
		if(!pio_inbyte(iobase + STATUS_REG)& STA_BSY)
			break;
		timeout--;
		tmr->sleep(1);
	}
	if(timeout==0)
	{
		// put unlock for mutex here
		cout<<"Time out but device never came back from busy state\n";	
		return 0;
	}*/
	/* Did the device report an error? */
	if (pio_inbyte(iobase + ALT_ST_REG) & STA_ERR) 
	{
		//put unlock mutex 
		kprintf("Error in operation\n");
		mutex_unlock(&ata_rw_mutex);
		return 0;
	}
	// we are ok till now
	// as we have stopped the interrupt we should poll data
	/*timeout = 300000;
	for(; timeout>0; timeout--)
	{
		if(pio_inbyte(iobase + STATUS_REG) & STA_DRQ)
			break;
	}
	if(timeout==0)
	{
		// put unlock for mutex here
		cout<<"time out waiting for data request\n";	
		return 0;
	}*/
	// well our request is successfull
	// now read it to the output buffer
	while(!(pio_inbyte(iobase + ALT_ST_REG) & STA_DRQ));
	if(direction == 0)
	{
		//cmd= ATA_CMD_READ;
		pio_rep_inw(iobase + DATA_REG, buf, 256);
		while(is_device_busy(drv))
		{
			sleep(30);
		}
	}
	else
	{
		//cmd= ATA_CMD_WRITE;
		pio_rep_outw(iobase + DATA_REG, buf, 256);	
	}
	mutex_unlock(&ata_rw_mutex);
	return 1;
}
unsigned int ata_r_sector(slot *drv,unsigned int block,unsigned short *buf)
{
	
	return ata_rw_sector(drv, block, buf,0);
}
unsigned int ata_w_sector(slot *drv,unsigned int block,unsigned short *buf)
{
	return ata_rw_sector(drv, block, buf,1);
}
