#include "headers.h"

/* This is a tentative optimization of IsPoset.c.  No testing
 * yet! */
/* Jan 16, 2018: Light testing:  Worsens rather than enhances performance, but
 * not by much.  Perhaps a test on the full set is in order? */
/* XXX Jan 16, 2018: Weird, this one is worse than the original, but only by a 
 * few seconds.  Could tweak around with it, but is it worth it? */

#define DUMMY (-2)
#define CARD (16)

extern int **potable;

int IsPoset(void)
{
	int i,j,k,*node,altered;
	/* Look for and remove extrema in potable */
	do
	{
		altered = 0;
		for (i = 0 ; i < CARD ; ++i)
		{
			/* Skip dummies */
			for(j = 0 ; potable[i][j] == DUMMY ;j++);
			/* Check if i is a maximum or already removed */
			if(potable[i][j] == -1)
			{
				/* Entry empty, so blank out and remove elsewhere */
				potable[i][0] =  -1;
				/* recycling j */
				for(j = 0 ; j < CARD ; j++)
				{
					node = potable[j];
					for(k = 0; node[k] != -1; k++)
						if(node[k] == i) 
						{
							/* It was a max */
							node[k] = DUMMY;
							altered = 1;
						}
				}
			}
			else
			{
				/* Check if i is a minimum */
				for( j = 0 ; j < CARD ; j++)
				{
					node = potable[j];
					for(k = 0 ; node[k] != -1 ; k++)
						/* Exit loop if not a minimum */
						if(node[k] == i) goto next;


				}
next:
				continue;
				/* If we're here, it's a minimum, so scratch it */
				potable[i][0] = -1;
				altered = 1;
			}
		}
	} while(altered);
	/* If any vertices are left, it's not a poset */
	for(i = 0 ; i < CARD ; i++) if(potable[i][0] != -1) return 0;
	return 1;
}
