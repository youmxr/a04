#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

void outputValues();
void userInput(char* line);
void reqRes(char* line);
void relRes(char* line);

typedef struct customer{
    int * resource;
}Customer;

int main (int argc, char *argv[]){
    

}

void userInput(char* line){
    int len = strlen(line);

    if len<1{
        outputValues();
    }

    else{
        char* original = malloc(sizeof(line));
        strcpy(original,line);

        char* token = strtok(original, " ");

        if (token == 'RQ'){
            reqRes(line);
        }
        else if (token == 'RL'){
            relRes(line);
        }

        free(original);
    }

    return;
}