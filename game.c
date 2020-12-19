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
#define MAX_INPUT 78


char* data[] = { "aaaa", "asdf", "abcde" };

typedef struct Word {
    char *str;
	int row, col;
    struct Word *next;
} word;
word *wordlist = NULL;

int hp = 3;
char user_input[MAX_INPUT];
int cursor_position = 0;

char* create_blank(int length);
void draw_border(void);
void *word_flow(void *none);
void append_wordlist(void);
word* make_word(void);
int delete_word(char *input);
void reset_wordlist(void);
void move_word(word* w);
int main(int argc, const char * argv[])
{
    int delete_check;
    pthread_t t;
    srand((int)time(NULL));
    
    initscr();
    clear();
    draw_border();
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
                
                char *blank = create_blank(sizeof(user_input));
                mvaddstr(MAP_HEIGHT - 1, 2, blank);
                
                memset(user_input, '\0', MAX_INPUT);
                move(MAP_HEIGHT - 1, 2);
                cursor_position = 0;
                free(blank);
            } else if (input == 127) {//삭제
                if(cursor_position > 0) {
                    char *blank = create_blank(sizeof(user_input));
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
    
    endwin();
    return 0;
}

char* create_blank(int length) {
    char *blank = (char*)malloc(sizeof(char) * length);
    memset(blank, ' ', length);
    blank[length - 1] = '\0';
    return blank;
}

void move_word(word* w) {
    char *blank = create_blank(sizeof(w->str));
    mvaddstr(w->row, w->col, blank);
    w->col = w->col + 1;
    mvaddstr(w->row, w->col, w->str);
    refresh();
    free(blank);
}

void reset_wordlist(void) {
    word *cur = wordlist;
    word *next = cur->next;
    while(cur != NULL) {
        free(cur);
        cur = next;
        next = cur->next;
    }
}

int delete_word(char *input) {//발견 실패 시 '0' 반환, 성공 시 '1' 반환
    word *cur = wordlist;
    word *prev = NULL;
    while(cur != NULL) {
        if(strcmp(cur->str, input) == 0) {//처음
            char *blank = create_blank(sizeof(cur->str));
            mvaddstr(cur->row, cur->col, blank);
            if(cur == wordlist) {
                wordlist = cur->next;
            } else if (cur->next == NULL) {//끝
                prev->next = NULL;
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
    new->row = rand() % (MAP_HEIGHT-4) + 1;
    new->col = 1;
    new->next = NULL;
    return new;
}

void append_wordlist(void) {
    //sleep(1);
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
        while(cur) {
            move_word(cur);
            cur = cur->next;
        }
        move(MAP_HEIGHT - 1 , cursor_position + 2);
        refresh();
        usleep(200000);
    }
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
