#include"headers.h"
#define NODES (8)

/* Upon completion of this routine, 
   `permblock' will contain all permutations
   of NODES	elements */

int (*permblock)[NODES];
static int j=0;

void recurseShuffle(int *perm,int len){
	int i;
	if(len==1){
		perm -= (NODES-1);
		for(i=0;i<NODES;i++){
			permblock[j][i]=perm[i];
		}
		j++;
		return ;
	}
	recurseShuffle(perm+1,len-1);
	int a;
	for(i=1;i<len;i++){
		a=perm[0];
		perm[0]=perm[i];
		perm[i]=a;
		recurseShuffle(perm+1,len-1);
		a=perm[0];
		perm[0]=perm[i];
		perm[i]=a;
	}
}
