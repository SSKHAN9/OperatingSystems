#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "LoadBalancer.h"

struct balancer{
    struct host *host;
    struct job_node *head;
    int size;
    int requests;
    pthread_mutex_t *lock;
};

balancer* balancer_create(int batch_size){
    balancer *lb = (balancer*)malloc(sizeof(struct balancer));
    lb->head = (struct job_node*)malloc(sizeof(struct job_node) * batch_size);
    lb->head = NULL;
    pthread_mutex_init(&lb->lock, NULL);
    lb->host = host_create();
    lb->size = batch_size;
    lb->requests = 0;
    return lb;
}

void balancer_destroy(balancer** lb){
    struct balancer* l = *lb;
    pthread_mutex_lock(&l->lock);
    if(l->head != NULL){
        host_request_instance(l->host, l->head);
        l->requests = 0;
        l->head = NULL;
    }
    pthread_mutex_unlock(&l->lock);
    host_destroy(&l->host);
    free(l->head);
    l->head = NULL;
    free(*lb);
    *lb = NULL;
}

void balancer_add_job(balancer* lb, int user_id, int data, int* data_return) {
    printf("LoadBalancer: Received new job from user #%d to process data = %d"
           " and store it at %p.\n", user_id, data, data_return);

    pthread_mutex_init(&lb->lock, NULL);
    pthread_mutex_lock(&lb->lock);

    struct job_node* jobNode = (struct job_node*)malloc(sizeof(struct job_node));
    jobNode->user_id = user_id;
    jobNode->data = data;
    jobNode->data_result = data_return;

    jobNode->next = lb->head;
    lb->head = jobNode;

    lb->requests++;

    if (lb->requests == lb->size) {

        struct job_node* tempNode;
        tempNode = lb->head;
        int i = lb->size;
        while (i != 0) {
            lb->head = lb->head->next;
            i--;
        }
        lb->requests = lb->requests - lb->size;
        host_request_instance(lb->host, tempNode);
    }

    pthread_mutex_unlock(&lb->lock);
    pthread_mutex_destroy(&lb->lock);
}