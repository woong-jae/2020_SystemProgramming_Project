#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>


char* data[] = { "aaa", "bbb", "cdd" };

typedef struct Word
{
	char str[100];
	int row;
	int col;
}Word;

Word list[100];

char* MakeStr()
{
	srand((int)time(NULL));
	int index = rand()%3;
	return data[index];
}

void Flow(int i)
{
	int t;
	for (t = 0; t <= i; t++)
		list[t].row += 2;
}

void Draw(int row, int col, char *str)
{
	move(row, col);
	addstr(str);
	refresh();
}

void CreateList(char* str, int i)
{
	srand((int)time(NULL));
	strcpy(list[i].str, str);
	list[i].col = rand() % 40 + 10;
	list[i].row = 2;
	usleep(2000000);
	Flow(i);
}

void Blank_OutputWord()
{
	for(int i=2;i<21;i++)
	{
		move(i,10);
		addstr("                                                            ");
	}
}


int main()
{
	int i = 0, t;
	initscr();
	memset(list,0,sizeof(list));
	while(1)
	{
		CreateList(MakeStr(), i);
		Blank_OutputWord();
		for(t = 0; t<=i; t++)
			Draw(list[t].row,list[t].col,list[t].str);
		i++;
		if(i>=100)
			i = 0;
	}

	return 0;
}
