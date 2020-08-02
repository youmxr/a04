/*
Umar Ali (150200830) & Ahmad Salaymeh (170430260)

LINK: github.com/youmxr/a04

CP386 Assignment 4
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>


void outputValues();
void userInput(char* line);
void reqRes(char* line);
void relRes(char* line);
void RunProgram();
int SafeSequence();
void *RunThread(void *thread);

typedef struct customer{
    int* allocate;
    int* maximum;
    int* need;
}Customer;

Customer* customers;
int num;
int custNum;
int* allocated;
int* available;
int* needed;
int* safeSequence;

//typedef enum{
//    false,true
//}
//boolean;


int main (int argc, char *argv[]){
    if(argc<2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}
    else if(argc>2){
        char* command;
        num = argc - 1;
        custNum=0;

        printf("\nNumber of Arguments passed: %d", argc);
        printf("\n-----following-----");
        for(int i=1; i<argc; i++){
            int k = atoi(argv[i]);
            available[i]=k;
            printf("%d", available[i]);
        }
        printf("\n");

        for (int k=0; k<num;k++){
            allocated[k]=0;
        }

        FILE* fp = fopen("sample4_in.txt", "r");
        char inputFile[100];

        if (fp==NULL){
            printf("could not find/open file.");
        }

        for(int i = 0; fgets(inputFile,sizeof(inputFile),fp); i++){
            custNum++;
            char* tok = strtok(inputFile, ",");
            int j = 0;
            while(tok != NULL){
                customers[i].maximum[j]=atoi(tok);
                customers[i].need[j]=atoi(tok);
                needed[j]=needed[j]+customers[i].need[j];
                customers[i].allocate[j]=0;
                j++;
                tok = strtok(inputFile, ",");
            }
        }

        while(1){

        printf("\nEnter Command: ");
        command=malloc(100);
        fflush(stdin);
        scanf("%s", command);

        userInput(command);
        }
    }
    return 0;
}
void userInput(char* line){
    int len = strlen(line);

    if (len == 1){
        if (strcmp(line,"*")){
            outputValues();
        }

        else{
            printf("\nINVALID INPUT...\n");
        }
    }

    else if(len == 3){
        if (strcmp(line, "Run")){
            RunProgram();
        }
        else{
            printf("\nINVALID INPUT...\n");
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

        else{
            printf("\nINVALID INPUT...\n");
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
        printf("%d", needed[i]);
    }

    printf("Available: ");

    for (int i=0;i<num;i++){
        printf("%d", available[i]);
    }

}
void reqRes(char* line){
    char* tok = strtok(line," ");
    int check = 1;
    int cust = atoi(tok);
    int resources[num];
    printf("\n Request for customer #%d\n", cust);

    for (int i=0; i < num; i++){
        tok = strtok(line," ");
        resources[i]= atoi(tok);
    }

    for (int i=0; i<num; i++){
        if(resources[i]>customers[cust].maximum[i]){
            check = 0;
        }
    }

    if (check==0){
        printf("\nRequested is larger than maximum allowed for this customer.\n");
    }

    else{
        for (int i=0; i<num; i++){
            if(resources[i]>available[i]){
                check = 0;
            }
        }
        if (check == 0){
            printf("\n NOT ENOUGH resources available.\n");
        }

        else{
            for (int i=0; i<num; i++){
                customers[cust].allocate[i] = customers[cust].allocate[i] + resources[i];
                customers[cust].need[i] = customers[cust].need[i] - resources[i];
                allocated[i]=allocated[i]+resources[i];
                available[i]=available[i]-resources[i];
            }

            printf("\nResource Request Granted.\n");
        }
    }

}
void relRes(char* line){
    char* tok = strtok(line," ");
    int check = 1;
    int cust = atoi(tok);
    int resources[num];

    printf("\n Release for customer #%d\n", cust);

    for (int i=0; i < num; i++){
        tok = strtok(line," ");
        resources[i]= atoi(tok);
    }

    for (int i=0; i<num; i++){
        if(resources[i]>customers[cust].allocate[i]){
            check = 0;
        }
    }

    if (check==0){
        printf("\nRelease is larger than allocated resources for this customer.\n");
    }

    else{
        for (int i=0; i<num; i++){
                customers[cust].allocate[i] = customers[cust].allocate[i]-resources[i];
                customers[cust].need[i] = customers[cust].need[i] + resources[i];
                allocated[i] = allocated[i]-resources[i];
                available[i] = available[i]+resources[i];
        }

        printf("\nResource Release Completed.\n");
    }
}

void RunProgram(){
    int safe;
    safe=SafeSequence();

    if(safe==0){
        printf("Release resources before continuing...");
    }

    else{
        for (int p=0; p<custNum; p++){
            int runP=safeSequence[p];

            pthread_t threadID;
			pthread_attr_t newThread;
			pthread_attr_init(&newThread);
			pthread_create(&threadID, &newThread, RunThread, (void *)&runP);

			pthread_join(threadID, NULL);
        }
    }
    return;
}

void *RunThread(void *thread){
    int *tid = (int*)thread;
    int h=0;

	printf("\n Customer/Thread %d\n", *tid);

    printf("Allocated Resources: ");
    for (int h=0; h<num; h++){
        printf("%d", customers[*tid].allocate[h]);
    }

    printf("\nAvailable Resources: ");
    for (h=0; h<num; h++){
        printf("%d", available[h]);
    }

    printf("\nThread has started.\n");
    printf("\nThread has finished.\n");
    printf("\nThread is releasing.\n");
    printf("NEW Available: ");

    for (h=0; h<num; h++){
        available[h]+=customers[*tid].allocate[h];
        printf("%d", available[h]);
    }

    for (h=0; h<num; h++){
        customers[*tid].allocate[h] = 0;
        customers[*tid].need[h] = customers[*tid].maximum[h];
    }
    exit(0);
}

int SafeSequence(){
    int check = 0;
    int safe = 0;
    int k, b;
    int count = custNum;
    int finish[custNum];
    for(k=0;k<custNum;k++){
        finish[k]=0;
    }
    int avail_copy[num];
    for (k=0;k<num;k++){
        avail_copy[k]=available[k];
    }
    Customer* state_copy=(Customer*) malloc (sizeof(Customer)*num);

    for(k=0;k<custNum;k++){
        for(b=0; b<num;b++){
            state_copy[k].need[b] = customers[k].need[b];
            state_copy[k].allocate[b] = customers[k].need[b];
        }
    }

    while(count>0){
        safe = 0;

        for (k=0; k<custNum; k++){
            if (finish[k] == 0){
                check = 1;
                for (b=0; b<num; b++){
                    if(state_copy[k].need[b] > avail_copy[b]){
                        check=0;
                        break;
                    }
                }
            }
        }
        if (check==1){
            finish[k] = 1;
            safeSequence[custNum-count] = k;
            count--;
            safe = 1;
        }
        for(b=0; b<num; b++){
            avail_copy[b]+= state_copy[k].allocate[b];
        }
    }

    if(safe==0){
        printf("\nNOT SAFE TO RUN...\n");
    }
    return safe;
}