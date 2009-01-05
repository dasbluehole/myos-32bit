//ide1.h
#ifndef __IDE_H__
#define __IDE_H__
#include "idt.h"
#define ATA_BASE_PRI	0x1f0 
#define ATA_BASE_SEC	0x170
#define DATA_REG	0	//RW
#define ERR_REG		1	//R
#define FEATURE_REG	1	//RW
#define SECT_CNT_REG	2	//RW
#define LBA_LOW_REG	3	//rw
#define LBA_MID_REG	4	//rw
#define LBA_HI_REG	5	//rw
#define DRV_HD_REG	6	//rw
#define STATUS_REG	7	//r
#define CMD_REG		7	//W
#define ALT_ST_REG	0x206	//R
#define DEV_CTRL_REG	0x206	//w
//status register fields
#define STA_ERR		1<<0	//error
#define STA_IDX		1<<1	//index mark
#define STA_CORR	1<<2	//data corrected
#define STA_DRQ		1<<3	//Data Transfer Requested
#define STA_DSC		1<<4	//seek complete
#define STA_DF		1<<5	//Device Fault
#define STA_DRDY	1<<6	//device ready
#define STA_BSY		1<<7	//busy
//error register fields
#define ERR_AMNF	1<<0	//Address Mark Not Found
#define ERR_TK0NF	1<<1	//Track 0 Not Found
#define ERR_ABRT	1<<2	//command aborted
#define ERR_MCR		1<<3	//Media Change Requested
#define ERR_IDNF	1<<4	//ID mark Not Found
#define ERR_MC		1<<5	//Media Changed
#define ERR_UNC		1<<6	//Uncorrectable data error
#define ERR_BBK		1<<7	//Bad Block
/* ATA command bytes */
#define	ATA_CMD_READ		0x20	/* read sectors */
#define	ATA_CMD_PKT		0xA0	/* signals ATAPI packet command */
#define	ATA_CMD_PID		0xA1	/* identify ATAPI device */
#define	ATA_CMD_READMULT	0xC4	/* read sectors, one interrupt */
#define	ATA_CMD_MULTMODE	0xC6
#define	ATA_CMD_ID		0xEC	/* identify ATA device */
//helper functions(reads)
#define ata_read_error(address)		(inportb((address)+ERR_REG))
#define ata_read_status(address)	(inportb((address)+STATUS_REG))
#define ata_read_sector_count(address)	(inportb((address)+SECT_CNT_REG))
#define ata_read_feature(address)	(inportb((address)+FEATURE_REG))
#define ata_read_alt_status(address)	(inportb((address)+ALT_ST_REG))
//helper functions(writes)
#define ata_write_feature(address,data)		(outportb((address)+FEATURE_REG,(data)))
#define ata_write_drv_ctrl(address,data)	(outportb((address)+DEV_CTRL_REG,(data)))
#define ata_write_cmd(address,data)		(outportb((address)+CMD_REG,(data)))
#define ata_write_head(address,data)		(outportb((address)+DRV_HD_REG,(data)))
#define	ata_write_lba(address,lba)		(outportb((address)+LBA_LOW_REG,(lba&0x0000ff))) \
						(outportb((address)+LBA_MID_REG,(lba&0x00ff00)>>8)) \
						(outportb((address)+LBA_HI_REG,(lba&0xff0000)>>16)) \

#define NUM_IO_SPANS	2
#define	read_le16(X)	*(unsigned short *)(X)
#define	read_be16(X)	bswap16(*(unsigned short *)(X))
typedef struct
{
	unsigned char dma;			/* 8-bit DMA mask */
	unsigned short irq;			/* 16-bit IRQ mask */
	unsigned short adr[NUM_IO_SPANS];	/* start of I/O range */
	unsigned short span[NUM_IO_SPANS];	/* length of I/O range */
} io_t;
typedef struct
{
/* hardware interface (hwif; or "bus") */
	io_t io;
/* which drive on the hwif? 2 for IDE, 4 for floppy, 7 for SCSI */
	unsigned char unit;
/* generic info (i.e. used by ALL types of block device) */
	unsigned long num_blks;
	unsigned short bytes_per_blk;
/* floppy and CHS IDE only */
	unsigned short sectors, heads, cyls;
} blkdev_t;
typedef struct
{
/* generic block device info */
	blkdev_t blkdev;
/* information specific to IDE drive */
	unsigned has_lba : 1;
	unsigned use_lba : 1;
	unsigned has_dma : 1;
	unsigned use_dma : 1;
	unsigned has_multmode : 1;
	unsigned use_multmode : 1;
	unsigned short mult_count;
} ide_t;


void detect_ide();
void read_sector(unsigned short port,unsigned int blk,char *read_buf);
void ide_handler(regs *r);
void dump(void *data_p, unsigned count);
void ide_read_handler(regs *r);
#endif
