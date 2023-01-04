#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIRS 6
#define STYLISTS 1
#define CUSTOMERS 75
#define DELAY 1000000

void stylistF(void);
void customerF(void);
extern void mon_initialize();
extern void mon_debugPrint();
extern void mon_checkCustomer();
extern int mon_checkStylist();

void main(void){

    mon_initialize();

    pthread_t stylistsThreads[STYLISTS];
    pthread_create(&stylistsThreads[0], NULL,(void*)stylistF, NULL);

    pthread_t customersThreads[CUSTOMERS];
    for(int j = 0; j < CUSTOMERS; j++) {
        pthread_create(&customersThreads[j], NULL,(void*)customerF, NULL);
    }
    
    for(int j = 0; j < CUSTOMERS; j++) {
        pthread_join(customersThreads[j], NULL);
    }
    pthread_join(stylistsThreads[0], NULL);
}

void stylistF(void){

    int j;
    while(1){
        mon_debugPrint();
        mon_checkCustomer();
        for( int i = 0; i < DELAY; i++);
    }
}

void customerF(void){
    int j;
    while(1){
        mon_debugPrint();
        if(mon_checkStylist()){
            break;
        }
        for( int i = 0; i < DELAY; i++);
    }
}
