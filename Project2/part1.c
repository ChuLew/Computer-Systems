
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
volatile sig_atomic_t minmax[4];
volatile sig_atomic_t totsum[2];
volatile sig_atomic_t gma = 0;
volatile sig_atomic_t gsum = 0;
volatile sig_atomic_t killpid;
pid_t mainPid;

void print(char* print){
    printf("%s\n",print);
    fflush(stdout);
}
void handler(int signum, siginfo_t *info, void *extra){ 
    void *ptr_val = info->si_value.sival_ptr;
    int int_val = info->si_value.sival_int;
    if(signum==SIGUSR1){
	minmax[gma]=int_val;
        //printf("%d",int_val);
	gma++;
    }
    if(signum==SIGUSR2){
	totsum[gsum]=int_val;
	gsum++;
    }
}

int max(int* array,int count){
    int i,max=array[0],ac = count/2;
    for(i=0;i<ac;i++){
	if(array[i]>max){
            max = array[i];
	}
    }
    return max;
}
int min(int* array,int count){
    int i,min=array[0],ac = count/2;
    for(i=0;i<ac;i++){
        if(array[i]<min){
            min = array[i];
       }
    }
    return min;
}
int sum(int* array,int count){
    int i,sum = 0,ac = count/2;
	for(i=0;i<ac;i++){
		sum+=array[i];
	}
    return sum;
}
int countFile(char *argv[]){
    char buffer[1000];
    char* fileName = argv[1];
    FILE* fpcount;
    fpcount=fopen(fileName,"r");
    int count=0;
    while(NULL!=fgets(buffer,1000,fpcount)){
        bzero(buffer, 1000);
        count++;
    }
	return count;
}
int* getArray(char* argv[]){
    char buffer[1000];
    int tmp;	
    char* fileName = argv[1];
    int count = countFile(argv);   
    int *array = (int*)malloc(sizeof(int)*count);
    int ac=0;
    FILE* fp;
    fp = fopen(fileName,"r");
    while(fgets(buffer, 1000, fp)!= NULL){
        tmp = atoi(buffer);
        array[ac]=tmp;
        ac++;
    }
    return array;
}
int main(int argc, char *argv[]){
    if (argc!=2){
        printf("Did not input file into command line program terminated\n");
        exit(EXIT_FAILURE);
    }
    
    struct sigaction action;  
    sigset_t mask; 
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);
    sigaddset(&mask,SIGUSR2);
    sigaddset(&mask,SIGALRM);
    sigaddset(&mask,SIGWINCH);
    sigaddset(&mask,SIGINT);
    action.sa_flags = SA_SIGINFO; 
    action.sa_mask =mask;
    action.sa_sigaction = &handler;
    
    if (sigaction(SIGUSR1,&action,NULL)==-1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGUSR2,&action,NULL)==-1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGALRM,&action,0)==-1){ //iniatilize sigalarm 
        perror("sigaction");
        exit(EXIT_FAILURE);
    } 
    if(sigaction(SIGWINCH,&action,0)==-1){
        perror("sigaction");
        exit(EXIT_FAILURE); 
    }
    if(sigaction(SIGINT,&action,0)==-1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    int* array = getArray(argv);
    int count = countFile(argv);
    int firstHalf[count/2];
    int secondHalf[count/2];
    int i,j=0;
    for(i=0;i<count/2;i++){ 
        firstHalf[i]=array[i];
    }
    for(i=count/2;i<count;i++){ 
        secondHalf[j]=array[i];
        j++;
    }
    mainPid=getpid();
    printf("I am process %d my parent is process %d\n",getpid(),getppid());
    int status;
    pid_t pid;
    pid=fork();
    if(pid==0){ //child1
        pid = fork();
        if(pid==0){//child1 of child1
            printf("I am process %d my parent is process %d\n",getpid(),getppid());
            int max1 = max(firstHalf,count);
            sigqueue(mainPid,SIGUSR1,(const union sigval) max1);
        }
        else{//child1
            pid = fork();
            if(pid==0){//child 2 of child 1
                printf("I am process %d my parent is process %d\n",getpid(),getppid());
                int min1 = min(firstHalf,count);
		sigqueue(mainPid,SIGUSR1,(const union sigval) min1);
            }
            else{//child 1 waits on children
                printf("I am process %d my parent is process %d\n",getpid(),getppid());
                int sum1 = sum(firstHalf,count);
                sigqueue(mainPid,SIGUSR2,(const union sigval) sum1);
            }
        }
           while ((pid=waitpid(-1,&status,0))!=-1);
    }
    else{ //parent
        while ((pid=waitpid(-1,&status,0))!=-1);
        pid = fork();       
        if (pid==0){//child2
            pid = fork();
            if(pid==0){//child1 of child 2
                printf("I am process %d my parent is process %d\n",getpid(),getppid());
                int max2 = max(secondHalf,count);
		sigqueue(mainPid,SIGUSR1,(const union sigval) max2);
            }
            else{//child2 
                while ((pid=waitpid(-1,&status,0))!=-1);
                pid = fork();
                if(pid==0){//child2 of child 2
                   printf("I am process %d my parent is process %d\n",getpid(),getppid());
                   int min2 = min(secondHalf,count);
                   sigqueue(mainPid,SIGUSR1,(const union sigval) min2);
                }
                else{//child 2 waits on children 
                    while ((pid=waitpid(-1,&status,0))!=-1);
                    printf("I am process %d my parent is process %d\n",getpid(),getppid());
                    int sum2 = sum(secondHalf,count);
                    sigqueue(mainPid,SIGUSR2,(const union sigval) sum2);     
                }
                   while ((pid=waitpid(-1,&status,0))!=-1);
            }
        }
        else{//parent
             while ((pid=waitpid(-1,&status,0))!=-1);
             
        }
           while ((pid=waitpid(-1,&status,0))!=-1);
    }
    if(getpid()==mainPid){
        while ((pid=waitpid(-1,&status,0))!=-1);
        while ((pid=waitpid(-1,&status,0))!=-1);
        //printf("the pid is %d ",getpid());
        //print("all children terminated");
        int t,maximum=minmax[0],minimum=minmax[0],finsum;
        for(t=0;t<4;t++){
            //printf("In min max is: %d\n",minmax[t]);
                if(minmax[t]>maximum){
                        maximum=minmax[t];
                }
        }
        for(t=0;t<4;t++){
                if(minmax[t]<minimum&&minmax[t]!=0){
                        minimum=minmax[t];
                }
        }
        finsum=totsum[0]+totsum[1];

        printf("The maximum value is %d\n",maximum);
        printf("The minimum value is %d\n",minimum);
        printf("The sum value is %d\n",finsum);
    }
 }