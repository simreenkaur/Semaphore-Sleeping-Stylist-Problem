#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIRS 6
#define CUSTOMERSTOBESERVED 75

typedef struct{
    int blockedThreadsCount;
    sem_t suspendThread;
}cond;

sem_t mutex;
cond stylistAvailable;
cond customerAvailable;

int customer = 0;
int stylist = 0;
int salonFull = 0;
int salonEmpty = 0;
int haircuts = 0;
int customerQueue[CHAIRS];

void mon_initialize(){
    sem_init(&mutex,0,1);

    sem_init(&(customerAvailable.suspendThread), 0, 0);
    customerAvailable.blockedThreadsCount = 0;

    sem_init(&(stylistAvailable.suspendThread), 0, 0);
    stylistAvailable.blockedThreadsCount = 0;
}

int count(cond *condVar) {
    return condVar->blockedThreadsCount;
}

void wait(cond *condVar) {
    condVar->blockedThreadsCount++;
    sem_post(&mutex);
    sem_wait(&(condVar->suspendThread));
    sem_wait(&mutex);
}

void signal(cond *condVar) {
    if(count(condVar) > 0){
        condVar->blockedThreadsCount--;
        sem_post(&(condVar->suspendThread));
    }
}

void mon_debugPrint() {

    sem_wait(&mutex);

    printf("|");
    for(int i = 0; i < customer; i++){
        printf("1|");
    }
    for(int i = 0; i < CHAIRS - customer; i++){
        printf("0|");
    }
    printf("=> %d", customer);

    printf("\n\nGiven Haircuts = %d\n", haircuts);
    printf("Salon full = %d times\n", salonFull);
    printf("Salon empty = %d times\n\n\n", salonEmpty);

    if(haircuts == CUSTOMERSTOBESERVED){
        printf("All customers have been served!");
    }

    sem_post(&mutex);
}

void mon_checkCustomer() {
    
    sem_wait(&mutex);
    stylist++;
    signal(&stylistAvailable);

    if(customer == 0) {
        wait(&customerAvailable);
    }

    customer--;
    haircuts++;

    if(customer == 0){
        salonEmpty++;
    }  
    sem_post(&mutex);
}

int mon_checkStylist(){
    
    int status = 0;
    sem_wait(&mutex);
    
    if(customer < CHAIRS) {
        customer++;
        if(customer == CHAIRS){
            salonFull++;
        }
        signal(&customerAvailable);

    if(stylist == 0) {
        wait(&stylistAvailable);
    }

    stylist--;
    status = 1;
  }

  sem_post(&mutex);
  return status;
}
