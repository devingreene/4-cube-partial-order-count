#include"headers.h"
#include<signal.h>

/* Each directed graph is encoded as a 32-bit word.  The least significant
   bit is 0 if 0000 points to 0001, 1 otherwise.  The second least significant
   bit is 0 if 0010 points to 0011, 1 otherwise.  The ninth least significant 
   bit is 0 if 0000 points to 0010, 1 otherwise.  The tenth least significant 
   is 0 if 0001 points to 0011, 1 otherwise, etc. */

extern void initializePotable(void);

void filter (uch *checklist);

extern int ** potable;

int main(void){
	initializePotable();

	uch *checklist=calloc(1<<28,sizeof(char));

	filter(checklist);
	return 0;
}
