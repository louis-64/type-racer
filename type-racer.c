#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>



#define WINDOW 100


void init(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
}

void cycle(FILE* fd){
    char strWin[WINDOW+1];
    strWin[WINDOW] = '\0';
    char c;
    int j = 0;

    init();
    
    for (int i = 0; i < WINDOW; i++){
        c = fgetc(fd);
        if (c == EOF) {j = WINDOW - i; break;}
        strWin[i] = c;
        addch(c);
    }

    while (j != WINDOW){ 
        while (getch() != strWin[0]){}
        clear();
        move(0,0);
        for (int i = 0; i < WINDOW - j - 1; i++){c = strWin[i+1]; strWin[i] = c; addch(c);}
        if (j > 0){j++;}
        else{c = getc(fd); if (c == EOF){j++;} else {strWin[WINDOW - 1] = c; addch(c);}}
    }
    attroff(COLOR_PAIR(1));
    endwin();
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {printf("Not enough arguments.\nUsage : ./typeracer <fileName>");exit(1);}
    FILE* txt = fopen(argv[1], "r");
    if (txt == NULL) {fprintf(stderr,"Can't open the file %s.\nUsage : ./typeracer <fileName>", argv[1]);exit(1);}
    cycle(txt);
    fclose(txt);
    return 0;
}
