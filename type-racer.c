#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW 115
#define MAX_SIZE 30
#define MAX_SIZE_S 10 // maximum length of the number of words ex: 10 => maximum number of words is 10^11 - 1

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
        if ((i + WINDOW < strLength)){
            addch(str[i+WINDOW]);
        } else {j++;}
        refresh();
    }
}

void cycleFile(FILE *fd){
    char str[MAX_SIZE];
    char strSizeS[MAX_SIZE_S];
    char tmp[30];
    if (fgets(strSizeS, MAX_SIZE_S, fd) == NULL) {fprintf(stderr, "Can't read the number of words in the file.");exit(1);}
    int strSize = atoi(strSizeS);
    char strWin[WINDOW+1] = {' '};
    strWin[WINDOW] = '\0';
    char c;
    int j = 0;
    for (int i = 0; i<min(strSize, WINDOW); i++){
        c = fgetc(fd);
        strWin[i] = c;
        addch(c);
    }
    // strWin contains the first window of chars, it's displayed, let's iterate through the string
    for (int i = 0; i<strSize;i++){
        while (getch() != strWin[0]){}
        clear();
        move(0,0);
        for (int k=0; k<min(strSize,WINDOW) -j-1;k++){
            c = strWin[k+1];
            strWin[k] = c;
            addch(c);
        }
        if ((i + WINDOW < strSize)){
            c = fgetc(fd);
            strWin[WINDOW-1] = c;
            addch(c);
        } else {j++;}
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

    //cycle("my really long long string");
    FILE* txt = fopen("text.txt", "r");
    cycleFile(txt);
    fclose(txt);
    attroff(COLOR_PAIR(1));
    endwin();
    return 0;
}
