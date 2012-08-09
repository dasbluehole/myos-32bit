/* vga.c
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


#include "gvga.h"
#include "low-io.h"

void vgaWriteRegs(VgaRegisters *vregs); /* schreibt Regs aus*/

void vgaDelay(void);

VgaMode currentVideoMode;
VgaMode lastVideoMode;
VgaRegisters defaultRegs;


VgaRegisters mode12hregs={
		{ 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E, 0x00, 0x40,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x8C, 0xDF, 0x28,
			0x00, 0xE7, 0x04, 0xE3},

		{	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
			0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x01, 0x00, 0x0F, 0x00,
			0x00},

		{	0x00, 0x0F, 0x00, 0x20, 0x00, 0x00, 0x05, 0x0F, 0xFF },
		{	0x03, 0x01, 0x0F, 0x00, 0x06 },
		{	0xE3 }
};


/*********************************************************************/
/**
*       SET VIDEO MODE
*
*//*******************************************************************/
int setVideoMode(int mode){
	unsigned char* buff=NULL;
  if(mode == currentVideoMode.mode){
    return(1);
  } 
  if(mode== lastVideoMode.mode){
    buff = lastVideoMode.buffer;
  }
  /*if(lastVideoMode.buffer){
    releaseMemory(*lastVideoMode.buffer,lastVideoMode.memSize); 
  } */

  lastVideoMode.isTextMode=currentVideoMode.isTextMode;
  lastVideoMode.mode=currentVideoMode.mode;
  lastVideoMode.width=currentVideoMode.width;
  lastVideoMode.height=currentVideoMode.height;
  lastVideoMode.bitplanes=currentVideoMode.bitplanes;
  lastVideoMode.buffer=currentVideoMode.buffer;
  lastVideoMode.memory=currentVideoMode.memory;
  lastVideoMode.memSize=currentVideoMode.memSize;
 
  switch (mode){
  	case MODE80x25 :
			vgaWriteRegs(&defaultRegs);
			currentVideoMode.isTextMode = TRUE;
			currentVideoMode.mode = MODE80x25;
			currentVideoMode.width = 80;
			currentVideoMode.height = 25;
			currentVideoMode.bitplanes = 2;
			currentVideoMode.buffer = buff;
			currentVideoMode.memory = (unsigned char *)0x000B8000;
			currentVideoMode.memSize = 0x10000;
			break;

    case MODE640x480x16:
			vgaWriteRegs(&mode12hregs);
			currentVideoMode.isTextMode = FALSE;
			currentVideoMode.mode = MODE640x480x16;
			currentVideoMode.width = 640;
			currentVideoMode.height = 480;
			currentVideoMode.bitplanes = 4;
			currentVideoMode.buffer = buff;
			currentVideoMode.memory = (unsigned char *)0x000A0000;
			currentVideoMode.memSize = (640*480/8)*4;
			break;	
  }		

  return(1);
}



/*********************************************************************/
/** \internal
*
*		Write VGA Registers
*	Writes contens of VgaRegister *regs to the ports
*	
*
*//*******************************************************************/
void vgaWriteRegs(VgaRegisters *vregs){
	int i;
	
  outportb(MIS_W , vregs->MISRegisters[0]);
  vgaDelay();

  outportb(IS1_RC,0);
	outportb(SEQ_I,0);
 	outportb(SEQ_D,1);
	
	outportb(SEQ_I,1);
	outportb(SEQ_D,vregs->SEQRegisters[1]);
	 
	for(i=2;i<5;i++){
  	outportb(SEQ_I,i);
		outportb(SEQ_D,vregs->SEQRegisters[i]);
  	vgaDelay();
  }
	outportb(SEQ_I,0);
	outportb(SEQ_D,3);

  /* clear Protection bits  */
  outportw( CRT_IC,0x0e11);
  vgaDelay();
  for(i=0;i<25;i++){
  	outportb(CRT_IC,i);
   	vgaDelay();
   	outportb(CRT_DC,vregs->CRTRegisters[i]);
   	vgaDelay();
  }

  for (i=0; i < 9; i++) {
    outportb( GRA_I,i);
    outportb( GRA_D ,vregs->GRARegisters[i]);
  }

  for (i=0; i < 0x15; i++) {
    inportb(IS1_RC);
	  vgaDelay();
    outportb(ATT_IW, i|0x20);
  	vgaDelay();
    outportb(ATT_IW,vregs->ATTRegisters[i]);
	  vgaDelay();
	}
	inportb(IS1_RC); 
	vgaDelay();
  outportb(ATT_IW, 0x20);
}





/*********************************************************************/
/** \internal
*
*		A simple delay.
*	Gives VGA-Ports time to react. 
* Taken from LINUX's SVGAlib ( They say it's necesary.)
*
*//*******************************************************************/
void vgaDelay(void){
  int i;
  for(i=0;i<10;i++){
  }
}


