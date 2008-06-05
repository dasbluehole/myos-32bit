/* vga.h
*/

/*
 *  RYMOS
 *  Copyright (C) 2003 Roberto Rodriguez 
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * contact me by e-mail<romyt01@yahoo.es><romyt01@hotmail.com>
 * ICQ#: 154747154
 *
 */

/* VGA index register ports */
/** CRT Controller Index - color emulation */
#define CRT_IC  0x3D4        
/** CRT Controller Index - mono emulation */   
#define CRT_IM  0x3B4           
/** Attribute Controller Index & Data Write Regis */
#define ATT_IW  0x3C0           
/** Graphics Controller Index */
#define GRA_I   0x3CE          
/** Sequencer Index */ 
#define SEQ_I   0x3C4 
/** PEL Write Index */         
#define PEL_IW  0x3C8 
/** PEL Read Index */
#define PEL_IR  0x3C7           

/* VGA data register ports */
/** CRT Controller Data Register - color emulatio */
#define CRT_DC  0x3D5           
/** CRT Controller Data Register - mono emulation*/
#define CRT_DM  0x3B5           
/** Attribute Controller Data Read Register */
#define ATT_R   0x3C1           
/** Graphics Controller Data Register */
#define GRA_D   0x3CF           
/** Sequencer Data Register */
#define SEQ_D   0x3C5           
/** Misc Output Read Register */
#define MIS_R   0x3CC           
/** Misc Output Write Register */
#define MIS_W   0x3C2           
/** Input Status Register 1 - color emulation */
#define IS1_RC  0x3DA           
/** Input Status Register 1 - mono emulation */
#define IS1_RM  0x3BA           
/** PEL Data Register */
#define PEL_D   0x3C9           
/** PEL mask register */
#define PEL_MSK 0x3C6           


#define MODE80x25				03
#define MODE640x480x16 	12

#define NULL (void*)0
#define FALSE 0
#define TRUE ~(FALSE)

/** The VGA Registers */
typedef struct
{
  unsigned char CRTRegisters[24];
  unsigned char ATTRegisters[21];
  unsigned char GRARegisters[9];
  unsigned char SEQRegisters[5];
  unsigned char MISRegisters[1];
}VgaRegisters;

/** The VgaMode structure (will be changed)  */
typedef struct
{
  char 		isTextMode;
  int 		mode;
  int 		width;
  int 		height;
  int		bitplanes;    		
  unsigned char *      	buffer;
  unsigned char *      	memory;
  unsigned int       	memSize;
}VgaMode;


int  setVideoMode(int mode);
void vgaClearScreen(void);
int vgaSetPixel(int x, int y, unsigned char color);
void drawLine(int x1, int y1, int x2, int y2,int c);void box(int x1,int y1,int x2,int y2,int color);

