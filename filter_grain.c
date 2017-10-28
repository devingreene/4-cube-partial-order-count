#include"headers.h"
#include<assert.h>

#ifndef START_Q4
#define START_Q4 0x0
#endif

#ifndef NQ4
#define NQ4 0x10000000
#endif

#define GROUP_CARD 384

extern void int2table(ui n);
extern int IsPoset(void);
extern ui symmetry(ui graph,uch pflip,uch tcycle,uch tau,uch xorop);
ui graph;

int IsBaseSource(ui graph){
	return !(0x01010101&graph);
}

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

/* We count the number of acyclic directed graphs in 
   Q4.  

	Procedure :
		1. Allocate table of size 2^28 corresponding to graph with 0000
		as the base.  
		2. For each entry: 
			a. Check off (set equal to 1) it if contains cycles. 
		   	b. Take all 384 group transformations.  Of these,
				if any have 0000 as base, and lie further down the table,
				check off that element of the table (set value to -1)
			c. Count how many group elements fix the current table 
			entry.  Number of distinct isomorphs is 384/<number fixed>
 */
	
/* Symmetry transformations */
#define BIGLOOP \
	for(pflip=0;pflip<4;pflip++) \
	for(tcycle=0;tcycle<3;tcycle++) \
	for(tau=0;tau<2;tau++) \
	for(xorop=0;xorop<16;xorop++) 

void filter (uch *checklist)
{
	ui graph_i,tgraph,tgraph_i,nisomorphs,count;
	uch pflip,tcycle,tau,xorop;

	count = 0;

	for(graph=START_Q4;graph<NQ4;graph++)
	{
		/* Skip those checked off */
		if(checklist[graph]) continue;

		graph_i = fixbp2source(graph);

		/* Check if partial order */
		int2table(graph_i);
		if(IsPoset())
		{
			nisomorphs = 0;
			BIGLOOP
			{
				/* transformed graph */
				tgraph_i = symmetry(graph_i, pflip, tcycle, tau, xorop);
				if(IsBaseSource(tgraph_i))
				{
					/* remove "base" bits for table entry */
					tgraph = invfixbp2source(tgraph_i);

					/* check off isomorphic graphs further down the list */
					if(tgraph > graph)
						checklist[tgraph]=1;
				}
				nisomorphs += tgraph_i == graph_i;
			}

			assert((GROUP_CARD % nisomorphs) == 0);

			count += GROUP_CARD/nisomorphs;
		}
	}
	printf("%u\n",count);
}
