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
    int* maximum;
}Customer;

Customer* customers;
int* allocated;
int* available;
int* need;

int main (int argc, char *argv[]){
    char inputFile[100];

    FILE* fp = fopen("sample4_in.txt", "r");
    if (fp==NULL){
        printf("could not find/open file.");
    }

    for(int i = 0; fgets(inputFile,sizeof(inputFile),fp); i++){
        char* tok = strtok(inputFile, ",");
        int j = 0;
        while(tok != NULL){
            customers[i].maximum[j]=atoi(tok);
            j++;
            char* tok = strtok(inputFile, ",");
        }
    }


}

void userInput(char* line){
    int len = strlen(line);

    if len<1{
        if (line=="*"){
            outputValues();
        }
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