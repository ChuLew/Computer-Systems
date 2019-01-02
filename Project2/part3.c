#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>

int totmax = 0;
int totmin = 1000000000;
int totsum = 0;
int count;
void* second_gen_max(void* array){
	printf("I am second generation thread max: %lu\n",pthread_self());
	int* arr =(int*)array;
	int temp = max(arr);
	if(temp>totmax){
		totmax = temp;
	}
	pthread_exit();
}
void* second_gen_min(void* array){
	printf("I am second generation thread min: %lu\n",pthread_self());
	int* arr =(int*)array;
	int temp = min(arr);
	if(temp<totmin){
		totmin = temp;
	}
	pthread_exit();
}
void* first_gen(void* array){
	printf("I am first generation thread %lu\n",pthread_self());
	int* arr = (int*)array;
	pthread_t maximum,minimum;
	pthread_create(&maximum,NULL,&second_gen_max,array);
	pthread_create(&minimum,NULL,&second_gen_min,array);
	pthread_join(maximum,NULL);
	pthread_join(minimum,NULL);
 	pthread_exit(); 
}
int max(int* array){
    int i,max=array[0],ac = count/2;
    for(i=0;i<ac;i++){
		if(array[i]>max){
				max = array[i];
		}
    }
    return max;
}
int min(int* array){
    int i,min=array[0],ac = count/2;
    for(i=0;i<ac;i++){
        if(array[i]<min){
            min = array[i];
       }
    }
    return min;
}
int sum(int* array){
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
	int* array = getArray(argv);
	count = countFile(argv);
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
	
	printf("I am MainThread %d\n",pthread_self());
	
	pthread_t child1,child2;
	pthread_create(&child1,NULL,&first_gen,(void*)firstHalf);
	pthread_create(&child2,NULL,&first_gen,(void*)secondHalf);
	pthread_join(child1,NULL);
	pthread_join(child2,NULL);
	printf("The maximum value is %d\n",totmax);
    printf("The minimum value is %d\n",totmin);
}