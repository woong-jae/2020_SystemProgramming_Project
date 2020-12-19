//
//  main.c
//  SystemProgrammingProject
//
//  Created by 정재웅 on 2020/12/18.
//

#include <stdio.h>
#include <curses.h>
#include <fcntl.h>
#include <termios.h>
#include "gets_header.h"

int main(int argc, const char * argv[]) {
    main_screen();
    return 0;
}

void main_screen() {
    char input;
    int current_position = 12;
    char selection[3][20] = {"- START", "- SCOREBOARD", "- EXIT"};
    
    initscr();
    noecho();
    clear();
    draw_border();
    draw_main();
    refresh();
    while (1) {
        input = getch();
        if(input == 'w' || input == 'W') {
            mvaddstr(current_position, 35, selection[current_position % 12]);
            if(--current_position < 12) {
                current_position = 14;
            }
            standout();
            mvaddstr(current_position, 35, selection[current_position % 12]);
            standend();
            refresh();
        }else if(input == 's' || input == 'S') {
            mvaddstr(current_position, 35, selection[current_position % 12]);
            if(++current_position > 14) {
                current_position = 12;
            }
            standout();
            mvaddstr(current_position, 35, selection[current_position % 12]);
            standend();
            refresh();
        }else if(input == '\n') {
//            if(current_position == 12) {
//                echo();
//                game();
//                noecho();
//                draw_border();
//                draw_main();
//                refresh();
//            }
            if(current_position == 13) {
                sBoard();
                clear();
                draw_border();
                draw_main();
                refresh();
                current_position = 12;
            } else if(current_position == 14) {
                break;
            }
        }
    }
    endwin();
}

void draw_main() {
    mvaddstr(5, 30, "+------------------+");
    mvaddstr(6, 30, "+      gets()      +");
    mvaddstr(7, 30, "+------------------+");
    standout();
    mvaddstr(12, 35, "- START");
    standend();
    mvaddstr(13, 35, "- SCOREBOARD");
    mvaddstr(14, 35, "- EXIT");
    mvaddstr(MAP_HEIGHT - 1, 2, "Press 'w' and 's' to move and press 'enter' to select...");
}

void draw_border(void) {
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
