#include"headers.h"

#define CARD 16
#define POS 4
#define MARGIN 8

int ** potable;
static int * block;

void initializePotable(void){
	potable = malloc(CARD*sizeof(int*));
	block = malloc(CARD*CARD*sizeof(int));
}

static ui thisreversemiddletwo(ui n){
	ui a,b;
	a = n&0x020;
	b = n&0x040;
	n &= 0xf9f;
	return n + (a<<1) + (b>>1);
}

/* convert integer encoding of directed graph into directed
   adjancency table */
void int2table(ui n)
{
	n = thisreversemiddletwo(n);
	int nelt[CARD];
	bzero(nelt,CARD*sizeof(int));
	int *block0 = block;
	int a,b,pos,margin,bit;
	bzero(potable,CARD*sizeof(int*));
	for(pos=0;pos<POS;pos++)
		for(margin=0;margin<MARGIN;margin++){
			bit = (1<<pos);
			a = margin&(bit-1);
			b = margin - a;
			b <<= 1;
			a += b;
			if((n>>(MARGIN*pos + margin))&0x1){
				b=a;
				a += bit;
			}
			else b = a + bit;
			if(potable[a]==0){
				potable[a]=block0;
				block0 += CARD;
			}
			*(potable[a])++ = b;
			nelt[a]++;
		}
	for(a=0;a<CARD;a++){
		if(potable[a]==0){
			potable[a]=block0;
			block0+=CARD;
		}
		*(potable[a])=-1;
		potable[a] -= nelt[a];
	}
}
