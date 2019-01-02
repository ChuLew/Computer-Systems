
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
volatile sig_atomic_t flag=0;
volatile sig_atomic_t killpid;
pid_t mainPid;
char* shm;
char buff[100];

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
    if(signum==SIGALRM){ 
	sigqueue(mainPid,SIGWINCH,(const union sigval) getpid());
        //kill(getpid(),SIGKILL);
        sleep(3);
    }
    if(signum==SIGWINCH){
        printf("Process %d took too long. Wont be factored into arithmetic calculations \n",int_val); //do something with sigkill here most likely
	//sigqueue(0,SIGINT,(const union sigval) getpid());
        sprintf(buff,"%d",int_val);
	memcpy(shm,buff,sizeof(buff)); //only goes to main process
	kill(0,SIGINT);
        //kill(int_val,SIGKILL);
    }
    if(signum==SIGINT){
	killpid = atoi(shm);
	printf("process %d has been notified to terminate %d\n",getpid(),killpid);
	kill(killpid,SIGKILL);
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
    
    int shmid;
    key_t key;
    char *s;
    key=9876;
    shmid = shmget(key,sizeof(getpid()),IPC_CREAT|0666);
    shm=shmat(shmid,NULL,0);
    
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
            alarm(3);
            printf("I am process %d my parent is process %d\n",getpid(),getppid());
            int max1 = max(firstHalf,count);
            sigqueue(mainPid,SIGUSR1,(const union sigval) max1);
        }
        else{//child1
            pid = fork();
            if(pid==0){//child 2 of child 1
                alarm(3);
                printf("I am process %d my parent is process %d\n",getpid(),getppid());
                int min1 = min(firstHalf,count);
		sigqueue(mainPid,SIGUSR1,(const union sigval) min1);
            }
            else{//child 1 waits on children
                alarm(3);
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
                alarm(3);  
                printf("I am process %d my parent is process %d\n",getpid(),getppid());
                int max2 = max(secondHalf,count);
		sigqueue(mainPid,SIGUSR1,(const union sigval) max2);
            }
            else{//child2 
                while ((pid=waitpid(-1,&status,0))!=-1);
                pid = fork();
                if(pid==0){//child2 of child 2
                    alarm(3);
                   printf("I am process %d my parent is process %d\n",getpid(),getppid());
                   int min2 = min(secondHalf,count);
                   sigqueue(mainPid,SIGUSR1,(const union sigval) min2);
                }
                else{//child 2 waits on children 
                    alarm(3);
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
        sleep(3);
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