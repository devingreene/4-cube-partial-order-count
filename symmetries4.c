#include"headers.h"

#define MAG 8

#define SPLIT_UP \
    a = 0xff000000&graph; \
    b = 0x00ff0000&graph; \
    c = 0x0000ff00&graph; \
    graph &= 0x000000ff


/* We encode a directed graph on the 4-cube by creating an ``edge
 * bitmask''.  Each directed edge on the 4-cube is mapped to a bit
 * position of a 32 bit string.  The positions are indexed by symbols
 * of the form b1.b2.*.b3, consisting of three constant bits and the
 * star corresponding to the bit in the genotype which changes if we
 * traverse the edge.  We refer to the constant bits b1.b2.b3 as the
 * ``background'' bits, and view them as the number they represent in
 * base 2.  As there are eight possible values for the background
 * bits, and there are four positions for the star, we have 32 bits
 * whose value indicates the direction of the directed edge.  We order
 * the bitmask indices by moving the star from right to left, and for
 * each star position counting from 0 to 7 and matching this number to
 * the background bits base 2 representation.  Schematically, we have
 *
 * *111, *110, *101, *100, ... , 1*11, 1*10, 1*01, ... , 11*1, 11*0,
 *       10*1, ..., 111*, 110*, 101*, 011*, 010*, 001*, 000*
 *
 * Any group action on the 4-cube can be decomposed into the product
 * of a permutation on the coordinates which determine the vertices of
 * the 4-cube and an xor-ing operation on said coordinates.  A
 * permutation can in turn be expressed as the product of parity
 * preserving second order group action, a single three cycle, and a
 * single transposition.  The functions below implement these
 * group actions via their effects on the edge bitmask representing a
 * directed graph on the cube. */

static inline ui effof01(ui n,ui shift){
    /*
     * On background: xyz -> xzy
     * Effect on edge mask: 76543210 -> 75643120
     */
    n >>= shift;
    ui a,b;
    a=n&0x44;
    b=n&0x22;
    n -= a+b;
    n += (a>>1) + (b<<1);
    return n<<shift;
}

static inline ui effof12(ui n, ui shift)
    /*
     * On background: xyz -> yxz
     * Effect on edge mask: 76543210 -> 76325410
     */
{
    n >>= shift;
    ui a,b;
    a = n&0x0c;
    b = n&0x30;
    n -= a + b;
    n += (a<<2) + (b>>2);
    return n<<shift;
}

static inline ui effof201(ui n,ui shift,ui inv)
{
    /*
     * !inv on background: xyz -> zxy
     * Effect on edge mask: 76543210 -> 73625140
     */
    n >>= shift;
    ui a,b,c,d,e,f;
    a = 0x40&n;
    b = 0x20&n;
    c = 0x10&n;
    d = 0x8&n;
    e = 0x4&n;
    f = 0x2&n;
    n -= a+b+c+d+e+f;
    if(inv) n += (a>>1) + (b>>2) + (c>>3) +
        (d<<3) + (e<<2) + (f<<1);
    else n += (a>>3) + (b<<1) + (c>>2) +
        (d<<2) + (e>>1) + (f<<3);
    return n << shift;
}

