#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int array[100];

void *fibonacci(void *voidNum){
 
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
 
}
int main( int argc, char* argv[]) {


    int num;
    int i;    
    printf("Enter the number of terms\n");
    scanf("%d",&num);
 
    printf("First %d terms of Fibonacci series are :-\n",num);
   
    pthread_t tid;
    pthread_create(&tid,NULL,&fibonacci,&num);
    pthread_join(tid,NULL);
    
    for(i=0;i<num;i++){
        printf("%d\n",array[i]);
    }            
}
