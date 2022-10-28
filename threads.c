#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define INPUT_SIZE 20
#define BUFFER_SIZE 3

//functions
void *startProducer(void *arg);
void *startConsumer(void *arg);

//global variables
int a = 0;
int b = 0;
char input[INPUT_SIZE];
sem_t a_lock;
sem_t b_lock;
sem_t buf_sem;
sem_t items_in_buf;
sem_t empty_slots;
int buffer[BUFFER_SIZE];
int index_to_write = 0;
int index_to_take = 0;



//main function
int main(int argc, char **argv) {
    
    
    //Read in a line using fgets, don't forget to trim the new line at the end
    printf("How many threads?  ");
    fgets(input, INPUT_SIZE, stdin); 
    int numThreads = atoi(input);
    
    
    //array of thread structs to act as headers
    pthread_t *threads;

    //start
    printf("Launching %d threads... \n\n",numThreads);
    
    //create structs and put on the heap
    threads = (pthread_t*) calloc(numThreads,sizeof(pthread_t));
    
    //initialize semaphores
    if(sem_init(&a_lock,0,1) < 0) {
        perror("Error initializing a_lock");
    }
    if(sem_init(&b_lock,0,1) < 0) {
        perror("Error initializing b_lock");
    }
    if(sem_init(&buf_sem,0,1) < 0) {
        perror("Error initializing buf_sem");
    }
    if(sem_init(&items_in_buf,0,0) < 0) {
        perror("Error initializing items_in_buf");
    }
    if(sem_init(&empty_slots,0,BUFFER_SIZE) < 0) {
        perror("Error initializing empty_slots");
    }

    
    
    
    
    //go through and launch all the threads
    for(int i=0; i<numThreads; i++) {
        
        long thread_id = (long)i;
        
        //make sure to launch the same number of producers and consumers
        if(i%2 == 0) { //producer
            
            //launch the thread
            if(pthread_create(&threads[i],NULL,&startProducer,(void *)thread_id) != 0) {
                perror("Error");
            }
            
        } else { //consumer
        
            //launch the thread
            if(pthread_create(&threads[i],NULL,&startConsumer,(void *)thread_id) != 0) {
                perror("Error");
            }
        }
        
    }
    
    
    //make sure that the program doesn't quit until all the threads have done their jobs
    for(int i=0; i<numThreads; i++) {
        pthread_join(threads[i],NULL);
    }
    
    
    //print out the sums
    printf("A: %d  \n",a);
    printf("B: %d  \n",b);
    
    free(threads);
}



    //producer method to be run by the thread
    void *startProducer(void *arg) {
        
        //get the thread id
        int t_id = (int)(long)arg;
        
        printf("Producer %d launched... \n",t_id);
       
       
       
        //add 100 times (PART B)
        int reps = 0;
        while(reps<100) {
            
            //make sure only 1 thread can modify A at a time
            sem_wait(&a_lock);
            
            a++; //add 1 to A 
            
            //unlock A so another thread can modify it
            sem_post(&a_lock);
            
            
            //make sure only 1 thread can modify B at a time
            sem_wait(&b_lock);
            
            //add 3 to B
            b+=3;
            
            //unlock B so another thread can modify it
            sem_post(&b_lock);
            
            //sleep for a random amount of time (btwn 0-100 micro-seconds)
            usleep(rand() % (100 + 1));
            
            reps++;
        }
        
        
        
        
        //PART C - produce 20 random numbers and add them to the buffer
        for(int i=0; i<20; i++) {
            
            //produce a random number 
            int item = rand() % (100 + 1);
            printf("Producer %d: produced %d \n",t_id,item);
            
            //wait until there is an empty slot in the buffer
            sem_wait(&empty_slots);
            
            //wait until we can read/write to the buffer
            sem_wait(&buf_sem);
            
            //produce
            //x = n; 
            buffer[index_to_write] = item;
            index_to_write = (index_to_write + 1)%BUFFER_SIZE; //increment the index
            
            //signal that the buffer is open to be read/modified now
            sem_post(&buf_sem);
            //signal that the number of items in the buffer has increased
            sem_post(&items_in_buf);
            
        }
        
        
    }
    
    
    
    //producer method to be run by the thread
    void *startConsumer(void *arg) {
        
        //get the thread id
        int t_id = (int)(long)arg;
        printf("Consumer %d launched... \n",t_id);
        
        
        
        //add 100 times
        int reps = 0;
        while(reps<100) {
            
            //make sure only 1 thread can modify B at a time
            sem_wait(&b_lock);
            
            
            b+=3; //add 3 to B
            
            //unlock A so another thread can modify it
            sem_post(&b_lock);
            
            
            //make sure only 1 thread can modify A at a time
            sem_wait(&a_lock);
            
            a++; //add 1 to A
            
            //unlock A so another thread can modify it
            sem_post(&a_lock);
            
            
            //sleep for a random amount of time (btwn 0-100 micro-seconds)
            usleep(rand() % (100 + 1));
            
            reps++;
        }
        
        
        
        //PART C - consume the 20 random numbers the producer produces
        for(int i=0; i<20; i++) {
            
            //wait until there are items in the buffer
            sem_wait(&items_in_buf);
            //wait until the buffer is open to read/write
            sem_wait(&buf_sem);
            
            //take the first item
            int item = buffer[index_to_take];
            index_to_take = (index_to_take + 1) % BUFFER_SIZE; //increment the index to take
            
            //signal that the buffer is open to be read/modified now
            sem_post(&buf_sem);
            //signal that the buffer now has an empty slot
            sem_post(&empty_slots);
            
            //consume
            printf("Consumer %d: consumed %d \n",t_id,item);
        }
        
    }



//5.16 figure is for pseudocode









