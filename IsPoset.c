#include"headers.h"

#define CARD (16)

extern int **potable;
/* When one of the branches of the recursive call hits a loop,
   we want to tell the other instances of 'lookAhead' to return 
   zero.  We do this by setting 'flag' to one, which tells the 
   other recursive branches to return zero immediately. */

static int flag=0;

/* 'hot' is the array of nodes forming the path so far */
static int lookAhead(int start,int *hot,int nhot){
    if(flag) return 0;
    int i,j,product=1;
    for(i=0;potable[start][i]!=-1;i++){
        /* hot[nhot-1] is the previous node.  This is a directed graph,
         * so hot[nhot-1] cannot be in potable[start] */
        for(j=0;j<nhot-1;j++){
            if(unlikely(potable[start][i]==hot[j])){
                flag=1;
                return 0;
            }
        }
        hot[nhot]=potable[start][i];
        /* 'hot' used over all branches in the recursion.  This isn't a
         * problem, though, since we only look at the members of the
         * array which are live for the current branch */
        product &= lookAhead(potable[start][i],hot,nhot+1);
    }
    return product;
}

/* Acts on "static" potable declared above */
int IsPoset(void){
    int start,hot[CARD];
    for(start=0;start<CARD;start++){
        flag=0;
        *hot=start;
        if(lookAhead(start,hot,1)) continue;
        return 0;
    }
    return 1;
}
