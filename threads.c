#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void *producer( void *arg);
void *consumer(void *arg);

//main function
int main(int argc, char **argv) {
    
    //array of thread structs to act as headers
    pthread_t *threads;
    int numThreads = 4;

    //start
    printf("Launching 4 threads... \n");
    
    //create structs and put on the heap
    threads = (pthread_t*) calloc(numThreads,sizeof(pthread_t));
    
    
    //go through and launch all the threads
    for(int i=0; i<numThreads; i++) {
        
        long thread_id = (long)i;
        
        //make sure to launch the same number of producers and consumers
        if(i%2 == 0) { //producer
            
            //launch the thread
            if(pthread_create(&threads[i],NULL,&producer,(void *)thread_id) != 0) {
                perror("Error");
            }
            
        } else { //consumer
        
            //launch the thread
            if(pthread_create(&threads[i],NULL,&consumer,(void *)thread_id) != 0) {
                perror("Error");
            }
        }
        
    }
    
}



    //producer method to be run by the thread
    void *producer( void *arg) {
        
        //get the thread id
        int t_id = (int)(long)arg;
        
        printf("Producer %d launched... \n",t_id);
    }
    
    
    
    //producer method to be run by the thread
    void *consumer( void *arg) {
        
        //get the thread id
        int t_id = (int)(long)arg;
        
        printf("Consumer %d launched... \n",t_id);
    }



//5.16 figure is for pseudocode









