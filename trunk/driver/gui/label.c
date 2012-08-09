/* label.c  RYMOS Labels 
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





#include "char.h"

#include "label.h"
#include "gvga.h"

int label(int Lx1,int Ly1,int Lx2,int Ly2, const char *message,int color,int bckcolor,int visback)
	{
	int counterLF;
	int Lxfactortoadd=3;
	int Lyfactortoadd=2;
	if (visback==1)	box(Lx1,Ly1,(Lx1+Lx2),(Ly1+Ly2),bckcolor);
	for(counterLF=0; ;counterLF++)
		{
		if(message[counterLF]=='$')return 0;
		if(Lxfactortoadd>=(Lx2-8))
		{
		Lyfactortoadd=Lyfactortoadd+8;
		Lxfactortoadd=3;
		}
		Ldischar((Lx1+Lxfactortoadd),Ly1+Lyfactortoadd,(Lx1+(Lx2-1)),(Ly1+(Ly2-1)),message[counterLF],color);
		Lxfactortoadd=Lxfactortoadd+6;
		}
	}
	
	

void Ldischar(int Cx1,int Cy1,int Cx2,int Cy2,char kar,int color)
	{
	int row,column;
	int helpx;
	
	void setchartoprint(char);
	
	helpx=Cx1;
	setchartoprint(kar);
	for(row=0;row<=4;row++)
		{
		for(column=0;column<=3;column++)
			{
			if(chartoprint[row][column]==1)
				{
				if (Cx1<=Cx2 && Cy1<=Cy2)
				vgaSetPixel(Cx1,Cy1,color);
				}
			Cx1++;
			}
		Cx1=helpx;
		Cy1++;
		}
	}
