#include"headers.h"

#define NODES (8)
#define PERMS (8*7*6*5*4*3*2)
#define EDGES (12)

extern int (*permblock)[NODES];
extern void recurseShuffle(int *perm,int len);

int main(void){
	permblock = malloc(PERMS*NODES*sizeof(int));
	int perm[NODES] = {0,1,2,3,4,5,6,7};
	recurseShuffle(perm,NODES);

	int graph,position,margin,shift,a,b,vec[2*EDGES],p,m;
	for(graph=0;graph<(1<<EDGES);graph++){
		for(position=0,shift=0;position<3;position++){
			for(margin=0;margin<4;margin++){
				a=margin&((1<<position)-1);
				b= margin - a;
				b <<= 1;
				b+= a;
				if(graph&(1<<shift)){
					vec[2*shift]=b+ (1<<position);
					vec[2*shift+1] = b;
				}
				else{
					vec[2*shift]=b;
					vec[2*shift+1]=b+(1<<position);
				}
				shift++;
			}
		}
		for(p=0;p<PERMS;p++){
			for(shift=0;shift<EDGES;shift++){
				for(m=0;m<NODES;m++){
					if(permblock[p][m]==vec[2*shift]) break;
					if(permblock[p][m]==vec[2*shift+1]) goto loc1; 
				}
			}
			printf("%03x\n",graph);
			goto loc2;
loc1:;
		}
loc2:;
	}
	return 0;
}
