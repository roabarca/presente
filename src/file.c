#include "file.h"
#include <stdio.h>
#include <stdlib.h>

int puntajeMaximo(){ // Nos retorna el valor escrito en hiscore.txt
    FILE* fp;
    int highscore;
    fp = fopen("resources/hiscore.txt", "r"); // read mode
    if (fscanf(fp, "%d", &highscore) == EOF){
        highscore = 0;
    }
    else{
        fscanf(fp, "%d", &highscore);
    }
    fclose(fp); 
    return highscore;
}

void cambiarPuntaje(int *score){ // Cambia el valor de hiscore.txt si score >= highscore
    FILE *fp;
    int highscore = puntajeMaximo();
    if (*score >= highscore){
        fp = fopen("resources/hiscore.txt", "w"); // write mode
        fprintf(fp, "%d", *score);
        fclose(fp); 
    }
}