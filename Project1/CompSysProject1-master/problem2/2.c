#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include "2.h"

tree_node* createTree(tree_node *thestruct, char* ident) // when node has 0 children what should ident BE?
{    
    FILE* fp = thestruct->fp;
    FILE* getRoot = thestruct->fp;
    char* pni = thestruct->processNum;
    char buffer[100];
    char* nextChild;
    char* strNumProc;
    int procNum;
    node newNode;
    node firstBorn = createNode();
    pid_t pid;
    
    int size = strlen(pni);
    int j;
    char yee = ident[0];
    
    for(j=0;j<size;j++)
    {
        if(pni[j]==yee)
        {
            break;
        }
    }
    char yami = pni[j+2];
    char yame[2] = {yami,'\0'};
    procNum = atoi(yame);
    thestruct->procNumber = procNum;      
    if(strcmp(ident,"")==0)// for initial condition if empty string was put into ident means first line
    {  
        fgets(buffer, 100, fp); 
    
    }
    else //this is to find the line where we create the ident from input file
    {
        char yayo[100];
        while(fgets(buffer,100,fp))
        {
           sprintf(yayo,"%s",buffer);     
           char *chk = strtok(yayo," ");
           unsigned char hello = ident[0];
           unsigned char goodbye = chk[0];
           if(hello==goodbye)
           {
               break;       
           }
          
        }
       
    }
    sprintf(thestruct->line,"%s",buffer);     
    char *rootNode = strtok(buffer," ");
    
    char *strNumChild = strtok(NULL," ");
    int numKids = atoi(strNumChild); 
    thestruct->rootNode = strdup(rootNode); 
    thestruct->numChildren = numKids;
    
    if(numKids==0){    
        pid = getpid();
        thestruct->pid = pid; 
        char* rotNode = thestruct->rootNode;
        sleep(1);
        return thestruct;
    }

    else if(numKids ==1)
    {
        char *stream = strtok(NULL,"\r\n");
        firstBorn->data = stream; 
        pid = fork();
        if (pid==0)
        {
            createTree(thestruct, stream);

            pid = getpid();
            thestruct->pid = pid; 
                  
            return thestruct;
        }
    }
    
    else //if number of children > 1
    {
        char *stream = strtok(NULL," ");
        firstBorn->data = stream;  
        int i;
                for(i=0;i<numKids-2;i++)
                {     
                    nextChild = strtok(NULL," ");      
                    newNode = addNode(firstBorn,nextChild);
                } 
                nextChild = strtok(NULL,"\n");
                newNode = addNode(firstBorn,nextChild);
                while(firstBorn != NULL)
                {             
                    pid = fork();
                    if (pid == 0) 
                    {
                        break;
                    }
                    else
                    {
                        firstBorn = firstBorn->next;  
                    }
                }  
                if (pid == 0) 
                {  
                    createTree(thestruct, firstBorn->data);  
                }         

               pid = getpid();
               thestruct->pid = pid; 
               return thestruct;
    }
}
void print_tree(tree_node *thestruct,int* fd,int numKids)
{    
    printf("%s",thestruct->line);
    fflush(stdout);
    int i = 0;
    char buff[20];
     
    for(i=0;i<(numKids);i++)
    {  
        bzero(buff,20);
        close(fd[2*i+1]);
        read(fd[2*i], buff, 20);
        close(fd[2*i]);
        printf("%s",buff);
        fflush(stdout);
    }
        
      
        
    
}

int main(int argc, char *argv[])
{       
   FILE *fo = freopen("output2.txt","w",stdout);
    
    char* fileName = argv[1];
    char buffer[100];
    char* init="";
    char* strNumChild;
    int numKids=0;
    pid_t pid = getpid();
    FILE* fp;
    FILE* countChildren;
    FILE* sc;
    countChildren = fopen(fileName,"r");
    fp = fopen(fileName,"r");
    sc = fopen(fileName,"r");
    int procNum = -1; 
    char* letter;
    char convert[1]; 
    
    while(fgets(buffer,100,countChildren))
    {
        strtok(buffer," ");
        strNumChild = strtok(NULL," ");
        numKids += atoi(strNumChild); 
        
    }
    
    char* pni = (char*)malloc((numKids*2)*sizeof(char)); // pni = process number identfier 
    
   
    
    while(fgets(buffer,100,sc))
    {
        letter = strtok(buffer," ");
        memcpy(pni+4*procNum,letter,1);
        memcpy(pni+4*procNum+1," ",1);
        sprintf(convert,"%d",procNum);
        memcpy(pni+4*procNum+2,convert,1);
        memcpy(pni+4*procNum+3," ",1);
        procNum++;
        
    }  
      
    // printf("%s\n\n\n\n\n",pni);
     
     
    int fd[2*numKids];
    //printf("%d\n",numKids);
    int i = 0;
    
    for(i=0;i<numKids;i++)
    {
        pipe(&fd[2*i]);
    }

    tree_node *initNode = malloc(sizeof(tree_node));
    initNode->fp = fp;  
    initNode->processNum = pni;
    initNode->numChildren = 0;
    initNode = createTree(initNode, init);
    
      char* rootNode = initNode->rootNode;
      pid_t dummyPID = initNode->pid;
      

      if(dummyPID == pid)
      {  
        
         print_tree(initNode,fd,numKids);  
      }
      else{//child process stuff 
         procNum = initNode->procNumber;
         //printf("%d\n",procNum);
         close(fd[2*procNum]);
         char* line = strdup(initNode->line); 
         write(fd[2*procNum+1],line,20);
         close(fd[2*procNum + 1]); 
      } 
     //fclose(fo);
}

