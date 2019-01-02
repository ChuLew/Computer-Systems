#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef enum { false, true } bool; 

struct newThread{
	int num;
	bool * TFarray;
};

int digRev(int num) {  //reverse the digits
    int reverse = 0;
    while(num > 0)
    {
        reverse = reverse*10 + num%10;
        num = num/10;
    }
    return reverse;
}
void* primeReverse(void * data) { //check if reversed number is prime and edit array

	printf("\nPrime numbers, digit reversed less than inputed n:\n");
  int i,j; 
	struct newThread *information = (struct newThread *) data; 

	int num = information->num; 
	int reversedDigits = 0;
	
	for ( i = 2; i <= num; i++) {	
		if (information->TFarray[i] == true) {	
			reversedDigits = digRev(i);
			if (information->TFarray[reversedDigits] == true) 
				printf("%d\n", i);
		}
	}

	pthread_exit(NULL); //wait for thread 
}

void* primeCheck(void * data) {	//check if prime number is prime and edit array

	struct newThread *information = (struct newThread *) data; 

	int num = information->num; 
  int i,j;
  
	for (i = 2; i <= num; i++) { 
		if (information->TFarray[i] == true) {
			for (j = i*i; j <= num; j += i)
				information->TFarray[j] = false;
		}
	}

	pthread_exit(NULL); 
}

int main(int argc, char *argv[]) {
  
  int n,i;
  
  if(argc<2){
      printf("Enter a number N, so we can calculate Primes less than N: ");
      scanf("%d",&n);
	}
 else{
    n = atoi(argv[1]);
 }
 
   pthread_attr_t attr;
   pthread_attr_init(&attr);

	bool *Array = malloc((n + 1) * sizeof(*Array)); // create boolean array
	for (i = 0; i < n+1; i++) {
		Array[i] = true; 	//initialize array to true		
  }
  
	struct newThread cont;
	
  cont.num = n;				
	cont.TFarray = Array; 	

	pthread_t tid;


	pthread_create(&tid, &attr, primeCheck, (void *) &cont); 

	pthread_join(tid, NULL); 			
	printf("Prime numbers that are less than inputed n:\n");

	for ( i = 2; i <= n; i++) {
		if (Array[i] == true) 			
			printf("%d\n", i);
	}


	pthread_create(&tid, &attr, primeReverse, (void *) &cont); 
	pthread_join(tid, NULL); 			

}
