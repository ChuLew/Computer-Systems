#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char *argv[]){
FILE* fo = freopen("outputB.txt","w",stdout);

char* fileName = argv[1];
    char buffer[1000];
    char mins[100];
    char maxs[100];
    char sums[100];
    int max;
    int min;
    int tmp;
    int sum=0;
    int count=0;
    int fdsum[2];
    int fdmax[2];
    int fdmin[2];
    int nbytes;
    pipe(fdmin);
    pipe(fdmax);
    pipe(fdsum);
    
    int ret;
    char buf[20];

    FILE* fpcount;
    fpcount=fopen(fileName,"r");
   while(NULL!=fgets(buffer,1000,fpcount)){
   bzero(buffer, 1000);
   count++;
   };
    int array[count];
    
    
    FILE* fp;
    fp=fopen(fileName, "r");

    int ac=0;
     while(fgets(buffer, 1000, fp)!= NULL){
     
     if(ac==0){
           max = tmp;
           min = tmp;
     }
     
     
     
    tmp = atoi(buffer);
    array[ac]=tmp;
    //printf("%d\n",array[ac]); 
    ac++;
    }
    min=array[0];
       //freopen("output.txt", "w", stdout); 
    printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());  
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0){
    printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
    fflush(stdout);
    pid_t pid1 = fork();
      
      if (pid1 == 0){
      printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid()); 
      fflush(stdout); 
      pid_t pid2 = fork();
      
      if (pid2 == 0){
      printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
      fflush(stdout);
        int k;
        
         for(k=0; k<count; k++){
           sum += array[k];
         }
       
       pid1 = wait(NULL);
       
       sprintf(sums,"Sum = %d\n", sum);
       close(fdsum[0]);
        write(fdsum[1], sums, sizeof(sums));
        return; 
      }
      
      
      max=array[0];
         int j;
         for(j=1; j<count-1; j++){
           if (array[j]>max){
             max=array[j];
         }
       }
       //bzero(buffer, 1000);
       //sprintf(buffer,"The maximum value is %d\n", max);
       //printf(buffer);
      //close(mypipefd2[0]);
      // write(mypipefd2[1], buffer, 1000);//wat if i wrote to a different buffer??? //wat if write from grandchild to child to parent???
       //printf("Located inside the buffer is %s\n",buffer);
       //close(mypipefd2[1]);
        
        pid2 = wait(NULL);
        
        sprintf(maxs,"Max = %d\n",max);
        close(fdmax[0]);
        write(fdmax[1], maxs, sizeof(maxs));
        return;
        
      }
      
      
       int i;
      // printf("the 000000count is %d\n", count);
       for(i=0; i<count-1; i++){
         //printf("what is contained in array is %d\n",array[i]);
         if (array[i]<min){
           min=array[i];

       }

     }

     pid1 = wait(NULL);
     
     sprintf(mins,"Min = %d\n",min);
     close(fdmin[0]);
     write(fdmin[1], mins, sizeof(mins));
     return;
     
   }
   else{
   
   close(fdmin[1]);
   close(fdmax[1]);
   close(fdsum[1]);
   pid = wait(NULL);
   nbytes = read(fdmin[0], mins, sizeof(mins));
   nbytes = read(fdmax[0], maxs, sizeof(maxs));
   nbytes = read(fdsum[0], sums, sizeof(sums));
   printf("%s", maxs);
   printf("%s", mins);
   printf("%s", sums);
   
   }

   
    
    
      	fclose(fo);

    return;
      

}

