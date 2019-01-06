#include"headers.h"

#define CARD (16)

extern int **potable;
/* When one of the branches of the recursive call hits a loop,
   we want to tell the other instances of 'lookAhead' to return 
   zero.  We do this by setting 'flag' to one, which tells the 
   other recursive branches to return zero immediately. */

static int flag=0;

static int lookAhead(int start,int *hot,int nhot){
    if(flag) return 0;
    int i,j,product=1;
    for(i=0;potable[start][i]!=-1;i++){
        for(j=0;j<nhot-1;j++){
            if(__glibc_unlikely(potable[start][i]==hot[j])){
                flag=1;
                return 0;
            }
        }
        hot[nhot]=potable[start][i];
        product &= lookAhead(potable[start][i],hot,nhot+1);
    }
    if(i==0) return 1;
    return product;
}

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
