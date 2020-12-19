#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 20
#define DATA_SIZE 3

char* data[] = { "aaa", "bbb", "cdd" };

typedef struct Word
{
	char str[100];
	int row;
	int col;
}Word;

Word list[100];
int score = 0;
int life = 0;

char* MakeStr()
{
	srand((int)time(NULL));
	int index = rand() % DATA_SIZE;
	return data[index];
}

void Flow(int i)
{
	int t;
	for (t = 0; t <= i; t++)
		list[t].col += 4;
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
	list[i].row = rand() % (MAP_HEIGHT-2) + 1;
	list[i].col = 2;
	usleep(2000000);
	Flow(i);
}

void Blank_OutputWord()
{
	for(int i=1;i<MAP_HEIGHT;i++)
	{
		move(i,1);
		addstr("                                                                             ");
	}
}

void draw_border()
{
	for(int i = 0 ; i < MAP_WIDTH; i++) {
		mvaddch(0, i, '-');
   	}
   	for(int i = 0 ; i < MAP_WIDTH; i++) {
       		mvaddch(MAP_HEIGHT, i, '-');
    	}
    	for(int i = 1 ; i < MAP_HEIGHT; i++) {
        	mvaddch(i, 0, '|');
    	}
    	for(int i = 1 ; i < MAP_HEIGHT; i++) {
        	mvaddch(i, MAP_WIDTH - 1, '|');
    	}
}

int main()
{
	int i = 0, t;
	initscr();
	draw_border();
	memset(list,0,sizeof(list));
	while(1)
	{
		CreateList(MakeStr(), i);
		Blank_OutputWord();
		for(t = 0; t<=i; t++)
		{
			if(list[t].col<MAP_WIDTH+2)
				Draw(list[t].row,list[t].col,list[t].str);
		}
		i++;
		if(i>=100)
			i = 0;
	}

	return 0;
}
