#include "blg312e.h"
#include "request.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//
#define MAX_BUFFERS 1024

int buffer_size;  // size of the buffer
int buffer[MAX_BUFFERS];  // buffer for connections
int buffer_in = 0;  // index for the next connection to be inserted
int buffer_out = 0; // index for the next connection to be taken

sem_t mutex;  // binary semaphore for locking
sem_t full;  // counting semaphore for full
sem_t empty;  // counting semaphore for empty

// connection_t: structure to hold connection file descriptor and client address
typedef struct {
    int connfd; // connection file descriptor
    struct sockaddr_in clientaddr;  // client address
} connection_t; 

// worker_thread_t: structure to hold worker thread and its id
typedef struct {
    pthread_t thread; // worker thread
    int id; // worker thread id
} worker_thread_t;

worker_thread_t *worker_threads;  // array of worker threads

// blg312e: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
    if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(1);
    }
    *port = atoi(argv[1]);
}

// function to initialize the thread pool
void initialize_thread_pool(int num_threads) {
    worker_threads = malloc(sizeof(worker_thread_t) * num_threads); // allocate memory for worker threads
    sem_init(&mutex, 0, 1); // initialize mutex semaphore
    sem_init(&full, 0, 0);  // initialize full semaphore
    sem_init(&empty, 0, buffer_size); // initialize empty semaphore

    for (int i = 0; i < num_threads; i++) {
        worker_threads[i].id = i; // set worker thread id
        pthread_create(&worker_threads[i].thread, NULL, worker, &worker_threads[i]);  // create worker thread
    }
}

// function to destroy the thread pool
void destroy_thread_pool(int num_threads) {
    for (int i = 0; i < num_threads; i++) {
        pthread_join(worker_threads[i].thread, NULL); // join worker threads
    }
    sem_destroy(&mutex);  // destroy mutex semaphore
    sem_destroy(&full); // destroy full semaphore
    sem_destroy(&empty);  // destroy empty semaphore
    free(worker_threads); // free memory allocated for worker threads
}

// worker: worker thread function
void *worker(void *arg) {
    worker_thread_t *thread = (worker_thread_t *)arg; // get worker thread
    while (1) { // infinite loop
        sem_wait(&full);  // wait for full semaphore
        sem_wait(&mutex); // wait for mutex semaphore
        int connfd = buffer[buffer_out];  // get connection file descriptor
        buffer_out = (buffer_out + 1) % buffer_size;  // update buffer_out
        sem_post(&mutex); // release mutex semaphore
        sem_post(&empty); // release empty semaphore

        requestHandle(connfd);  // handle the request
        Close(connfd);  // close the connection
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <port> <threads> <buffers> <schedalg>\n", argv[0]);
        exit(1);
    }

    // Convert command line arguments to their respective types
    int port = atoi(argv[1]); // Port number for the server to listen on
    int num_threads = atoi(argv[2]); // Number of threads in the thread pool
    buffer_size = atoi(argv[3]); // Size of the buffer
    char *schedalg = argv[4]; // Scheduling algorithm to use (not used in this basic implementation)

    // Check if the buffer size is within the allowed limit
    if (buffer_size > MAX_BUFFERS) {
        fprintf(stderr, "Buffer size too large. Maximum is %d\n", MAX_BUFFERS);
        exit(1);
    }

    // Open a listening socket on the specified port
    int listenfd = Open_listenfd(port);

    // Initialize the thread pool with the specified number of threads
    initialize_thread_pool(num_threads);

    // Main server loop
    while (1) {
        struct sockaddr_in clientaddr; // Client address structure
        socklen_t clientlen = sizeof(clientaddr); // Size of the client address structure
        // Accept a new connection from a client
        int connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        // Use semaphores to ensure thread-safe access to the buffer
        sem_wait(&empty); // Wait until the buffer is not empty
        sem_wait(&mutex); // Wait until the buffer is not being used by another thread
        buffer[buffer_in] = connfd; // Add the new connection to the buffer
        buffer_in = (buffer_in + 1) % buffer_size; // Increment the buffer index, wrapping around if necessary
        sem_post(&mutex); // Signal that the buffer is no longer in use
        sem_post(&full); // Signal that the buffer is not full
    }

    // Clean up the thread pool
    destroy_thread_pool(num_threads);

    return 0;
}