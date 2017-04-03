#include"headers.h"

int **potable;
//extern int hot;
//int nlines=NLINES;

int IsPoset(void);

int main(){
	potable = malloc(NLINES*sizeof(int*));
	int offset[NLINES];
	int node=0;
	bzero(offset,NLINES*sizeof(int));
	int *block=malloc(NLINES*NLINES*sizeof(int));
	FILE *fp=fopen("poset","r");
	char *line=NULL,*endptr=0;
	size_t n;
	ssize_t result;
	do{
		result=getline(&line,&n,fp);
		if(result==-1) break;
		potable[node]=block;
		block += NLINES;
		while(*line != '\n'){
			potable[node][offset[node]++]=strtol(line,&endptr,0);
			line=endptr;
		}
		potable[node][offset[node]] = -1;
		node++;
	} while(1);
	fclose(fp);
	//int hot[NLINES];
	return IsPoset();
}
