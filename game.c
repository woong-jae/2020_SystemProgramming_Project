#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 20
#define DATA_SIZE 3

char* data[] = { "aaaaaa", "asdf", "abcde" };

typedef struct Word
{
	char str[100];
	int row;
	int col;
}Word;

Word list[100];
int score = 0;
int life = 3;
pthread_t pthread;
pthread_mutex_t lock;

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
	move(81,30);
	refresh();
}

void CreateList(char* str, int i)
{
	srand((int)time(NULL));
	strcpy(list[i].str, str);
	list[i].row = rand() % (MAP_HEIGHT-4) + 1;
	list[i].col = 2;
	usleep(2000000);
	Flow(i);
}

void Blank_OutputWord()
{
	for(int i=1;i<MAP_HEIGHT-2;i++)
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
	mvaddstr(MAP_HEIGHT-2,1,"---------------------------------------------");
}

void* Game_Play(void *arg)
{
        int i = 0, t;
        while(1)
        {
                CreateList(MakeStr(), i);
                Blank_OutputWord();
                for(t = 0; t<=i; t++)
                {
                        if(list[t].col<MAP_WIDTH-2)
			{
				Draw(list[t].row,list[t].col,list[t].str);
                        
			}
			else
                        {
                                life--;
                                if(life==0)
                                {
                                        //GameOver();

                                }

                        }
			
                }
		//move(81,30);
		
                i++;
                if(i>=100)
                        i = 0;
        }
}

void* User_Input();

int main()
{

	initscr();
	draw_border();
	memset(list,0,sizeof(list));
	

	pthread_t pthread2;

	pthread_create(&pthread, NULL, Game_Play, NULL);
	pthread_create(&pthread2, NULL, User_Input, NULL);
	/*
	while(life > 0)
	{
		for(num = 0; num < 50;)
		{
			if(life == 0)
				pthread_exit(NULL);
			ch = getch();
			if(ch == '\n')
			{
				input[num] = '\0';
				//FindWords();
				for(int i=0; i<50; i++)
					input[i] = '\0';
				move(81, 30);
				addstr("                                    ");
				move(81, 30);
				break;
			}
			else
			{
				input[num++] = ch;
				move(81, 30);
				addstr(input);
			}
			refresh();
		}
	}
	*/
	pthread_join(pthread, NULL);
	pthread_join(pthread2, NULL);
	Blank_OutputWord();
	refresh();
	clear();
	
	return 0;
}

void* User_Input()
{
	int i = 0, t;
        char input[50];
        int num, ch;

	 
	while(life > 0)
        {
                for(num = 0; num < 50;)
                {
                        if(life == 0)
                                pthread_exit(NULL);
                        ch = getch();
                        if(ch == '\n')
                        {
                                input[num] = '\0';
                                //FindWords();
                                for(int i=0; i<50; i++)
                                        input[i] = '\0';
                                move(81, 30);
                                addstr("                                    ");
                                move(81, 30);
                                break;
                        }
                        else
                        {
                                input[num++] = ch;
                                move(81, 30);
                                addstr(input);
                        }
                        refresh();
                }
        }
}