/*********************************************************************/
/**
*
*		
*
*	warning For 640x480x16 only
*	
*
*//*******************************************************************/
void vgaClearScreen(void){
  unsigned regBuff[3];
	
	switch(currentVideoMode.mode){
		

		case MODE640x480x16:
				outportb(GRA_I,0x08);
			  regBuff[0]=inportb(GRA_D);
				outportb(GRA_I,0x05);
			  regBuff[1]=inportb(GRA_D);
				outportb(GRA_I,0x03);
			  regBuff[2]=inportb(GRA_D);

				outportb(GRA_I,0x05);
				outportb(GRA_D,0x02);
				outportb(GRA_I,0x03);
				outportb(GRA_D,0x00);
				outportb(GRA_I,0x08);
				outportb(GRA_D,0xff);
			  memset((unsigned char *)currentVideoMode.memory,0,currentVideoMode.memSize);
				outportb(GRA_I,0x08);
				outportb(GRA_D,regBuff[0]);
				outportb(GRA_I,0x05);
				outportb(GRA_D,regBuff[1]);
				outportb(GRA_I,0x03);
				outportb(GRA_D,regBuff[2]);
				break;

		
	}
	
}



/*********************************************************************/
/**
*
*		Sets the pixel index at the given coordinates.
*	For MODE640x480x16 only.
*
*	\param x X-Coord of the Pixel
*	\param y Y-Coord of the Pixel
*	\param color (Palettenindex)
*
*	\warning For MODE640x480x16 only.
*
*//*******************************************************************/
int vgaSetPixel(int x, int y, unsigned char color){
  int bitPos;
  unsigned char bitMask;
  char buff;
  int offset;
  
  switch(currentVideoMode.mode){
		case MODE640x480x16:		  
			offset =(y*currentVideoMode.width)/8+(x/8);
			bitPos= 7-(x % 8);
			bitMask = (1 << bitPos);
			outportb(GRA_I,0x08);   /* BitMask Reg*/
			outportb(GRA_D,bitMask);
			outportb(GRA_I,0x05);   /* GFXMode Reg*/
			outportb(GRA_D,0x02); /* write-2 read-0*/
			buff=*(unsigned char *)(currentVideoMode.memory + offset);
			*(unsigned char *)(currentVideoMode.memory+offset)=color;

			/*default Werte setzen*/ 
			outportb(GRA_I,0x08);   
			outportb(GRA_D,0xff);
			outportb(GRA_I,0x05);  
			outportb(GRA_D,0x00); 
			break;

	}				
  return(0);	
}

/*********************************************************************/
/** \internal
*	
*		Draws a vertical Line.
*	\param x 	x-Position
*	\param y1	y1-Position
*	\param y2	y2-Position
*	\param c	Color
*
* Draws a vertical Line from (x,y1) to (x,y2) wich is colored c.
*
*//*******************************************************************/
void drawVLine(int x,int y1, int y2, int c){
	int dy;
	for(dy=y1;dy<y2;dy++){
	    vgaSetPixel(x,dy,c);
 	}
}

/*********************************************************************/
/** \internal
*	
*		Draws a horizontal Line.
*	\param x1	x1-Position
*	\param x2	x2-Position
*	\param y	y-Position
*	\param c	Color
*
* Draws a horizontal Line from (x1,y) to (x2,y) wich is colored c.
*
*//*******************************************************************/
void drawHLine(int x1,int x2, int y, int c){
	int dx;
	for(dx=x1;dx<x2;dx++){
	    vgaSetPixel(dx,y,c);
 	}
}
/*********************************************************************/
/**
*
*		Draws a Line.
*	Uses Midpoint Line Algo. from Bresenham.
* \note
*		Vertical and horizontal Lines are 
*
*//*******************************************************************/
void drawLine(int x1, int y1, int x2, int y2,int c){
 	int buf;
	int dx,dy,incrE,incrNE,d,x,y;

	if(x2 < x1){
		buf=x1;
		x1=x2;
		x2=buf;
	}	
	if(y2 < y1){
		buf=y1;
		y1=y2;
		y2=buf;
	}	
	
	if(x1==x2){
		drawVLine(x1,y1,y2,c);
		return;
	}	
	if(y1==y2){
		drawHLine(x1,x2,y1,c);
		return;
	}	
	
	dx = x2-x1;
	dy= y2-y1;
	d=2*dy-dx;
	incrE=2*dy;
	incrNE=2*(dy-dx);
  x=x1;
	y=y1;
	vgaSetPixel(x,y,c);
	while(x<x2){
		if(d<=0){
			d+=incrE;
			x++;
		}else{
			d+=incrNE;
			x++;
			y++;
		}
		vgaSetPixel(x,y,c);		
	}		
}

/*********************************************************************/
/**
*
*		DRAW A BOX
*
*
*
*//*******************************************************************/
void box(int x1,int y1,int x2,int y2,int color)
{
for(;y1<=y2;y1++)drawLine(x1,y1,x2,y1,color);
}
