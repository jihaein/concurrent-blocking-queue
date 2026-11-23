/* queue.c */
#include "queue.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct queue *make_queue() {
    // TODO: Allocate memory and initialize synchronization mechanisms
    
    // allocate a new queue 
    struct queue *q = (struct queue*)malloc(sizeof(*q));
    if(q == NULL) return NULL;

    // initializing the lock
    // if initializing fails, free & return NULL
   if(pthread_mutex_init(&q->lock, NULL)!=0){
    free(q);
    return NULL;
   };

   // initializing sempahore
   // if failed, free q  and return NULL
    if(sem_init(&q->sem, 0, 0) != 0){
        fprintf(stderr, "Error initializing the semaphore.\n");
        pthread_mutex_destroy(&q->lock);
        free(q);
        return NULL;
    }

    // set the q to be an empty queue.
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

void enqueue(struct queue *q, void *item) {
    // TODO: Add item to the queue and update necessary pointers

    // create a queue node
    queue_node_t *qn = malloc(sizeof(*qn));
    if(qn == NULL) return;
    qn->item = item;
    qn->next =  NULL;
    qn->prev = NULL;

    // entering the critical section 
    pthread_mutex_lock(&q->lock);
    // adding the node when the queue was empty
    if (q->head == NULL && q->tail == NULL){
        q->head = qn;
        q->tail = qn;
        q->size++;
    } else { // if the node wasn't empty, add the new node at the end of the queue
        qn->prev = q->tail; 
        q->tail->next = qn;
        q->tail = qn;
        q->size ++;
    }

    // unlocking the mutex
    pthread_mutex_unlock(&q->lock);
    // signal semaphore 
    sem_post(&q->sem); 
    
}

void *dequeue(struct queue *q) {
    // TODO: Retrieve an item while ensuring thread safety
    
    // if queue is empty, nothing to dequeue
    if (q==NULL) return NULL;

    // wait until queue is not empty
    sem_wait(&q->sem);

    // entering the critical section
    pthread_mutex_lock(&q->lock);

    // dn is the first node (head) of the queue
    queue_node_t *dn = q->head; 
    q->head = dn->next;

    if(q->head == NULL){ // after dequeuing, if it is an empty queue, set tail to NULL
        q->tail = NULL;
    } else {
        q->head->prev = NULL; // if not empty, head shouldnt have a node in front
    }
    q->size--;

    pthread_mutex_unlock(&q->lock);

    void *item = dn-> item;
    free(dn);
    
    return item;
}

void destroy_queue(struct queue *q) {
    // TODO: Clean up allocated resources and synchronization mechanisms
    if (!q) return;

    // lock the queue
    pthread_mutex_lock(&q->lock);

    // free nodes in the queue
    queue_node_t *qn = q->head;
    while(qn!= NULL){
        queue_node_t *next = qn->next;
        free(qn);
        qn = next;
    }

    // set queue to empty queue
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    // unlock 
    pthread_mutex_unlock(&q->lock);

    sem_destroy(&q->sem);
    pthread_mutex_destroy(&q->lock); 
    free(q);
}
