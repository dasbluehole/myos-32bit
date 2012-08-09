/*

char.c

*/

/*

example : the code for an 'A'

	 |-------------------------------|
	1|x	 |x 	 |x	 |x	 |
	 |-------------------------------|
	2|x	 | 	 |	 |x	 |
	 |-------------------------------|
	3|x	 |x	 |x	 |x	 |
	 |-------------------------------|
	4|x	 | 	 |	 |x	 |
	 |-------------------------------|
	5|x	 | 	 |	 |x	 |


result in matrixes

*/

int chartoprint[5][4];
void setchartables();

void displaychar(int x,int y,char kar,int color);


void setchartoprint(char kar);
int displaymessages(int x,int y, const char *message,int color);
int Rsizeof( const char *message);
