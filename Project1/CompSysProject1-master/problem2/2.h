#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


struct LinkedList
{
    char* data;
    struct LinkedList *next;
    
};
typedef struct LinkedList *node;

node createNode()
{
    node temp;
    temp = (node)malloc(sizeof(struct LinkedList)); 
    temp->next = NULL;
    return temp;
}

node addNode(node head, char* value)
{
    node temp,p;
    temp = createNode();
    temp->data = value; 
    if(head == NULL){
        head = temp;    
    }
    else{
        p  = head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

typedef struct tree_node
{
    FILE* fp;
    char* rootNode;
    int numChildren;
    node firstBorn;//**head child node**
    pid_t pid; 
    char* processNum;
    int procNumber;
    char line[20];

}tree_node;
