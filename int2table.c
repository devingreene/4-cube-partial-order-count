#include"headers.h"

#define CARD 16
#define POS 4
#define MARGIN 8

int ** potable;
static int * block;

/* Set up partial order table for int2table */
void initializePotable(void){
	potable = (int**)malloc(CARD*sizeof(int*));
	block = (int*)malloc(CARD*CARD*sizeof(int));
}

/* convert integer encoding of directed graph into directed
   adjacency table.  See main.c for a description of the integer
   encoding */

void int2table(ui n)
{
	int nelt[CARD];
	bzero(nelt,CARD*sizeof(int));
	int *block0 = block;
	int a,b,pos,margin,bit;
	bzero(potable,CARD*sizeof(int*));

        /* pos is bit position <=> bit changed when traversing edge;
         * POS = 4 */
	for(pos=0;pos<POS;pos++)
                /* margin is standard numerical interpretation of
                 * remaining bits; MARGIN = 8 */
		for(margin=0;margin<MARGIN;margin++){
                        /* insert bit from encoding into margin bit
                         * string to obtain edge source */
			bit = (1<<pos);
			a = margin&(bit-1);
			b = margin - a;
			b <<= 1;
                        /* a is pre-insert output; */
			a += b;

                        /* a is start, b is end of edge */

                        /* Case: 1 bit is start */
			if((n>>(MARGIN*pos + margin))&0x1){
				b=a;
				a += bit;
			}
                        /* Case: 0 bit is end */
			else b = a + bit;

                        /* Initialize potable entry if needed */
			if(potable[a]==NULL){
				potable[a]=block0;
				block0 += CARD;
			}

                        /* Add edge end to a's entry */
			*(potable[a])++ = b;
			nelt[a]++;
		}

        /* Add sentry to sink nodes */
	for(a=0;a<CARD;a++){
		if(potable[a]==0){
			potable[a]=block0;
			block0 += CARD;
		}
		*(potable[a])=-1;
		potable[a] -= nelt[a];
	}
}
