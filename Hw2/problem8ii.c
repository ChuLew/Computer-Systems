#include <stdio.h> 
#include <stdbool.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <errno.h>

int total_keysE;
int total_keysC; 

sem_t cleanerKeys;
sem_t employeeKeys;
sem_t switchWorker;


break_time(){
    sleep(3);
}


working_window(){// this is the time the coders code
    printf("employees are coding in office\n");
    sleep(1);
}

clean_office(){//this is time the cleaners clean
    printf("cleaners are cleaning office\n");
    sleep(1);
}

void employee(){

    int keys;
    
    while(true){ 
    /*synchronization schemes*/
        wait(&switchWorker); // grabs a key if no cleaners
        wait(&employeeKeys); //grabs 1 of n keys
        
        working_window(); //works
        
    /*synchronization schemes*/ 
        signal(&employeeKeys);  //gives up key
         
     while(true){     
       sem_getvalue(&employeeKeys,&keys);  
      
       if(keys==total_keysE){ // all N employees left for break
           signal(&switchWorker); //swtich workers
           break;
        }  
     }   
 
     break_time();
    }

}

void cleaner(){
    int keys;
    
    while(true){
    /*synchronization schemes*/
    wait(&switchWorker); //grabs key if no employees
    wait(&cleanerKeys);//grabs one of the m keys for cleaners
    
    clean_office();
    /*synchronization schemes*/
     signal(&switchWorker);
       
     while(true){     //keeps checking amount of cleanerKeys
        sem_getvalue(&cleanerKeys,&keys);  
       
        if(keys==total_keysC){ // all N cleaners left for break
            signal(&switchWorker); //switch workers
            break;
         }  
      }   
     
    
    break_time();
  
    }
}


int main(int argc, char ** argv){
 
   int e,c,i;
   printf("please enter the amount of employees: \n");
   scanf("%d",&e);
   
   printf("please enter the amount of cleaners: \n");
   scanf("%d",&c);
  
   total_keysE=e;
   total_keysC=c;
   
    sem_init(&employeeKeys,0,e);
    sem_init(&switchWorker,0,1);
    sem_init(&cleanerKeys,0,c);
     
    pthread_t c_tid[c];
    pthread_t e_tid[e]; 
    pthread_attr_t attr;
    pthread_attr_init(&attr);


    for(i=0;i<e;i++){ 
        pthread_create(&c_tid[c],&attr,(void*)cleaner,NULL);
    }
    for(i=0;i<e;i++){
        pthread_create(&e_tid[i],&attr,(void*)employee,NULL);
    }
    for(i=0;i<c;i++){
        pthread_join(e_tid[c],NULL);
    }
    for(i=0;i<e;i++){
        pthread_join(e_tid[i],NULL);
    }
}
