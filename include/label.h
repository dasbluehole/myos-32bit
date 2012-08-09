/*
label.h

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
int x1;
int y1;
int x2;
int y2;
int color;
int visible;
char* text;
int txtcolor;
int bckcolor;
int visback;
int win;
} Rlabel[100];

int label(int Lx1,int Ly1,int Lx2,int Ly2, const char *message,int color,int bckcolor,int visback);
void Ldischar(int Cx1,int Cy1,int Cx2,int Cy2,char kar,int color);
