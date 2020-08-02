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
void reqRes(int line[]);
void relRes(char* line);
void RunProgram();
int SafeSequence();
void *RunThread(void *thread);

typedef struct customer{
    int cID;
    int allocate[4];
    int maximum[4];
    int need[4];
}Customer;

Customer* customers;
int num=0;
int custNum=0;
int allocated[4];
int available[4];
int needed[4];
int safeSequence[5];

sem_t oSem;

//typedef enum{
//    false,true
//}
//boolean;


int main (int argc, char *argv[]){
    sem_init(&oSem, 0, 1);
    Customer* c = (Customer*) malloc(sizeof(Customer)*5);
    customers=c;
    if(argc<2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}
    else if(argc>2){
        char* command;
        num = argc - 1;
        custNum=0;
        printf("\nNumber of Arguments passed: %d", num);
        printf("\n-----following-----");
        for(int i = 1; i<5; i++){
            int k = atoi(argv[i]);
            available[i-1] = k;
            //printf("%d", available[i]);
        }
        printf("\n");

        for (int k=0; k<num;k++){
            allocated[k]=0;
        }

        printf("Reading file...\n");

        FILE* fp = fopen("sample4_in.txt", "r");
        char inputFile[100];

        if (fp==NULL){
            printf("could not find/open file.");
        }
        for(int i = 0; fgets(inputFile,sizeof(inputFile),fp); i++){
            c[i].cID=i;
            
            printf("%s",inputFile);
            custNum++;
            //char* tok = strtok(inputFile, ",");
            //printf("%s\n",tok);
            int j = 0;
            int n=0;
            int m = inputFile[j] - '0';
            while(j<7){
               // printf("\n%d",m);
                c[i].maximum[n] = m;
                c[i].need[n] = m;
                needed[n]+=c[i].need[n];
                c[i].allocate[n]=0;
                j+=2;
                n++;
                //tok = strtok(inputFile, ",");
                //printf("%s",tok);
                //m = atoi(tok);
                m = inputFile[j] - '0';
            }
        }
        printf("\nDONE file reading...");

        while(1){
        
        printf("\nEnter Command: ");
        printf("\n* , Run , RQ, RL, 1\n");

        command=malloc(100);
        fflush(stdin);
        printf(":");
        scanf("%s", command);

        printf("%s",command);

        if (!(strcmp(command,"1")))
        {
            break;
        }
        else{
            userInput(command);
        }
        }
    }
    return 0;
}
void userInput(char* line){
    int len = strlen(line);

    printf("%d",len);

    if (len == 1){
        if (!(strcmp(line,"*"))){
            outputValues();
        }

        else{
            printf("\nINVALID INPUT...\n");
        }
    }

    else if(len == 3){
        if (!(strcmp(line, "Run"))){
            RunProgram();
        }
        else{
            printf("\nINVALID INPUT...\n");
        }
    }

    else{
        char* token = strtok(line, " ");

        if (!(strcmp(token,"RQ"))){
            int array[5];
            token = strtok(NULL," ");
            int j=0;
            while(token!=NULL){
                array[j]=atoi(token);
                printf("\n%d\n",array[j]);
                token = strtok(NULL," ");
            }
            reqRes(array);
        }

        else if (!(strcmp(token,"RL"))){
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
        printf(" %d ", allocated[i]);
    }

    printf("Needed: ");

    for (int i=0;i<num;i++){
        printf(" %d ", needed[i]);
    }

    printf("Available: ");

    for (int i=0;i<num;i++){
        printf(" %d ", available[i]);
    }

}
void reqRes(int line[]){
    int check = 1;
    int cust = line[0];
    int resources[num];
    printf("\nRequest for customer #%d\n", cust);
    sem_wait(&oSem);
    

    for (int i=0; i < num; i++){
        resources[i]= line[i+1];
        printf("\n%d\n",resources[i]);
        
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

    sem_post(&oSem);

    return;
}
void relRes(char* line){
    int check = 1;
    int cust = line[0];
    int resources[num];

    printf("\n Release for customer #%d\n", cust);
    sem_wait(&oSem);

    for (int i=0; i < num; i++){
        resources[i]= line[(i+1)];
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
    sem_post(&oSem);

    return;
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

    sem_wait(&oSem);

	printf("\n Customer/Thread %d\n", customers[*tid].cID);

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
    sem_post(&oSem);
    pthread_exit(0);
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