#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include "4.h"
#include <ctype.h>
int ap=0;

char* remSpace(char* expr)
{
  char* j = expr;
  char* k = expr;
  while(*j != 0)
  {
    *j = *k++;
    if(*j != ' ')
      j++;
  }
  *j = 0;
  return expr;
}

int initArray(char* expr){
  int sz = 0;
  int sp=0;
  
  int len = strlen(expr);
  while(sp<len){
  char tmp= expr[sp];
  if(tmp == '(' || tmp==')'||tmp==' '){
  
  sp++;
  }
  else if(tmp=='+' || tmp=='*'){
  sz++;
  sp++;
  }
  else if(isdigit(tmp)){
    while(isdigit(tmp)){
      sp++;
      tmp= expr[sp];
    }
    sz++;
  }
  
  
  } 
  return sz;
}
int skipParen(char* expr,int sp){
char tmp= expr[sp];
  while(tmp !=')'){
        sp++;
        tmp= expr[sp];
              if(tmp == '(' ){
                sp = skipParen(expr,sp);
                
              }
        
  }
  return sp;    
}
int number(char* expr){
int sp=0;
int len  = strlen(expr);
char tmp= expr[sp];
    while(sp<len){
      if(!isdigit(tmp)){
        return 0;      
      }
    
      sp++;
      tmp= expr[sp];
    }
  return 1;
}
void setArray(char* expr,char* arr[]){
  char tmp;
  int len  = strlen(expr);
  int sp=0;
  if(expr[0] == '('){
    if(expr[len-1] == ')'){
      char * cut=malloc(sizeof(char) * 100);
      strncpy(cut,expr+1, len-2);
      strcpy(expr,cut);
    }
  }
  if(number(expr)){
    arr[ap]= expr;
    ap++;
    return;
  }
    while(sp<len){
    tmp= expr[sp];
    if(tmp == '(' ){
      sp = skipParen(expr,sp);
    }
    else if(tmp=='+' || tmp=='*'){
      if(tmp == '+'){
        arr[ap]="+";
      }
      else{
        arr[ap]="*";
      }
       
       ap++;
       char * str1=malloc(sizeof(char) * 100);
       strncpy(str1,expr,(sp));
       char * str2=malloc(sizeof(char) * 100); 
       strncpy(str2,expr+sp+1,((len)-(sp)));
         
         if(number(str2)){
           setArray(str2, arr);
         setArray(str1, arr);
         
         }
         else{
           setArray(str1, arr);
         setArray(str2, arr);
         
         }
         
         
       return;
    }

    sp++;
    }
  

  

}
void uniqueOP(char* init[],int len){
  int idp=0;
  int idm=0;
  int chk=0;
  
  while(chk<len){
    if(strcmp(init[chk],"+")==0){
      char* tp=malloc(sizeof(char)*100);
      sprintf(tp,"+%d",idp);
      init[chk]=tp;
      idp++;
    }
    else if(strcmp(init[chk],"*")==0){
      char* tm=malloc(sizeof(char)*100);
      sprintf(tm,"*%d",idm);
      init[chk]=tm;
      idm++;
    }
    
    
    
    chk++;
    }
  return;
}
FILE* treeSchema(char * arr[],int sz){
  FILE* sc =fopen("schema.txt","w");
  int p=0;
  while(p<sz){
    char* tmp1 = arr[p];
    char* tmp2 = arr[p+1];
    char* tmp3 = arr[p+2];
    if(number(tmp1)){
    fprintf(sc,"%s 0\n",tmp1);
      p++;
    }
    else{
      if(number(tmp2) && number(tmp3)){
        fprintf(sc,"%s 1 %s %s\n",tmp1,tmp2,tmp3);
      }
      else if(!number(tmp2)&& number(tmp3)){
        fprintf(sc,"%s 2 %s %s\n",tmp1,tmp2,tmp3);
      }
      else if(number(tmp2)&& !number(tmp3)){
        fprintf(sc,"%s 3 %s %s\n",tmp1,tmp2,tmp3);
      }
      else if(!number(tmp2) && !number(tmp3)){
        fprintf(sc,"%s 4 %s %s\n",tmp1,tmp2,tmp3);
      }
      p++;
    }
  
  }
  fclose(sc);
  FILE* fp = fopen("schema.txt","r");
  return fp;

}
int createTree(tree_node *thestruct, char* ident) // when node has 0 children what should ident BE?
{    


    FILE* fp = thestruct->fp;
    FILE* getRoot = thestruct->fp;
    char buffer[100];
    char* nextChild;
    node newNode;
    node firstBorn = createNode();
    pid_t pid;
    
   
    
          
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
    char *rootNode = strtok(buffer," ");
    
    char *md = strtok(NULL," ");
    int mode = atoi(md); 
    thestruct->rootNode = strdup(rootNode); 
    thestruct->mode = mode;
                                             
    if(rootNode[0]=='+' || rootNode[0]=='*'){
    // printf("here %c\n",rootNode[0]);
          thestruct->op = rootNode[0];
          }
    
            int fd[4];
          int i;
      		for(i = 0; i < 2; i++){
      			pipe(&fd[2*i]);//set up pipe
      		}
   pid_t* pids = (pid_t*) malloc(sizeof(pid_t) * 10);
    
              if(mode==0){
                  int ans = atoi(thestruct->rootNode);
                  return ans; // this is returned value to pipe 
              }
              else //if number of children > 1
              {
                  char *stream = strtok(NULL," ");
                  firstBorn->data = stream;  
                          nextChild = strtok(NULL,"\n");
                          newNode = addNode(firstBorn,nextChild);
                          i = 0;
                           
                          while(i<2)
                          {
                          
                                  pids[i] = fork();
                                
                                  wait(NULL);
                                  if (pids[i] == 0) 
                                  {
                                      break;
                                  }
                                  else
                                  {
                                      firstBorn = firstBorn->next;
                                      i++;  
                                  }
                              
                              
                          }  
                          if (pids[i] == 0 && i<2) 
                          { 
                            close(fd[2*i]);
                             int ans = createTree(thestruct, firstBorn->data);
                          
                          write(fd[2*i+1],&ans,sizeof(ans));
             					     close(fd[2*i+1]);
                  					exit(0);  
                          }
                       
                          int yoshi[2];
                          
                          int status;
                          for(i = 0; i < 2; i++){
                          
                      			close(fd[2*i+1]);
                      			waitpid(pids[i],&status,0);
                           
                      			read(fd[2*i],&yoshi[i],sizeof(yoshi[i]));
                            
                      			close(fd[2*i]);
                            
                      		}
                        
                      		if(rootNode[0] == '+'){
                         
     
                      			return yoshi[0]+ yoshi[1];
                      		}else if(rootNode[0] == '*'){
                           
                      			return yoshi[0] * yoshi[1];
                      		}         
          
                         
                         exit(0);
                        return;
             }
}
int main(int argc, char *argv[])
{
    char* fileName = argv[1];
    char buffer[100];
      char* ident="";
    FILE* fp;
    fp = fopen(fileName,"r");
  
  fgets(buffer,100,fp);
  char* expr = malloc(sizeof(char) * 100);
  sprintf(expr,"%s",buffer);
  expr= remSpace(expr);
  
    int sz;
    sz = initArray(expr);
   
    char* init[sz];
    setArray(expr,init);
  
    uniqueOP(init,sz);
    FILE* ts = treeSchema(init,sz);
    tree_node *initNode = malloc(sizeof(tree_node));
    initNode->fp = ts;  

  int ans = createTree(initNode, ident);
  FILE * fo = fopen("output4.txt","w");
  fprintf(fo,"Your answer is %d",ans);

   fclose(fp);
   fclose(fo);
}