ui symmetry(ui graph,ui pflip,ui cycle,ui tau,ui xorop)
{
    ui a,b,c;
    SPLIT_UP;
    switch(pflip){
        case 1: // 3210 -> 2301
            a = effof01(a,24);
            b = effof01(b,16);
            c = effof12(c,8);
            graph = effof12(graph,0);
            graph <<= 8;
            graph += (a>>8) + (b<<8) + (c>>8);
            break;
        case 2: // 3210 -> 1032
            a = effof201(a,24,1);
            b = effof201(b,16,1);
            c = effof201(c,8,0);
            graph = effof201(graph,0,0);
            graph <<= 16;
            graph += (a>>16) + (b>>16) + (c<<16);
            break;
        case 3: // 3210 -> 0123
            a = effof01(effof201(a,24,0),24);
            b = effof01(effof201(b,16,0),16);
            c = effof01(effof201(c,8,0),8);
            graph = effof01(effof201(graph,0,0),0);
            graph <<= 24;
            graph += (a>>24) + (b>>8) + (c<<8);
            break;
        default: // identity
            graph += a + b + c;
            break;
    }

    SPLIT_UP;
    switch(cycle){
        case 1: // 3210 -> 3021
            a=effof201(a,24,0);
            b=effof01(b,16);
            c=effof01(c,8);
            graph <<= 16;
            graph += a + (b>>8) + (c>>8);
            break;
        case 2: // 3210 -> 3102
            a=effof201(a,24,1);
            c=effof01(c,8);
            graph=effof01(graph,0);
            graph <<= 8;
            graph += a + (b>>16) + (c<<8);
            break;
        default: // identity
            graph += a + b + c;
    }

    SPLIT_UP;
    if(tau){ // 3210 -> 3201
        a = effof01(a,24);
        b = effof01(b,16);
        graph <<= 8;
        graph += (c>>8) + a + b;
    }
    else graph += a + b + c; // identity

    ui d;
    if(xorop&0x1){
        SPLIT_UP;
        a >>= 24;
        d = a&0x55;
        a -= d;
        a = (a>>1) + (d<<1);
        a <<= 24;
        b >>= 16;
        d = b&0x55;
        b -= d;
        b = (b>>1) + (d<<1);
        b <<= 16;
        c >>= 8;
        d = c&0x55;
        c -= d;
        c = (c>>1) + (d<<1);
        c <<= 8;
        graph ^= 0xff;
        graph += a + b + c;
    }
    if(xorop&0x2){
        SPLIT_UP;
        a >>= 24;
        d = a&0x33;
        a -= d;
        a = (a>>2) + (d<<2);
        a <<= 24;
        b >>= 16;
        d = b&0x33;
        b -= d;
        b = (b>>2) + (d<<2);
        b <<= 16;
        c ^= 0xff00;
        d = graph&0x55;
        graph -= d;
        graph = (graph>>1) + (d<<1);
        graph += a + b + c;
    }
    if(xorop&0x4){
        SPLIT_UP;
        a >>= 24;
        d = a&0x0f;
        a -= d;
        a = (a>>4) + (d<<4);
        a <<= 24;
        b ^= 0x00ff0000;
        c >>= 8;
        d = c&0x33;
        c -= d;
        c = (c >> 2) + (d << 2);
        c <<= 8;
        d = graph&0x33;
        graph -= d;
        graph = (graph >> 2) + (d << 2);
        graph += a + b + c;
    }
    if(xorop&0x8){
        SPLIT_UP;
        a ^= 0xff000000;
        b >>= 16;
        d = b&0x0f;
        b -= d;
        b = (b>>4) + (d<<4);
        b <<= 16;
        c >>= 8;
        d = c&0x0f;
        c -= d;
        c = (c>>4) + (d<<4);
        c <<= 8;
        d = graph&0x0f;
        graph -= d;
        graph = (graph >> 4) + (d<<4);
        graph += a + b + c;
    }
    return graph;
}

#ifdef TESTING_b534ff4f
enum group_op_type {
    PFLIP,
    CYCLE,
    TAU,
    XOROP
};

int main(int argc, char *argv[])
{
    enum group_op_type type;
    char *endptr;
    int Case,n;

    if(argc < 2)
    {
        fprintf(stderr,"%s\n",
                "Need group operation type");
        exit(1);
    }

    if(strcmp(argv[1],"pflip")==0)
        type = PFLIP;
    else if(strcmp(argv[1],"cycle")==0)
        type = CYCLE;
    else if(strcmp(argv[1],"tau")==0)
        type = TAU;
    else if(strcmp(argv[1],"xorop")==0)
        type = XOROP;
    else
    {
        fprintf(stderr,"%s\n","Undefined group op type");
        exit(1);
    }

    if(argc < 3 || argv[2][0] == '\0')
    {
        fprintf(stderr,"%s\n","Op case required");
        exit(1);
    }

    if(argc < 4 || argv[3][0] == '\0')
    {
        fprintf(stderr,"%s\n","Integer required");
        exit(1);
    }

    Case = strtol(argv[2],&endptr,0);
    if(*endptr != '\0')
    {
        fprintf(stderr,"%s: %s\n","Invalid case value",argv[2]);
        exit(1);
    }

    n = strtol(argv[3],&endptr,0);
    if(*endptr != '\0')
    {
        fprintf(stderr,"%s: %s\n","Invalid graph value",argv[3]);
        exit(1);
    }

    switch(type){
        case PFLIP:
            printf("0x%08x\n",symmetry(n,Case % 4,0,0,0));
            break;
        case CYCLE:
            printf("0x%08x\n",symmetry(n,0,Case % 3,0,0));
            break;
        case TAU:
            printf("0x%08x\n",symmetry(n,0,0,Case % 2,0));
            break;
        case XOROP:
            printf("0x%08x\n",symmetry(n,0,0,0,1 << (Case % 4)));
            break;
    }
}
#endif
