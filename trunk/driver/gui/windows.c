/* Windows.c
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




#include "windows.h"
#include "gvga.h"
#include "char.h"
#include "label.h"

int largo;
int maxwindows=0;
int curmaxwin=99;


#define black		0x0          /* dark colors */
#define blue		0x1
#define green		0x2
#define cyan		0x3
#define red		0x4
#define magenta		0x5
#define brown		0x6
#define lightgray	0x7
#define darkgray	0x8          /* light colors */
#define lightblue	0x9
#define lightgreen	0xa
#define lightcyan	0xb
#define lightred	0xc
#define lightmagenta	0xd
#define yellow		0xe
#define white		0xf


void drawWindow(int x1,int y1,int x2,int y2,int color){

	drawLine(x1,y1,x1,(y1+y2),white);
	drawLine((x1+x2),y1,(x1+x2),(y1+y2),black);
	drawLine(x1,y1,(x1+x2),y1,white);
	drawLine(x1,(y1+y2),(x1+x2),(y1+y2),black);



	box((x1+1),(y1+16),(x1+(x2-1)),(y1+(y2-1)),color);

largo=sizeof("Window  bad  drawwed for test$");


/*THEME*/

	drawLine((x1+1),y1,(x1+1),(y1+y2),lightgray);
	drawLine((x1+x2-1),y1,(x1+x2-1),(y1+y2),lightgray);
	drawLine(x1+1,(y1+y2-1),(x1+x2-1),(y1+y2-1),lightgray);

	box((x1+1),(y1+1),(x1+(x2-1)),(y1+13),lightgray);
        box((x1+2),(y1+2),(x1+(largo*6)),(y1+12),blue);

        drawLine((x1+2+(largo*6)),(y1+4),(x1+x2-2),(y1+4),white);
        drawLine((x1+2+(largo*6)),(y1+5),(x1+x2-2),(y1+5),black);
        drawLine((x1+2+(largo*6)),(y1+7),(x1+x2-2),(y1+7),white);
        drawLine((x1+2+(largo*6)),(y1+8),(x1+x2-2),(y1+8),black);
        drawLine((x1+2+(largo*6)),(y1+10),(x1+x2-2),(y1+10),white);
        drawLine((x1+2+(largo*6)),(y1+11),(x1+x2-2),(y1+11),black);

        drawLine((x1+2),(y1+14),(x1+x2-2),(y1+14),black);
        drawLine((x1+2),(y1+15),(x1+x2-2),(y1+15),white);
        drawLine((x1+2),(y1+16),(x1+x2-2),(y1+16),lightgray);

        displaymessages((x1+5),(y1+5),"WINDOW TEST$",white);















}


int init_windows(){
int initW;
int Wmax=99;
    for (initW=0;initW<=Wmax;initW++)
    {
    window[initW].Wusage=0;
    window[initW].Wx1=0;
    window[initW].Wy1=0;
    window[initW].Wx2=0;
    window[initW].Wy2=0;
    window[initW].Wcolor=0;
    window[initW].Wvisible=0;
    window[initW].Wcaption="NONE$";
    
    }
return 0;
}



int creat_window(){
int chkwin;
int wincreated=0;
int selwin;

if (maxwindows >= 1 && maxwindows < curmaxwin){
for (chkwin=0;chkwin<=maxwindows;chkwin++)
    {
    if (window[chkwin].Wusage==0 && wincreated==0){
    window[chkwin].Wusage=1;
    window[chkwin].Wx1=0;
    window[chkwin].Wy1=0;
    window[chkwin].Wx2=0;
    window[chkwin].Wy2=0;
    window[chkwin].Wcolor=0;
    window[chkwin].Wvisible=0;
    window[chkwin].Wcaption="NONE$";
    selwin=chkwin;
    wincreated=1;
    
    
       }
    else if (chkwin==maxwindows && wincreated==0){
    maxwindows++;
    
    selwin=maxwindows;
    
    window[selwin].Wusage=1;
    window[selwin].Wx1=0;
    window[selwin].Wy1=0;
    window[selwin].Wx2=0;
    window[selwin].Wy2=0;
    window[selwin].Wcolor=0;
    window[selwin].Wvisible=0;
    window[selwin].Wcaption="NONE$";
       
    wincreated=1;
    
    
     } 
    }
   }
else if(maxwindows < 1)
    {
    selwin=maxwindows;
    
    window[selwin].Wusage=1;
    window[selwin].Wx1=0;
    window[selwin].Wy1=0;
    window[selwin].Wx2=0;
    window[selwin].Wy2=0;
    window[selwin].Wcolor=0;
    window[selwin].Wvisible=0;
    window[selwin].Wcaption="NONE$";
    maxwindows++;
    
    }
else
    {
    window[curmaxwin].Wusage=1;
    window[curmaxwin].Wx1=200;
    window[curmaxwin].Wy1=200;
    window[curmaxwin].Wx2=100;
    window[curmaxwin].Wy2=100;
    window[curmaxwin].Wcolor=lightgray;
    window[curmaxwin].Wvisible=1;
    window[curmaxwin].Wcaption="ERROR MAX WIN$";
    }       
        
return (selwin);
}

