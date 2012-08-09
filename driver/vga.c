#include "vga.h"
#include "low-io.h"
#include "stdargs.h"
void init_VGA()
{
	char c;	
	c = (*(unsigned short*)0x410&0x30);//Detects video card type, vga or mono
	if(c == 0x30) //c can be 0x00 or 0x20 for colour, 0x30 for mono
	{
		videomem = (unsigned short*) 0xb0000;
		crtc_mem   = 0x3B4;	// mono
	}
	else
	{
		videomem = (unsigned short*) 0xb8000;
		crtc_mem   = 0x3D4;	// colour
	}
}
void clear()
{
	unsigned int i;

	for(i = 0; i < (scrWidth * scrHeight); i++)
	{
		videomem[i] = (unsigned char) ' ' | (colour << 8) ;
	}

	gotoxy(0, 0);
}
void clear_to_line(void)
{
	while (xpos<scrWidth)
	{
		putc(' ');
	}
}
void putc(char c)
{
	int t;
	switch(c)
	{
	case '\r':                         //-> carriage return
		xpos = 0;
		break;

	case '\n':                         // -> newline (with implicit cr) 
		xpos = 0;
		ypos++;
		break;

	case 8:                            // -> backspace 
		t = xpos + ypos * scrWidth;    // get linear address 
		if(t > 0) t--;      
									   // if not in home position, step back 
		if(xpos > 0)
		{
			xpos--;
		}
		else if(ypos > 0)
		{
			ypos--;
			xpos = scrWidth - 1;
		}

		*(videomem + t) = ' ' | (colour << 8); // put space under the cursor 
		break;
	case 9:
		xpos +=4;
		break;
	default:						// -> all other characters 
		if(c < ' ') break;				// ignore non printable ascii chars 
		*(videomem + xpos + ypos * scrWidth) = c | (colour << 8);
		xpos++;							// step cursor one character 
		if(xpos == scrWidth)			// to next line if required 
		{
			xpos = 0;
			ypos++;
		}
		break;
	}

	if(ypos == scrHeight)			// the cursor moved off of the screen? 
	{
		scrollup();					// scroll the screen up 
		ypos--;						// and move the cursor back 
	}
									// and finally, set the cursor 
	setcursor(xpos, ypos);
}
void putch(char c) /* a wrapper over putc() only*/
{
	putc(c);
}
void write(char *cp)		//Puts every char in a string onto the screen
{
	while((*cp) != 0)
	 putc(*(cp++));
}
void scrollup()		// scroll the screen up one line 
{
	int t;

	disable();	//this memory operation should not be interrupted,
				//can cause errors (more of an annoyance than anything else)
	for(t = 0; t < scrWidth * (scrHeight - 1); t++)		// scroll every line up 
		*(videomem + t) = *(videomem + t + scrWidth);
	for(; t < scrWidth * scrHeight; t++)				//clear the bottom line 
		*(videomem + t) = ' ' | (colour << 8);

	enable();
}
void setcursor(unsigned x, unsigned y)	//Hardware move cursor
{
    unsigned short offset;

	offset = x + y * scrWidth;      // 80 characters per line 
	outportb(crtc_mem + 0, 14);     // MSB of offset to CRTC reg 14 
	outportb(crtc_mem + 1, offset >> 8);
	outportb(crtc_mem + 0, 15);     // LSB of offset to CRTC reg 15 
	outportb(crtc_mem + 1, offset);
}

void gotoxy(unsigned x, unsigned y)		//Software move cursor
{
	xpos = x; ypos = y;					// set cursor to location 
	setcursor(x, y);				// call the hw. cursor setup 
}

void SetTextColour(Colours col)
{						//Sets the colour of the text being displayed
	text = (unsigned char)col;
	colour = (back << 4) | text;
}

void SetBackColour(Colours col)
{						//Sets the colour of the background being displayed
	if(col > 7)
	{
		col = BLACK;
	}
	back = (unsigned char)col;
	colour = (back << 4) | text;
}

unsigned char GetTextColour()
{						//Sets the colour of the text currently set
	return (unsigned char)text;
}

unsigned char GetBackColour()
{						//returns the colour of the background currently set
	return (unsigned char)back;
}
void SetColour(Colours Text, Colours Back, bool blink)
{						//Sets the colour of the text being displayed
	SetTextColour(Text);
	SetBackColour(Back);
	if(blink)
	{
		colour = (back << 4) | text | 128;
	}
}
void kprintf( const char *fmt, ... )
{
  va_list args;
  char buf[128];
  int i=0, j, n;
	bzero(buf,128);
  va_start(args, fmt);
  n = vsprintf(buf, fmt, args);
  va_end(args);
	//write(buf);
	while(buf[i] !='\0')
	{
		putc(buf[i]);
		i++;
	}
}
