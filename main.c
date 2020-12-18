//
//  main.c
//  SystemProgrammingProject
//
//  Created by 정재웅 on 2020/12/18.
//

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <fcntl.h>


#define MAP_WIDTH 80
#define MAP_HEIGHT 20

void set_nodelay_mode(void);
void set_cr_noecho_mode(void);
void tty_mode(int);
void draw_border(void);
void main_screen(void);
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
    mvaddstr(5, 30, "+------------------+");
    mvaddstr(6, 30, "+      gets()      +");
    mvaddstr(7, 30, "+------------------+");
    standout();
    mvaddstr(12, 35, "- START");
    standend();
    mvaddstr(13, 35, "- SCOREBOARD");
    mvaddstr(14, 35, "- EXIT");
    mvaddstr(MAP_HEIGHT - 1, 1, "Press 'w' and 's' to move and press 'enter' to select...");
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
            if(current_position == 14) {
                break;
            }
        }
    }
    endwin();
}


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

////Set Mode
//void tty_mode(int how) {
//    static struct termios original_mode;
//    static int original_flags;
//    if (how == 0) {
//        tcgetattr(0, &original_mode);
//        original_flags = fcntl(0, F_GETFL);
//    } else {
//        tcsetattr(0, TCSANOW, &original_mode);
//        fcntl(0, F_SETFL, original_flags);
//    }
//}
//void set_cr_noecho_mode() {
//    struct termios ttystate;
//    
//    tcgetattr(0, &ttystate);
//    ttystate.c_lflag &= ~ICANON;
//    ttystate.c_lflag &= ~ECHO;
//    ttystate.c_cc[VMIN] = 1;
//    tcsetattr(0, TCSANOW, &ttystate);
//}
//void set_nodelay_mode() {
//    int termflags;
//    termflags = fcntl(0, F_GETFL);
//    termflags |= O_NDELAY;
//    fcntl(0, F_SETFL, termflags);
//}
