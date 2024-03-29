#include"headers.h"
#include<signal.h>

/* Each directed graph is encoded as a 32-bit word.  The nth least significant
 * represents the direction of the arrow on an edge as follows.  Write n as
 * 8*k+r.  The number k corresponds, from right to left, the position of the bit
 * on the 4-bit string which changes upon traversal of the edge.  Removing that
 * bit from the string results in the remaining fixed three bits.  Viewed as a
 * 3-bit string, this corresponds to a number from 0 from 7, to which r
 * corresponds.  The value of nth bit is then set to zero if the arrow goes from
 * bit 0 to 1, and 1 otherwise.
 *
 * To take an example, suppose we have a 4-cube whose partial order which
 * inherits its order from the following linear order:
 *
 *
 * 0111 < 0110 < 1100 < 0010 < 0100 < 0001 < 1001 < 0000 < 1010 < 1011 < 1110 <
 * 1111 < 0101 < 1101 < 0011 < 1000
 *
 *
 * The bit encoding for this is then 00011000000111011001110100011001.
 *
 */

extern void initializePotable(void);
extern void freePotable(void);
extern void filter (ui start, ui end);

extern int ** potable;

/* Bitfield for marking redundant graphs */
uch *checklist;

int main(void){
    initializePotable();

    checklist=calloc(1<<25,sizeof(char));

    filter(0,1<<28);
    return 0;
}
