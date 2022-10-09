#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define WINDOW 100

float time_diff(struct timeval *startTime, struct timeval *stopTime)
{
    return (stopTime->tv_sec - startTime->tv_sec) + 1e-6*(stopTime->tv_usec - startTime->tv_usec);
}

void init(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));
}

void endscreen(int nbChar, int nbError, struct timeval *startTime, struct timeval *stopTime){
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    char *tmp = malloc(80*sizeof(char));
    float typingTime = time_diff(startTime, stopTime);
    snprintf(tmp,80, "Typed %d characters in %.2f seconds\nAverage : %.2f cpm\nCorrectness : %.2f%%", nbChar, typingTime, 60 * ((float) nbChar / typingTime), 100 * (float) nbChar / (nbError+nbChar));
    mvaddstr(0, 0, tmp);
    attroff(COLOR_PAIR(2));
    getch();
    endwin();
}

void cycle(FILE* fd){
    char strWin[WINDOW+1];
    strWin[WINDOW] = '\0';
    char c;
    int j = 0;
    int nbChar = 0;
    int nbError = 0;
    struct timeval startTime, stopTime; gettimeofday(&startTime, NULL);
    init();
    
    for (int i = 0; i < WINDOW; i++){
        c = fgetc(fd);
        if (c == EOF) {j = WINDOW - i; break;}
        strWin[i] = c;
        addch(c);
    }

    while (j != WINDOW){ 
        while (getch() != strWin[0]){nbError++;}
        nbChar++;
        clear(); move(0,0);
        for (int i = 0; i < WINDOW - j - 1; i++){c = strWin[i+1]; strWin[i] = c; addch(c);}
        if (j > 0){j++;}
        else{c = getc(fd); if (c == EOF){j++;} else {strWin[WINDOW - 1] = c; addch(c);}}
    }
    gettimeofday(&stopTime, NULL);
    endscreen(nbChar, nbError, &startTime, &stopTime);
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {printf("Invalid arguments.\nUsage : ./typeracer <fileName>");exit(1);}
    FILE* txt = fopen(argv[1], "r");
    if (txt == NULL) {fprintf(stderr,"Can't open the file %s.\nUsage : ./typeracer <fileName>", argv[1]);exit(1);}
    cycle(txt);
    fclose(txt);
    return 0;
}
