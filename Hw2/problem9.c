#include <stdio.h> 
#include <stdbool.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <errno.h>

int ratio; 
sem_t childSem;
sem_t teachSem;
pthread_mutex_t oneTeach;

void verify_compliance(){


    int teachersInRoom;
    int childrenInRoom;
    float testRatio;
    
    sem_getvalue(&teachSem,&teachersInRoom);// gets number of teachers currently in room
    sem_getvalue(&childSem,&childrenInRoom);
    
   if(teachersInRoom!=0){//teachers present
       testRatio =childrenInRoom/(teachersInRoom);
   } 
   else{ //no teachers
       if(childrenInRoom==0){//no children
           //room empty
           printf("Parent %u see an empty room Compliance met\n",(int)pthread_self());
       }
       else{//children in room
          // parents angry
          printf("Parent %u sees Children present with no teachers. Compliance not met\n",(int)pthread_self());
       }
       
   }
   if(testRatio<=ratio){
      //parents happy
      printf("Parent %u sees Ratio met, Compliance met\n",(int)pthread_self());
   }
   else{
       //parents angry
       printf("Parent %u sees Ratio not met,Compliance not met\n",(int)pthread_self());
   }
   
}
void parent_exit(){
    printf("Parent %u has left the class.\n",(int)pthread_self());
}
void parent_enter(){
    printf("Parent %u has come\n",(int)pthread_self());
}

void child_exit(){
    sem_wait(&childSem);
    printf("Child %u has left the class.\n",(int)pthread_self());
}
void learn(){
 //  printf("Child %u is learning.\n",(int)pthread_self());
    sleep(3);
    
}
void child_enter(){
    sem_post(&childSem);
    printf("Child %u is present.\n",(int)pthread_self());
}
void go_home(){
    //printf("Person %u is going home.\n",(int)pthread_self());
    pthread_exit(0);
}

void teacher_enter(){
    sem_post(&teachSem);
    printf("Teacher %u has entered the class.\n",(int)pthread_self());
}

void teach(){
  //  printf("Teacher %u is teaching the class.\n",(int)pthread_self());
    sleep(3); //wait for kids to leave
    
}
void teacher_exit(){

    int teachersInRoom;
    int childrenInRoom;
    float testRatio;

    printf("Teacher %u is trying to exit the room.\n",(int)pthread_self());
    
    sem_getvalue(&teachSem,&teachersInRoom);// gets number of teachers currently in room
    sem_getvalue(&childSem,&childrenInRoom);
    
   if(teachersInRoom-1!=0){
       testRatio =childrenInRoom/(teachersInRoom-1);
   } 
   else{ //1 teacher left
       if(childrenInRoom==0){
           sem_wait(&teachSem); //no children left teacher can leave
           printf("Teacher %u has left class.\n",(int)pthread_self());
           exit(1);
       }
       else{
           printf("The last teacher %u cant leave the room there are children left!\n",(int)pthread_self());
           teach(); //there are still children in room teacher cant leave
           teacher_exit();
       }
   }
   if(testRatio<=ratio){
       sem_wait(&teachSem);
       printf("Teacher %u has exited the classRoom.\n",(int)pthread_self());
   }
   else{
          printf("The teacher %u can't leave yet there are too many children in class.\n",(int)pthread_self());
           teach(); //there are still children in room teacher cant leave
           teacher_exit();
   }
}



void Teacher(){

	for (;;) {

    teacher_enter(); //will increment number of teachers in room in this non protected region
    teach();// give time for children to enter the room
		pthread_mutex_lock(&oneTeach); // letting more than 1 teacher leave will be problematic	
    teacher_exit(); //if multiple teachers tried to leave calculatin ratio will get messed up
    pthread_mutex_unlock(&oneTeach);//unlock lock
		go_home();

	}


}



void Child(){

	for (;;) {

		child_enter();
		learn();
		child_exit();
		go_home();
	}


}



void Parent(){

		for (;;) {

			parent_enter();
		verify_compliance();
			parent_exit();
			go_home();

		}


}



int main(int argc, char ** argv){

   int numChildren,numTeachers,numParents;
   
     if(argc<4){
         printf("Must enter number of children, teachers and parents respectively in command line arguments, ie: ./a.out 3 4 5\n");
         return;
     }
     printf("There should be one teacher in the room per N children, specify N, the ratio: ");
     scanf("%d",&ratio);
   
   numChildren = atoi(argv[1]);
   numTeachers = atoi(argv[2]);
   numParents = atoi(argv[3]);
   
   sem_init(&childSem,0,0);
   sem_init(&teachSem,0,0);
   
   pthread_t c_tid[numChildren];
   pthread_t t_tid[numTeachers];
   pthread_t p_tid[numParents];
   
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   //initializing all the people
  int m;
  
  for(m=0;m<numTeachers;m++){ //teachers must enter first
     pthread_create(&t_tid[m],&attr,(void*)Teacher,NULL);
   }
    
    sleep(1);
    
	for(m=0;m<numChildren;m++){ //children will go second doesnt make sense for parents to check before children arrive
		pthread_create(&c_tid[m],&attr,(void *) Child, NULL);
   //sleep(1);
	}
   
  for(m=0;m<numParents;m++){ //parents last
    pthread_create(&p_tid[m],&attr,(void*)Parent,NULL);
  }
 // reap the threads

  for(m=0;m<numTeachers;m++){
		pthread_join(t_tid[m],NULL);
	}
	for(m=0;m<numChildren;m++){
		pthread_join(c_tid[m], NULL);
	}
	for(m=0;m<numParents;m++){
		pthread_join(p_tid[m], NULL);
	}
 
 sem_destroy(&childSem);
 sem_destroy(&teachSem);
 pthread_mutex_destroy(&oneTeach);
   
}