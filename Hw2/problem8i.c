#include <stdio.h> 
#include <stdbool.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <errno.h>

sem_t officeKeys;

break_time(){// time either worker takes break
    printf("workers left\n");
    sleep(1);
}

working_window(){// this is the time the coders code
    printf("cleaners are cleaning office\n");   
}

clean_office(){//this is time the cleaners clean
    printf("employees are coding in the office\n");
}

void employee(){

    while(true){ 
    /*synchronization schemes*/
        wait(&officeKeys);
        working_window();
    /*synchronization schemes*/
        signal(&officeKeys);       
        break_time();
    }

}

void cleaner(){
    while(true){
    /*synchronization schemes*/
    wait(&officeKeys);
    clean_office();
    /*synchronization schemes*/        
    signal(&officeKeys);
    
    break_time();
    }
}


int main(int argc, char ** argv){
 
    sem_init(&officeKeys,0,1);
     
    pthread_t c_tid,e_tid; 
    pthread_attr_t attr;
    pthread_attr_init(&attr);
 
    pthread_create(&c_tid,&attr,(void*)cleaner,NULL);
    pthread_create(&e_tid,&attr,(void*)employee,NULL);

    pthread_join(c_tid,NULL);
    pthread_join(e_tid,NULL);
}

/* please justify if there is any possibility of starvation in the solution you provided?

The solution to this problem in an unideal situation may cause a starvation/livelock. Here we have a binary semaphore which will only allow a cleaner or worker to "work in the office" at a single point in time. the cleaner/employee gives up the "officeKey" when he/she is done however this does not necesarily mean that the semaphore will relinquish its "officeKey" to the opposing worker. What could possibly happen is the semaphore gives its "officeKey" to the employee, the employee works, and the then relinquishes its key, takes a break, but takes the "officeKey" again before the cleaner can access it in a never ending loop, never allowing access to the office for the cleaner.
*/
