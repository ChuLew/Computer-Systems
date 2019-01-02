#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char *argv[]){
   FILE * fo = freopen("outputC.txt","w",stdout);
    char* fileName = argv[1];
    char buffer[1000];
    int max=0;
    int min=100000;
    int tmp;
    int sum=0;

    //counts number of items in text file and creates array of length count
    FILE* fpcount;
    fpcount=fopen(fileName,"r");
    int count=0;
    
       while(NULL!=fgets(buffer,1000,fpcount)){
           bzero(buffer, 1000);
           count++;
       };
       
    int array[count];
 
    // place text file into array
    
    int ac=0;
    FILE* fp;
    fp = fopen(fileName,"r");
    
      while(fgets(buffer, 1000, fp)!= NULL){
        
        tmp = atoi(buffer);
        array[ac]=tmp;
        //printf("%d\n",array[ac]); 
        ac++;
        
    }
                 /*begining of the forking proccess*/
 
     pid_t pid,pid2;
     printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
     fflush(stdout); 
     int mypipefd[2];
     int thepipefd[2];
     int finanlpipefd[2];
     int ret;
     char buf[100];
     char buf2[100];
     ret = pipe(mypipefd); 
     pipe(finanlpipefd);
     
     if(ret==-1)
     {
         perror("pipe");
         exit(1);
     
     }
     
     pid = fork();
     
     if(pid==0)
     //child process
     {
     printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
     fflush(stdout);  
         //printf("Child Process\n");
         close(mypipefd[0]);
         write(mypipefd[1], "** 1ST STARTING ** \n", 100);
         
         pid_t pidMax,pidMin;
         int mypipefd1[2],mypipefd2[2];
         ret=pipe(mypipefd1);
         
         if(ret==-1)
         {
             perror("pipe1");
             exit(1);
         
         }
         ret=pipe(mypipefd2);
         
         if(ret==-1)
         {
             perror("pipe2");
             exit(1);
         
         }
         pidMax=fork();
         
         if(pidMax==0)
         {//grandchildmax of child1
             //close(mypipefd1[0]);
             printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
             fflush(stdout);  
             int j;
             for(j=0; j<(count/2)+1; j++)
             {
                   if (array[j]>max)
                   {
                       max=array[j];
                   }
             }
             //printf("The maximum value of the 1st half of the array is %d\n",max);
             bzero(buf,100);
             sprintf(buf,"%d", max);
             write(mypipefd1[1],buf,100);
             
         }  else{//child1
             //close(mypipefd1[1]);
             read(mypipefd1[0],buf,100);
             //printf("grandchild1 to child1: %s", buf);
             write(mypipefd[1],buf,100); //writing grandchild1 max to main process
             
             
             pidMin=fork();
  
              if(pidMin==0)
              {//grandchildmin of child1
                  //close(mypipefd2[0]);
                  printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
                  fflush(stdout);  
                  int i;
                  for(i=0; i<(count/2); i++)
                  {
                      if (array[i]<min)
                      {
                      min=array[i];
                      }
                  }
                  bzero(buf,100);
                  sprintf(buf,"%d", min);
                  write(mypipefd2[1],buf,100);        
      
              } else{//child1 from grandchildmin
                     bzero(buf,100);
                     read(mypipefd2[0],buf,100);
                     write(mypipefd[1],buf,100);
                     //printf("11111111: %s\n",buf);
                     
                     //printf("here the sum is calculated\n");
                     bzero(buf,100);
                     int sum=0;
                     int k;
                         for(k=0; k<(count/2); k++){
                             sum += array[k];
                         }
                     bzero(buf,100);
                     sprintf(buf,"%d", sum);
                     write(mypipefd[1],buf,100);
             
               }
             
             
             
             
             
           }         
     
     
     }  else{
     //parent process
     
         //printf("Parent Process\n");
         //close(mypipefd[1]);
         read(mypipefd[0],buf, 100);
         //printf("child1 to main: %s",buf);
         
         //bzero(buf,1000);
         read(mypipefd[0],buf,100);
         int max1 = atoi(buf);
         //printf("The MAX number of first half is: %s\n",buf);
         
         bzero(buf,100);
         read(mypipefd[0],buf,100);
         int min1 = atoi(buf);
         //printf("The MIN number of first half is: %s\n",buf);        
         
         bzero(buf,100);
         read(mypipefd[0],buf,100);
         int sum1 = atoi(buf);
         //printf("The SUM of first half is: %s\n",buf);   
         
        // printf("now i wanna fork the 2nd child process\n");
        
        
                      /* START OF 2ND FORK */
                      
        ret = pipe(thepipefd);
     
         if(ret==-1)
         {
             perror("thepipe");
             exit(1);
         
         }
         
         pid2 = fork();
         
         if(pid2==0)
         {
         printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
         fflush(stdout);  
             write(thepipefd[1],"** 2ND STARTING ** \n", 100);
             
             pid_t pidMax,pidMin;
             int thepipefd1[2],thepipefd2[2];
             
             ret=pipe(thepipefd1);
             
             if(ret==-1)
             {
                 perror("pipe1");
                 exit(1);
             
             }
             
             ret=pipe(thepipefd2);
         
             if(ret==-1)
             {
                 perror("pipe2");
                 exit(1);
             
             }
             pidMax=fork();
             
             if(pidMax==0)
             {//grandchildmax of child2
             printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
             fflush(stdout);  
                   int j;                     
                   for(j=(count/2); j<count; j++)
                   {
                       if (array[j]>max)
                       {
                           max=array[j];
                       }
                   }
                   bzero(buf2,100);
                   sprintf(buf2,"%d", max);
                   write(thepipefd1[1],buf2,100);   
                  
             }else{
                 //child2
                 read(thepipefd1[0],buf2,100);
                 //printf("contents of buf2: %s\n",buf2);
                 write(thepipefd[1],buf2,100);//writing grandchild2 max to child2 process
                 
                 
                 pidMin=fork();
                 
                   if(pidMin==0)
                   {
                   printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
                   fflush(stdout); 
                       int i;
                       for(i=(count/2); i<count; i++)
                       {
                           if (array[i]<min)
                           {
                                min=array[i];
                           }
                       }
                        bzero(buf2,100);
                        sprintf(buf2,"%d", min);
                        write(thepipefd2[1],buf2,100);
                       //write(thepipefd2[1],"** MIN # 2ND HALF **\n",100);
                       
                   }
                   else{
                   bzero(buf2,100);
                   read(thepipefd2[0],buf2,100);
                   write(thepipefd[1],buf2,100);
                   
     
                    int sum=0;
                    int k;
                    for(k=(count/2); k<count; k++)
                    {
                        sum += array[k];
                    }
                    bzero(buf2,100);
                    sprintf(buf2,"%d",sum);
                    //printf("contents of buf2: %s\n",buf2);
                    write(thepipefd[1],buf2,100);
                   }
                   
                   
                     
             }
             
         
         }else{//parent function
         
             bzero(buf2,100);
             read(thepipefd[0],buf2,100);
             //printf("child2 to main: %s",buf2);
             
             bzero(buf2,100);
             read(thepipefd[0],buf2,100);
             int max2 = atoi(buf2);
             //printf("The MAX number of 2nd half is %s\n",buf2);
             
             bzero(buf2,100);
             read(thepipefd[0],buf2,100);
             int min2 = atoi(buf2);
             //printf("The MIN number of 2nd half is %s\n",buf2);
             
             bzero(buf2,100);
             read(thepipefd[0],buf2,100);
             int sum2 = atoi(buf2);
             //printf("The SUM of second half is: %s\n",buf2);
             
             /*comparison of max, min, and sum of child 1 and child 2*/
             //printf("THESE ARE THE OVERALL MAX,MIN,SUM VALUES\n");
             sum=sum1+sum2;
             
             
             pid_t oMin;
             oMin = fork();
             
             if(oMin==0)
             {
                  printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
                  fflush(stdout); 
                 if(min1<min2)
                 {
                     min=min1;
                 
                 }else{
                     min=min2;  
                     }
                     
                 
                
                  pid_t oMax;//getting forked twice
                 oMax = fork();
                 
                 if(oMax==0)
                 {
                     printf("Hi, I'm process %d and my parent is %d\n",getpid(),getppid());
                     fflush(stdout); 
                     if(max1>max2)
                     {
                         max=max1;
                 
                     }else{
                         max=max2;
                       
                     }
                     printf("SUM=%d\n",sum);
                      printf("MAX=%d\n",max);
                       printf("MIN=%d\n",min);
                      
                 }else{
                 //do nothing

                 }
          
   
             }
            

            
            fclose(fo);
         }
         
         
        
         
     wait(NULL);
     }  

}
