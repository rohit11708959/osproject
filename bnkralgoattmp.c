#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMBER_OF_RESOURCES 3
#define NUMBER_OF_CUSTOMERS 5
int i = 0;
int j = 0;
pthread_mutex_t mutex; //mutex lock to lock global variables
int initResourceVector [NUMBER_OF_RESOURCES];

//available, max, allocation, need
int available [NUMBER_OF_RESOURCES];
int allocation [NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {{1,1,0},{1,3,0},{0,0,2},{0,1,1},{0,2,0}};
int max [NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {{5,5,5},{3,3,6},{3,5,3},{7,1,4},{7,2,2}};
int need [NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int requestResource(int processID,int request[]);
int releaseResource(int processID,int releaseVector[]);
int ifGreaterThanNeed(int processID,int request[]);
int ifEnoughToRelease(int processID,int releaseVector[]);
int ifInSafeMode();
int ifEnoughToAlloc();
void printNeed();
void printAllocation();
void printAvailable();
void printReqOrRelVector(int vec[]);
void *customer(void* customerID);

void *customer(void* customerID)
{
            int processID = *(int*)customerID;
int c=0;
            while(c<3)
            {
                        //generate random requests
                        sleep(1);
                        int request[NUMBER_OF_RESOURCES];

                       
                        pthread_mutex_lock(&mutex);

                        //initialize need vector
                        for(i = 0; i < NUMBER_OF_RESOURCES; i++)
                        {
                                    if(need[processID][i] != 0)
                                    {
                                     request[i] = rand() % need[processID][i];
                                    }
                                    else
                                    {
                                                request[i] = 0;
                                    }
                        }


printf("Process %d is requiring resources:\n",processID);
                        printReqOrRelVector(request);

                        requestResource(processID,request);

pthread_mutex_unlock(&mutex);
           
//generate random release vector            
sleep(1);
int releaseVector[NUMBER_OF_RESOURCES];
                       
pthread_mutex_lock(&mutex);

//inicialize release vector
for(i = 0; i < NUMBER_OF_RESOURCES; i++)
{
if(allocation[processID][i] != 0)
            {
                        releaseVector[i] = rand() % allocation[processID][i];
            }
            else
            {
                        releaseVector[i] = 0;
            }
}


printf("Process %d is releasing resources:\n",processID);
                        printReqOrRelVector(releaseVector);
                       
releaseResource(processID,releaseVector);
//unlock
pthread_mutex_unlock(&mutex);

c++;
            }
}

int requestResource(int processID,int request[])
{
            if (ifGreaterThanNeed(processID,request) == -1)
            {
                        printf("Number of requested resources is more than needed.\n");
                        return -1;
            }
            printf("Resources are being allocated...\n");

            if(ifEnoughToAlloc(request) == -1)
            {
                        printf("Not enough resources.\n");
                        return -1;
            }

        
            for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
            {
                        need[processID][i] -= request[i];
                        allocation[processID][i] += request[i];
                        available[i] -= request[i];
            }
            printf("Checking is the state is safe...\n");
           
            if (ifInSafeMode() == 0)
            {
                        printf("Safe Mode. Resources Allocated \n Matrica Available \n");
                        printAvailable();
                        printf("Matrica Allocated:\n");
                        printAllocation();
                        printf("Matrica Need:\n");
                        printNeed();
                        return 0;
            }
            else
            {           
                        printf("State is not safe.\n");
                        for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
                        {
                                    need[processID][i] += request[i];
                                    allocation[processID][i] -= request[i];
                                    available[i] += request[i];
                        }
                   
                        return -1;
            }
}

int releaseResource(int processID,int releaseVector[])
{          

            if(ifEnoughToRelease(processID,releaseVector) == -1)
            {
                        printf("Not enought resources.\n");
                        return -1;
            }
            for(i = 0; i < NUMBER_OF_RESOURCES; i++)
            {
                        allocation[processID][i] -= releaseVector[i];
                        need[processID][i] += releaseVector[i];
                        available[i] += releaseVector[i];
            }
            printf("Released.\nMatrica Available:\n");
            printAvailable();
            printf("Matrica Allocated:\n");
            printAllocation();
            printf("Matrica Need:\n");
            printNeed();
            return 0;
}


int ifEnoughToRelease(int processID,int releaseVector[])
{          
            for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
            {
                        if (releaseVector[i] <= allocation[processID][i])
                                    { continue; }
                        else
                                    { return -1;}
            }
            return 0;
}

int ifGreaterThanNeed(int processID,int request[])
{

            for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
            {
                        if (request[i] <= need[processID][i])
                                    {  continue; }
                        else
                        { return -1; }
            }
            return 0;
}

int ifEnoughToAlloc(int request[])
{

            for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
            {
                        if (request[i] <= available[i])
                                    { continue; }
                        else
                        { return -1; }
            }
            return 0;
}

void printNeed()
{
            for (i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
            {
                        printf("{ ");
                        for (j = 0; j < NUMBER_OF_RESOURCES; ++j)
                        {
                                    printf("%d, ", need[i][j]);
                        }
                        printf("}\n");
            }
            return;
}

void printAllocation()
{
            for (i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
            {
                        printf("{ ");
                        for (j = 0; j < NUMBER_OF_RESOURCES; ++j)
                        {
                                    printf("%d, ", allocation[i][j]);
                        }
                        printf("}\n");
            }
            return;
}

void printAvailable()
{
            for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
            {
                        printf("%d, ",available[i]);
            }
            printf("\n");
            return;
}


void printReqOrRelVector(int vec[])
{
            for (i = 0; i < NUMBER_OF_RESOURCES; ++i)
            {
                        printf("%d, ",vec[i]);
            }
            printf("\n");
            return;
}


int ifInSafeMode()
{
            int finish[NUMBER_OF_CUSTOMERS] = {0};

            //inicialize Work=Available
            int work[NUMBER_OF_RESOURCES];

            for(i = 0; i < NUMBER_OF_RESOURCES; i++)
            {
                       work[i] = available[i];
            }

            int k;
            for(i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                        if (finish[i] == 0)
                        {
                                    for(j = 0; j < NUMBER_OF_RESOURCES; j++)
                                    {
                                                if(need[i][j] <= work[j])
                                                {
                                                            if(j == NUMBER_OF_RESOURCES - 1)                                                                                                            {
                                                                        finish[i] = 1;
                                                            for (k = 0; k < NUMBER_OF_RESOURCES; ++k)
                                                                        {
                                                                                    work[k] += allocation[i][k];
                                                                                 
                                                                        }
                                                                       
                                   
i = -1;
break;
            }
else
                        { continue; }
}
else
                        { break; }
}
}
else
{  continue; }
}
           
            for(i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                        if (finish[i] == 0)
                        {
                                    
                                    return -1;
                        }
                        else
                        { continue; }
            }
            
            return 0;

}

int main(int argc, char const *argv[])
{
            if(argc != NUMBER_OF_RESOURCES + 1)
            {
                        printf("Number of parameters is not correct.\n");
                        return -1;
            }
            for(i = 0; i < NUMBER_OF_RESOURCES; i++)
            {
                        //argv[0] is the name of the program
                        initResourceVector[i] = atoi(argv[i+1]);
                        available[i] = initResourceVector[i];
            }

            //initializing need matrix
            for (i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
            {
                        for (j = 0; j < NUMBER_OF_RESOURCES; ++j)
                        {
                                    need[i][j] = max[i][j] - allocation[i][j];
                        }
            }

            printf("Available Vector is:\n");
            printAvailable();

            printf("Allocation Matrix is:\n");
            printAllocation();

            printf("Need Matrix is:\n");
            printNeed();

            pthread_mutex_init(&mutex,NULL);
            pthread_attr_t attrDefault;
            pthread_attr_init(&attrDefault);
            pthread_t *tid = malloc(sizeof(pthread_t) * NUMBER_OF_CUSTOMERS);

            int *pid = malloc(sizeof(int) * NUMBER_OF_CUSTOMERS); // process ID 

            //initialize pid and create pthreads
            for(i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                        *(pid + i) = i;
                        pthread_create((tid+i), &attrDefault, customer, (pid+i));
            }

            //join threads
            for(i = 0; i < NUMBER_OF_CUSTOMERS; i++)
            {
                        pthread_join(*(tid+i),NULL);
            }
            return 0;
}
