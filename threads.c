#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#define INPUT_SIZE 20

//functions
void *startProducer(void *arg);
void *startConsumer(void *arg);

//global variables
int a = 0;
int b = 0;
char input[INPUT_SIZE];




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
}



    //producer method to be run by the thread
    void *startProducer(void *arg) {
        
        //get the thread id
        int t_id = (int)(long)arg;
        
        printf("Producer %d launched... \n",t_id);
       
       
       
        //add 100 times
        int reps = 0;
        while(reps<100) {
            
            //add 1 to A & 3 to B
            a++;
            b+=3;
            
            //sleep for a random amount of time (btwn 0-100 micro-seconds)
            
            reps++;
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
            
            //add 3 to B and 1 to A
            b+=3;
            a++;
            
            //sleep for a random amount of time (btwn 0-100 micro-seconds)
            reps++;
        }
        
        
    }



//5.16 figure is for pseudocode









