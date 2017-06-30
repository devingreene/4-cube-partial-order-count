#include<stdlib.h>
#include<stdio.h>
#include<error.h>
#include "../headers.h"

#define NLOCI 4
#define NGENOTYPES 16

uch peak_count(char *code)
	/* Input is code as described in 
	   ../main.c */
{
	ui n,i,dir,base,insert_at,low,gtype,narr_in[NGENOTYPES];
	uch tally = 0;

	n = strtol(code,0,16);

	/* Clear narr_in */
	for(i = 0;i<NGENOTYPES;i++) narr_in[i] = 0;

	/* Go through coded graph, tallying in arrows */
	for(i=0;i<32;i++)
	{
		dir = !(n&1);
		/* ex. if abcd -> aecd, then base = acd and insert_at = 2 */
		base = i%8;
		insert_at = i / 8;

		/* Mask the lower bits */
		low = base&((1<<insert_at)-1);

		/* Derive the genotype arrow points into */
		gtype = low + // low part
			(dir<<insert_at) + // side arrow points into
			((base-low)<<1); // high part
		narr_in[gtype]++;

		n >>= 1;
	}

	/* Tally peaks */
	for(i=0;i<NGENOTYPES;i++) tally += narr_in[i]==NLOCI;

	return tally;
}

int main()
	/* Feeds input file.  Very strong assumption 
	here that input file is in precisely the right
	format. */
{
	char code[9]; // Extra byte for null byte
	while(fgets(code,9,stdin))
	{
		/* Scan past newline */
		fgetc(stdin);

		printf("%s:%u\n",code,peak_count(code));
	}
}
