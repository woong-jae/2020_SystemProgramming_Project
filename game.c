#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "gets_header.h"

#define MAX_INPUT 78
#define DATA_SIZE 87

char* data[] = {"system programming", "program", "disk", "computer", "getchar", "putchar", "user space", "system space", "kernel", "Unix", "system call", "shell", "ls", "utmp", "wtmp", "0644", "lseek", "directory", "dirent", "permission", "stat", "sticky bit", "pwd", "inode table", "hard link", "mount point", "symbolic links", "stty", "buffering", "race condition", "APPEND", "file descriptor", "terminal driver", "onlcr", "TCSANOW", "canonical mode", "crmode", "static", "blocked", "signal", "Asynchronous signal", "refresh", "sleep", "pause", "microseconds", "SIGVTALRM", "SIGPROF", "struct itimerval", "struct timeval", "fflush", "sigaction", "critical section", "reentrant", "process", "execvp", "fork", "wait", "exit value", "core dump flag", "signal number", "0x7F", "0x80", "shell scripts", "control command", "neutral", "local variables", "environment variables", "extern", "execlp", "pipe", "IPC", "dup", "dup2", "common ancestor", "socket", "disk files", "devices", "child", "parent", "fdopen", "popen", "zombie", "thread", "global variables", "pthread", "mutex", "function"};

typedef struct Word {
    char *str;
    int row, col;
    struct Word *next;
} word;
word *wordlist = NULL;

int hp = 3, score = 0, stage = 0;
char user_input[MAX_INPUT];
int cursor_position = 0;
int stage_goal[] = {80, 200, 500, 800};
int hit_score[] = {10, 20, 30, 40, 50};
int stage_speed[] = {500000, 400000, 300000, 200000, 100000};

void status_bar(void);
char* create_blank(int length);
void *word_flow(void *none);
void append_wordlist(void);
word* make_word(void);
int delete_word(char *input);
void reset(void);
int move_word(word* w);

void game(void)
{
    int delete_check;
    pthread_t t;
    srand((int)time(NULL));
    
    clear();
    draw_border();
    status_bar();
    move(MAP_HEIGHT - 1 , 2);
    refresh();

    pthread_create(&t, NULL, word_flow, NULL);
    
    memset(user_input, '\0', MAX_INPUT);
    while(hp > 0){ //유저 인풋 핸들
        char input = getch();
        if(cursor_position < MAX_INPUT) {
            if (input == '\n') {//엔터
                user_input[cursor_position] = '\0';
                delete_check = delete_word(user_input);
                if(delete_check == 1) {
                    score += hit_score[stage];
                    if(stage < 4 && score >= stage_goal[stage]) {
                        flash();
                        clear();
                        draw_border();
                        status_bar();
                        move(MAP_HEIGHT - 1 , 2);
                        reset();
                        stage++;
                    }
                    status_bar();
                }
                char *blank = create_blank(cursor_position + 1);
                mvaddstr(MAP_HEIGHT - 1, 2, blank);
                memset(user_input, '\0', MAX_INPUT);
                move(MAP_HEIGHT - 1, 2);
                cursor_position = 0;
                free(blank);
            } else if (input == 127) {//삭제
                if(cursor_position > 0) {
                    char *blank = create_blank(cursor_position + 1);
                    mvaddstr(MAP_HEIGHT - 1, 2, blank);
                    user_input[--cursor_position] = '\0';
                    move(MAP_HEIGHT - 1, 2);
                    addstr(user_input);
                    free(blank);
                } else {
                    mvaddstr(MAP_HEIGHT - 1, 2, "  ");
                    move(MAP_HEIGHT - 1, 2);
                }
            } else {
                user_input[cursor_position++] = input;
                user_input[cursor_position] = '\0';
                move(MAP_HEIGHT - 1, 2);
                addstr(user_input);
            }
        }
        refresh();
    }
    pthread_join(t, NULL); 
    reset();
    stage = 0;
    score = 0;
    input_prof();
}

void status_bar(void) {
    char cur_score[10], cur_hp[3], cur_stage[3];
    for(int i = 1 ; i < MAP_WIDTH - 1; i++) {
        mvaddch(MAP_HEIGHT - 2, i, '-');
    }
    sprintf(cur_hp, "%d", hp);
    sprintf(cur_stage, "%d", stage);
    sprintf(cur_score, "%d", score);
    mvaddstr(MAP_HEIGHT - 1, 1, ":");
    mvaddstr(MAP_HEIGHT - 3, 2, "HP: ");
    addstr(cur_hp);
    addstr(" SCORE: ");
    addstr(cur_score);
    addstr(" STAGE: ");
    addstr(cur_stage + 1);
}

char* create_blank(int length) {
    char *blank = (char*)malloc(sizeof(char) * length);
    memset(blank, ' ', length);
    blank[length - 1] = '\0';
    return blank;
}

int move_word(word* w) {
    char *blank = create_blank((int)(strlen(w->str) + 1));
    mvaddstr(w->row, w->col, blank);    
    w->col = w->col + 1;
    if(w->col + strlen(w->str) >= MAP_WIDTH)
    {
        beep();
	    hp--;
	    if(hp < 0)
	    {
		    mvaddstr(MAP_HEIGHT - 1, 28, ">>>>>Press Any Key<<<<<");
		    refresh();
		    pthread_exit(NULL);
        }
        word *cur, *prev;
        cur = wordlist;
        prev = NULL;
        while(1) {
            if(w == cur) {
                if(prev == NULL) {//cur이 첫번째
                    wordlist = cur -> next;
                } else {
                    prev->next = cur->next;
                }
                free(cur);
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        status_bar();
        return 1;
    }
    else
    	mvaddstr(w->row, w->col, w->str);
    refresh();
    free(blank);
    return 0;
}



void reset(void) {
    word *cur = wordlist;
    word *prev = NULL;
    wordlist = NULL;
    while(1) {
        if(cur == NULL) {
            break;
        }
        prev = cur;
        cur = cur->next;
        free(prev);
    }
    hp = 3;
}

int delete_word(char *input) {//발견 실패 시 '0' 반환, 성공 시 '1' 반환
    word *cur = wordlist;
    word *prev = NULL;
    while(1) {
        if(cur == NULL) {
            return 0;
        }
        if(strcmp(cur->str, input) == 0) {//처음
            char *blank = create_blank((int)(strlen(cur->str) + 1));
            mvaddstr(cur->row, cur->col, blank);
            if(prev == NULL) {
                wordlist = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(blank);
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

word* make_word(void) {
    word *new = (word *)malloc(sizeof(word));
    new->str = data[rand() % DATA_SIZE];
    new->row = rand() % (MAP_HEIGHT - 4) + 1;
    new->col = 1;
    new->next = NULL;
    return new;
}

void append_wordlist(void) {
    if (!wordlist) {
        wordlist = make_word();
    } else {
        word* cur = wordlist;
        while(cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = make_word();
    }
}

void *word_flow(void *none) {//화면에 글을 출력
    int word_rate = 10;
	while(1) {
		word_rate++;
	    if((word_rate%10)==0)
        	append_wordlist();
        word *cur = wordlist;
        word *next = NULL;
        while(1) {
            if(cur == NULL) {
                if(next == NULL) {
                    break;
                }
                cur = next;
                next = next->next;
            } else {
                move_word(cur);
                cur = cur->next;
            }
        }
        move(MAP_HEIGHT - 1 , cursor_position + 2);
        refresh();
        usleep(stage_speed[stage]);
    }
}
