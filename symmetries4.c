#include"headers.h"

#define MAG 8

#define SPLIT_UP \
    a = 0xff000000&graph; \
    b = 0x00ff0000&graph; \
    c = 0x0000ff00&graph; \
    graph &= 0x000000ff


/* We encode four cube graphs with unsigned 32-bit integers.  This function
   performs symmetry operations on the four-cube at the encoding level */

/* TODO: Explain these functions! */

static inline ui effof01(ui n,ui shift){
    /* Effect on the encoding of swapping the 0
       and 1 bit at the genotype level */
    n >>= shift;
    ui a,b;
    a=n&0x44;
    b=n&0x22;
    n -= a+b;
    n += (a>>1) + (b<<1);
    return n<<shift;
}

static inline ui effof12(ui n, ui shift)
/* This is equivalent to conjugating 
   effof01 with effof201 */
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
        case 1: // abcd -> badc
            a = effof01(a,24);
            b = effof01(b,16);
            c = effof12(c,8);
            graph = effof12(graph,0);
            graph <<= 8;
            graph += (a>>8) + (b<<8) + (c>>8);
            break;
        case 2: // abcd -> cdab
            a = effof201(a,24,1);
            b = effof201(b,16,1);
            c = effof201(c,8,0);
            graph = effof201(graph,0,0);
            graph <<= 16; 
            graph += (a>>16) + (b>>16) + (c<<16);
            break;
        case 3: // abcd -> dcba
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
        case 1: // abcd -> adbc
            a=effof201(a,24,0);
            b=effof01(b,16);
            c=effof01(c,8);
            graph <<= 16;
            graph += a + (b>>8) + (c>>8);
            break;
        case 2: // abcd -> acdb
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
    if(tau){ // ABCD -> ABDC
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
