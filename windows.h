/* Windows.h
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




struct{
int Wusage;
int Wx1;
int Wy1;
int Wx2;
int Wy2;
int Wcolor;
int Wvisible;
char* Wcaption;
} window[100];


int init_windows();
void drawWindow(int x1,int y1,int x2,int y2,int color);
int creat_window();
void RdrawWindow();
void errmsgbox(char* cap,char *msg);

