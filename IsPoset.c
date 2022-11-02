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

#ifdef TESTING_b534ff4f
extern void int2table(ui n);
extern void initializePotable(void);
int main()
{
    /* Bit encoded graphs */
    /* Items 18, 20-35 are posets */
    ui graphs[] = {
        0x096428b6, 0x402a9de1, 0x20e700b5, 0xf1924865, 0xc564b1de, 0x6ee82f6f,
        0xe867ddda, 0xee3ba40f, 0x2c3aa310, 0x8f6fd788, 0xb419ee5d, 0xc8c17844,
        0xe3d34fe7, 0x125f8fd4, 0xe4180aef, 0x7582dc9c, 0xfecff018, 0x819cf50b,
        0x7d2a5d08, 0x1ff8180b,
        0x00000000, 0x000000ff, 0x0000ff00, 0x0000ffff, 0x00ff0000, 0x00ff00ff,
        0x00ffff00, 0x00ffffff, 0xff000000, 0xff0000ff, 0xff00ff00, 0xff00ffff,
        0xffff0000, 0xffff00ff, 0xffffff00, 0xffffffff };
    int i,bl;
    _Bool failure = 0;
    initializePotable();
    for(i = 0; i < 36; i++)
    {
        int2table(graphs[i]);
        bl = IsPoset();
        if( ( ( i == 18 || i >= 20 ) && bl == 0 ) ||
        ( ( i != 18 && i < 20 ) && bl == 1 ) )
        {
            printf("Test failed at %08x\n",graphs[i]);
            failure = 1;
        }
    }
    if(!failure)
        printf("All tests passed!\n");
}
#endif
