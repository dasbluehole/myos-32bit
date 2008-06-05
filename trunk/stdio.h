// **************************************************************************
// * This file is part of Mojo OS libc.
// *
// * Feel free to copy it as long as this notice remains unchanged. If you
// * change anything in this code and then re-distribute it then remember to
// * make a note on how it differ from the original.
// *
// * (C) Copyright 2002-2003, Christian Lange
// **************************************************************************

#ifndef _STDIO_
#define _STDIO_

int printf(const char*, ...);
void pad_str(char*, char*, char, int, int);

char* gets(char*, int);
int puts(char*);

typedef enum {
  LEFT,
  RIGHT
} side_enum;
enum {FALSE,TRUE};
#endif
