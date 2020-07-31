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
    int* allocate;
    int* maximum;
}Customer;

Customer* customers;
int num;
int* allocated;
int* available;
int* need;

int main (int argc, char *argv[]){
    if(argc<2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}
    if(argc>2){
        num = argc - 1;

        printf("\nNumber of Arguments passed: %d", argc);
        printf("\n-----following-----");
        for(int i=1; i<argc; i++){
            int k = atoi(argv[i]);
            available[i]=k;
            printf("%d", available[i]);
        }
        printf("\n");

        FILE* fp = fopen("sample4_in.txt", "r");
        char inputFile[100];

        if (fp==NULL){
            printf("could not find/open file.");
        }

        for(int i = 0; fgets(inputFile,sizeof(inputFile),fp); i++){
            char* tok = strtok(inputFile, ",");
            int j = 0;
            while(tok != NULL){
                customers[i].maximum[j]=atoi(tok);
                j++;
                tok = strtok(inputFile, ",");
            }

        }
    }

}
void userInput(char* line){
    int len = strlen(line);

    if (len == 1){
        if (strcmp(line,"*")){
            outputValues();
        }
    }

    else if(len ==3){
        if (strcmp(line, "Run"){
            SafeSequence();
        }
    }

    else{
        char* token = strtok(line, " ");

        if (strcmp(token,"RQ")){
            reqRes(line);
        }
        else if (strcmp(token,"RL")){
            relRes(line);
        }
    }
}
void outputValues(){
    printf("Allocated Resouces: ");

    for (int i=0;i<num;i++){
        printf(" %d", allocated[i]);
    }

    printf("Needed: ");

    for (int i=0;i<num;i++){
        printf("%d", need[i]);
    }

    printf("Available: ");

    for (int i=0;i<num;i++){
        printf("%d", available[i]);
    }

}
void reqRes(char* line){
    char* tok = strtok(line," ");
    bool check = TRUE;
    int cust = atoi(tok);
    int* resources;

    for (int i=0; i < num; i++){
        tok = strtok(line," ");
        resources[i]= atoi(tok);
    }

    for (i=0; i<num; i++){
        if(resources[i]>customers[cust].maximum[i]){
            check = FALSE;
        }
    }

    if (check==FALSE){
        printf("\nRequested is larger than maximum allowed for this customer.\n");
    }

    else{
        for (i=0; i<num; i++){
            if(resources[i]>available[i]){
                check = FALSE;
            }
        }
        if (check == FALSE){
            printf("\n NOT ENOUGH resources available.\n");
        }

        else{
            for (i=0; i<num; i++){
                customers[cust].allocate[i] = customers[cust].allocate[i] + resources[i];
        }

            for (i=0; i<num; i++){
                allocated[i]=allocated[i]+resources[i];
            }

            for (i=0; i<num; i++){
                available[i]=available[i]-resources[i];
            }
        }
    }

}
void relRes(char* line){
    char* tok = strtok(line," ");
    bool check = TRUE;
    int cust = atoi(tok);
    int* resources;

    for (int i=0; i < num; i++){
        tok = strtok(line," ");
        resources[i]= atoi(tok);
    }

    for (i=0; i<num; i++){
        if(resources[i]>customers[cust].allocate[i]){
            check = FALSE;
        }
    }

    if (check==FALSE){
        printf("\nRelease is larger than allocated resources for this customer.\n");
    }

    else{
        for (i=0; i<num; i++){
                customers[cust].allocate[i] = customers[cust].allocate[i]-resources[i];
        }

        for (i=0; i<num; i++){
                allocated[i] = allocated[i]-resources[i];
        }

        for (i=0; i<num; i++){
                available[i] = available[i]+resources[i];
        }

    }
}

void SafeSequence(){

}