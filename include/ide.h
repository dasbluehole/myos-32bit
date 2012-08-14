//ide1.h
#ifndef __IDE_H__
#define __IDE_H__
#include "idt.h"
#include "types.h"
#define ATA_BASE_PRI	0x1f0 	// default ISA
#define ATA_BASE_SEC	0x170	// default ISA
#define MASTER				0xA0
#define SLAVE				0xB0
// bellow are offset from ATA_BASE_xxx
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
/* Important bits in the device control register.
   See ATA/ATAPI-4 spec, section 7.9.6 */
#define ATA_CTL_SRST    0x04
#define ATA_CTL_nIEN    0x02
/* ATA command bytes */
#define	ATA_CMD_READ		0x20	/* read sectors */
#define	ATA_CMD_WRITE		0x30	/* write sectors */
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
#define read_le32(X)	*(unsigned int *)(X)

// devtype defines
#define PATA	0
#define PATAPI	1
#define SATA	2
#define	SATAPI	3

//swaps bytes in a short int
#define IDENTIFY_TEXT_SWAP(field,size) \
    \
    ({ \
	unsigned char tmp; \
	int i; \
        \
        for (i = 0; i < (size); i+=2) \
        { \
	    tmp = (field)[i]; \
	    (field)[i]   = (field)[i+1]; \
	    (field)[i+1] = tmp; \
	} \
    })
//==========================================
typedef struct chan
{
	unsigned short base_reg;
	unsigned short ctrl_reg;
	unsigned short bmide;
	unsigned char nIEN;
}chan;

typedef struct pci_ata
{
	chan chan0;
	chan chan1;
}pci_ata;

typedef struct partition
{
	unsigned char boot_indicator;
	unsigned char starting_head;
	unsigned short starting_sec_cyl;
	
	unsigned char  system_id;
	unsigned char  ending_head;
	unsigned short ending_sec_cyl;
	
	unsigned int   start_lba;
	unsigned int   total_sectors;
} __attribute__((packed)) partition;
typedef struct mbr
{
	unsigned char boot_code[446]; // 436 bytes of boot code + 10 bytes of Uniq ID of disk but all can be used for boot code
	partition partitions[4];      // 4 partitions
	unsigned short signature;     // 0x55,0xaa
} __attribute__((packed)) mbr;      // TOTAL 446+4*16+2=446+64+2=512 bytes
typedef struct slot
{
	unsigned char ps:1;      // primary 0, secondary 1
	unsigned char ms:1;      // master =0 slave =1
	unsigned char exists:1;  // if exists 1 else 0
	unsigned char devtype:2; // unknown 000, 001 ata, 010 atapi, 011 sata, 100 satapi 
	unsigned char lba:1;
	unsigned char dma:1;
	chan *chanl;              // which channel this drive is connected primary or secondary
	unsigned short heads;    // number of heads 
	unsigned short sectors;  // number of sectors 
	unsigned int cylinders;  // number of cylinders
	unsigned int capacity;   // total number of sectors
	unsigned int sectors28;
	unsigned long long sectors48;
	unsigned short drv_number; // drive number 0-4
	partition partition_table[4]; // a Partition table has 4 partitions
	struct slot *next;
}slot;


typedef struct ata_ident
{
	unsigned short discard1[10]; // 10 shorts -> 10 
	unsigned char  slnum[20];   // 10 shorts->20
	unsigned short discard2[3]; // 3 shorts ->23
	unsigned char  fw_rev[8];   // 4 shorts ->27
	unsigned char  model[40];   // 20 shorts->47
	unsigned short discard3[2]; //  2 shorts->49
	unsigned int   capability;  //  2       ->51 DMA 8th bit lba 9th bit
	unsigned short doscard4[9]; //  9       ->60
	unsigned int   lba28maxsects;// 2       ->62
	unsigned short discard5[2];  // 2       ->64
	unsigned short pio_modes_supported; //1 ->65  // 0-7 bits to be checked
	unsigned short discard6[15]; // 15      ->80 
	unsigned short major_ata_ver;//  1      ->81
	unsigned short minor_ata_ver;//  1      ->82
	unsigned int   cmdset_supported;//2     ->84
	unsigned short discard7[4];   //   4     ->88
	unsigned short ultraDMAfetures;// 1     ->89
	unsigned short discard8[11];   // 11     ->100
	unsigned long long lba48maxsects;//4    ->104
	unsigned short discard9[23];//  23      ->127
	unsigned short discard10[129];//129	->256  
} __attribute__((packed)) ata_ident;


//bool detect_ide();
void init_ide();
void display_partition_info(partition *p);
void display_slot_info();
#endif
