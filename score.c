//
// SystemProgrammingProject
// 이름과 점수가 저장된 파일을 이용하여 scoreboard 출력
//
// baekseonguk on 2020/12/19
//

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <fcntl.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 20

void draw_border();
void sBoard();

void draw_border() {
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

void sBoard() {
	FILE *f = fopen("score.txt", "r");
	char name[10], buf[30], input;
	int score, i = 1;

	clear();
	draw_border();
	mvaddstr(3,30,"+---SCOREBOARD---+");
	mvaddstr(5,28,"+RANK----NAME----SCORE+");

	while (fscanf(f, "%s %d", name, &score) != EOF) {
		sprintf(buf,"%2d:%8s    %5d",i++, name, score);
		mvaddstr(5+i,30,buf);
        }
	mvaddstr(MAP_HEIGHT-1,2,"Press 'q' to main...");
	refresh();
	while (1) {
		input = getch();
		if(input == 'q') {
			break;
		}
	}
	fclose(f);
}

void main() {
	initscr();
	draw_border();
	sBoard();
	endwin();
}