void RdrawWindow(void)
{
int Rdrwin;
int x1;
int y1;
int x2;
int y2;
int color;
char* caption;
int Lcaption[100];

for (Rdrwin=0;Rdrwin<=maxwindows;Rdrwin++)
    {
    if (window[Rdrwin].Wusage==1 && window[Rdrwin].Wvisible==1){
    x1=window[Rdrwin].Wx1;
    y1=window[Rdrwin].Wy1;
    x2=window[Rdrwin].Wx2;
    y2=window[Rdrwin].Wy2;
    color=window[Rdrwin].Wcolor;
    caption=window[Rdrwin].Wcaption;
    Lcaption[Rdrwin]=Rsizeof(window[Rdrwin].Wcaption);
    
        
	drawLine(x1,y1,x1,(y1+y2),white);
	drawLine((x1+x2),y1,(x1+x2),(y1+y2),black);
	drawLine(x1,y1,(x1+x2),y1,white);
	drawLine(x1,(y1+y2),(x1+x2),(y1+y2),black);



	box((x1+1),(y1+16),(x1+(x2-1)),(y1+(y2-1)),color);

largo=Lcaption[Rdrwin];     /*sizeof(caption);
*/

/*THEME*/

	drawLine((x1+1),y1,(x1+1),(y1+y2),lightgray);
	drawLine((x1+x2-1),y1,(x1+x2-1),(y1+y2),lightgray);
	drawLine(x1+1,(y1+y2-1),(x1+x2-1),(y1+y2-1),lightgray);

	box((x1+1),(y1+1),(x1+(x2-1)),(y1+13),lightgray);
        box((x1+2),(y1+2),(x1+(largo*6)),(y1+12),blue);

        drawLine((x1+2+(largo*6)),(y1+4),(x1+x2-2),(y1+4),white);
        drawLine((x1+2+(largo*6)),(y1+5),(x1+x2-2),(y1+5),black);
        drawLine((x1+2+(largo*6)),(y1+7),(x1+x2-2),(y1+7),white);
        drawLine((x1+2+(largo*6)),(y1+8),(x1+x2-2),(y1+8),black);
        drawLine((x1+2+(largo*6)),(y1+10),(x1+x2-2),(y1+10),white);
        drawLine((x1+2+(largo*6)),(y1+11),(x1+x2-2),(y1+11),black);

        drawLine((x1+2),(y1+14),(x1+x2-2),(y1+14),black);
        drawLine((x1+2),(y1+15),(x1+x2-2),(y1+15),white);
        drawLine((x1+2),(y1+16),(x1+x2-2),(y1+16),lightgray);

        displaymessages((x1+5),(y1+5),caption,white);










      }

     }


}

void errmsgbox(char* cap,char* msg)
{
int msgwin;
int WWx2;
WWx2=Rsizeof(msg);
msgwin=creat_window();
    
    window[msgwin].Wx1=150;
    window[msgwin].Wy1=200;
    window[msgwin].Wx2=(75+(WWx2*6));
    window[msgwin].Wy2=100;
    window[msgwin].Wcolor=lightgray;
    window[msgwin].Wvisible=1;
    window[msgwin].Wcaption=cap;
    
    RdrawWindow();
    /*displaymessages(160,250,msg,white);
    */
    label(153,216,((75+(WWx2*6))-10),60,msg,red,cyan,1);

}
