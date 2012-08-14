#ifndef __ATARW_H__
#define __ATARW_H__

#include "ide.h"
#include "mutex.h"
extern mutex_t ata_rw_mutex;
unsigned int ata_r_sector(slot *drv,unsigned int block,unsigned short *buf);
unsigned int ata_w_sector(slot *drv,unsigned int block,unsigned short *buf);

#endif
