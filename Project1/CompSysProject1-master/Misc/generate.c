#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	int i;
	
	FILE *file;
	file = fopen(argv[1],"w");
 	for(i = 0; i < 100000; i++){
		fprintf(file, "%d\n", rand()%10000+1);
	}
	fprintf(file, "\n");
	
	return 0;
  
}

