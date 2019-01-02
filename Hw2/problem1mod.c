#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

unsigned long  array[100];
volatile int boolean = 1;

void *fibonacci(void *voidNum){
 
 boolean = 1 ; // 
     int *num = (int*)voidNum;
     int  first = 0, second = 1, next, c;
 
   for ( c = 0 ; c < *num ; c++ )
   {
      if ( c <= 1 )
         next = c;
      else
      {
         next = first + second;
         first = second;
         second = next;
      }
      array[c] = next; 
   }
   boolean = 0; 
    
 
}

int main( int argc, char* argv[]) {


    int num;
    int i;
    printf("Enter the number of terms\n");
    scanf("%d",&num);
 
    printf("First %d terms of Fibonacci series are :-\n",num);
   
    pthread_t tid;
    pthread_create(&tid,NULL,&fibonacci,&num);
   //pthread_join(tid,NULL);
    
    while(boolean == 1){
    //DO NOTHING 
    }
    
    for(i=0;i<num;i++){
        printf("%lu\n",array[i]);
    }
}
