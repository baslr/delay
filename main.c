//
//  main.c
//  delay
//
//  Created by baslr on 21.11.15.
//  Copyright © 2015 baslr. All rights reserved.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef unsigned long u64int;

void *workerThread(void *data);

pthread_t       t_workerThread;
pthread_mutex_t gLock;


typedef struct item {
    u64int at;
    struct item *pNext;
} t_item;



t_item *pHead;
t_item *pLast;


t_item* t_item_new(u64int at) {
    t_item *new = malloc(sizeof(t_item));
    new->pNext = NULL;
    new->at = at;
    return new;
}

u64int t_item_length(t_item *pStart) {
    u64int length = 0;
    t_item *pTmp = pStart;
    while(pTmp != NULL) {
        length++;
        pTmp = pTmp->pNext;
    }
    return length;
}


int main(int argc, const char * argv[]) {
    
    pHead = t_item_new( time(NULL) + 10 );
    pLast = pHead;
    
    
    u64int now = time(NULL);
    for(int i = 0; i < 10; i++) {

        t_item *pItem = t_item_new((now + (i+1) * 60));

        pLast->pNext = pItem;
        pLast = pItem;
    }
    
    printf("list length %lu\n", t_item_length(pHead));
    
    int tErr;
    tErr = pthread_create(&t_workerThread, NULL, workerThread, pHead);
    printf("status worker createion %d\n", tErr);
    
    pthread_mutex_init(&gLock, NULL);

    
    printf("pthread_exit\n");
    
    
    printf("%lu\n", sizeof( u64int   ) );
    pthread_exit(NULL);
}


void *workerThread(void *data)
{
//    int **a;
//    a = data;
//    int *b;
//    b = *a;

    printf("next in:%lu\n", pHead->at);
    u64int curTime;
    u64int curAt;
    t_item *pTmp;
    while(1) {
        sleep(1);
        if(pHead == NULL) continue;
        curTime = time(NULL);
        pthread_mutex_lock(&gLock);
        curAt = pHead->at;
        pthread_mutex_unlock(&gLock);
        if(curTime < pHead->at) {
            continue;
        }
        
        pthread_mutex_lock(&gLock);
        
        pTmp = pHead;
        pHead = pHead->pNext;
        free(pTmp);
        
        // get foo send msg

        printf("exec command for time %lu\n", curAt);
    
        printf("list length %lu\n", t_item_length(pHead));
        
        pthread_mutex_unlock(&gLock);
    }
    

    
    return 0;
}