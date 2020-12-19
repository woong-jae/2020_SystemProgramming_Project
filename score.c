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
#include "gets_header.h"

struct ranking {
	char name[10];
	int score;
};

void scoreBoard() {
	char name[10], buf[30], input;
	int score, i = 1;

	clear();
	draw_border();
	mvaddstr(3,30,"+---SCOREBOARD---+");
	mvaddstr(5,28,"+RANK----NAME----SCORE+");
    
	sort_score();

	mvaddstr(MAP_HEIGHT-1,2,"Press 'q' to return to main...");
	refresh();
	while (1) {
		input = getch();
		if(input == 'q') {
			break;
		}
	}
}

void sort_score() {
	struct ranking* r;
    	struct ranking tmp;
	int i = 0, buf[30];
	FILE *f = fopen("score.txt", "r");

	r = (struct ranking*)calloc(30, sizeof(struct ranking)); //할당 크기
    	while (fscanf(f, "%s %d", r[i].name, &r[i].score) != EOF) {
        	i++;
    	}
    	for (int a = 0; a < 10; a++) { //for size
        	for (int b = 0; b < 10-a-1; b++) {
            		if (r[b].score < r[b+1].score) {
                		tmp = r[b];
                		r[b] = r[b + 1];
                		r[b + 1] = tmp;
            		}
        	}
    	}
	if (f == NULL) {
		mvaddstr(10,30,"NO SCORE SUBMITTED");
	}
	else {
		int n = 0;
		do {
			sprintf(buf,"%2d:%8s    %5d",n+1, r[n].name, r[n].score);
			n++;
			mvaddstr(5+n,30,buf);
		}while(n<10);
	}
	fclose(f);
}
