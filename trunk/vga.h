#ifndef _VGA_H_
#define _VGA_H_
typedef enum _Colours {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE, DARKGRAY,
	BRIGHTBLUE, BRIGHTGREEN, BRIGHTCYAN, PINK, BRIGHTMAGENTA, YELLOW, BRIGHTWHITE} Colours;
typedef enum _bool {false, true} bool;

unsigned short *videomem ;		//pointer to video memory
unsigned short crtc_mem;
unsigned int ypos ;				//offset, used like a y cord
unsigned int xpos ;				//position, used like x cord
unsigned int scrHeight =25;
unsigned int scrWidth =80;
unsigned char colour =0x7;			//Text colours
unsigned char text =0x7;
unsigned char back =0;

void init_VGA();
void clear() ;
void clear_to_line(void);
void write(char *cp) ;
void putc(char c) ;
void putch(char x);
void gotoxy(unsigned x, unsigned y) ;
void SetColour(Colours Text, Colours Back, bool blink);
unsigned char GetBackColour();
unsigned char GetTextColour();
void SetBackColour(Colours col);
void SetTextColour(Colours col);
void scrollup() ;
void setcursor(unsigned x, unsigned y);
void kprintf( const char *fmt, ... );
#endif

