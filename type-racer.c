#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW 15

int min(int a, int b){
    if (a < b) {return a;}
    return b;
}

void cycle(char str[]){
    int strLength = strlen(str);
    int j = 0;
    for (int i=-1; i<strLength; i++){
        while ((i != -1) && (getch() != str[i])) {}
        clear();
        move(0,0);
        for (int k=0; k < min(strLength, WINDOW - 1) - j; k++) {addch(str[i+k+1]);}
        if ((i < strLength - WINDOW)){
            addch(str[i+WINDOW]);
        } else {j++;}
        refresh();
    }
}


int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    cycle("my really long long string");
    
    attroff(COLOR_PAIR(1));
    endwin();
    return 0;
}
