#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "InstanceHost.h"

struct host {
    pthread_t* th;
};

pthread_mutex_t list_lock;
pthread_t th[100];
int instance_id = 0;

void* runner_computation(void* nodeHead);

host* host_create() {
    host* host = (struct host*)malloc(sizeof(struct host));
    return host;
}

void host_destroy(host** host) {
    free(*host);
    *host = NULL;
}

void host_request_instance(host* h, struct job_node* batch) {
    printf("LoadBalancer: Received batch and spinning up new instance.\n");

    if (pthread_create(&th[instance_id], NULL, &runner_computation, (void*)batch) != 0)
        perror("Failed to create thread");
    if (pthread_join(th[instance_id], NULL) != 0)
        perror("Failed to join thread");

    instance_id++;
}

void* runner_computation(void* nodeHead) {
    pthread_mutex_init(&list_lock, NULL);
    pthread_mutex_lock(&list_lock);

    while(nodeHead != NULL) {
        *((struct job_node*)nodeHead)->data_result = ((struct job_node*)nodeHead)->data * ((struct job_node*)nodeHead)->data;
        nodeHead = ((struct job_node*)nodeHead)->next;
    }

    pthread_mutex_unlock(&list_lock);
    pthread_mutex_destroy(&list_lock);          

    pthread_exit(0);
}

