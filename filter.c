#include"headers.h"
#include<signal.h>

#define START_Q4 0x0

#ifndef NQ4
#define NQ4 0X10000000
#endif

extern void int2table(ui n);
extern int IsPoset(void);
extern ui symmetry(ui graph,uch pflip,uch tcycle,uch tau,uch xorop);
ui graph;

void handler(void){
	fprintf(stderr,"At %07x\n",graph);
}

int IsBaseSource(ui graph){
	return !(0x01010101&graph);
}

/* We loop over graphs with 0000 as a source.  So we can encode our graphs
   with 28 bit words.  The following two functions convert back and forth 
   from the 28 bit representation to the 32 bit one */
ui fixbp2source(ui graph){
	ui a,b,c;
	a=0xfe00000&graph;
	b=0x1fc000&graph;
	c=0x3f80&graph;
	graph &= 0x7f;
	a <<= 4;
	b <<= 3;
	c <<= 2;
	graph <<= 1;
	return graph + a + b + c;
}

ui invfixbp2source(ui graph){
	ui a,b,c;
	a=0xff000000&graph;
	b=0x00ff0000&graph;
	c=0x0000ff00&graph;
	graph &= 0x000000ff;
	a >>= 4;
	b >>= 3;
	c >>= 2;
	graph >>= 1;
	return graph + a + b + c;
}

/* This routine marches through each directed graph on Q4.  If it is
   not a partial order, or if it is an isomorphic copy of a previous checked
   graph, we mark this in the 'checklist' so we can skip it when we get to it.

	Since we are listing isomorphism classes, and since these are partial 
	orders, we fix our search set to those graphs where 0000 is a source.
	This reduces the number of iterations from 2**32 to 2**28. */

void filter (char *checklist){
	struct sigaction sa;
	sa.sa_handler=(void(*)(int))handler;	
	sigaction(SIGQUIT,&sa,0);

	ui Graph,tgraph;
	for(graph=START_Q4;graph<NQ4;graph++){
		if(checklist[graph]) continue;
		Graph = fixbp2source(graph);
		int2table(Graph);
		if(__glibc_unlikely(IsPoset())){
			printf("%08x\n",Graph);
		}

		/* flip through all symmetries */
		uch pflip,tcycle,tau,xorop;
		for(pflip=0;pflip<4;pflip++)
			for(tcycle=0;tcycle<3;tcycle++)
				for(tau=0;tau<2;tau++)
					for(xorop=0;xorop<16;xorop++){
						tgraph = symmetry(Graph,pflip,tcycle,tau,xorop);
						if(IsBaseSource(tgraph)){
							tgraph = invfixbp2source(tgraph);
							if(tgraph > graph)
								checklist[tgraph]=1;
						}
					}		
	}
}
