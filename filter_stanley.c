/* Count partial orders on the 4-cube.  The label ``stanley'' refers
 * to the fact the number can be obtained using a formula derived by
 * Richard Stanley.  This program can be thought of as a ``brute
 * force'' verification of Stanley's result for the 4-cube.
 */

#include"headers.h"
#include<assert.h>

#define G 384 // Cardinality of 4-cube group

extern void int2table(ui n);
extern int IsPoset(void);
extern ui symmetry(ui graph,uch pflip,uch tcycle,uch tau,uch xorop);
extern uch *checklist;
ui graph;

int IsBaseSource(ui graph){
    return !(0x01010101&graph);
}

/* We loop over graphs with 0000 as a source.  So we can encode our graphs
   with 28 bit words.  The following two functions convert back and forth
   from the 28 bit representation to the 32 bit one.*/
ui fixbp2source(ui graph){
    ui a,b,c;
    a=0xfe00000&graph;
    b=0x1fc000&graph;
    c=0x3f80&graph;
    graph &= 0x7f;
    a <<= 4;
    b <<= 3;
    c <<= 2;
    graph <<= 1;
    return graph + a + b + c;
}

ui invfixbp2source(ui graph){
    ui a,b,c;
    a=0xff000000&graph;
    b=0x00ff0000&graph;
    c=0x0000ff00&graph;
    graph &= 0x000000ff;
    a >>= 4;
    b >>= 3;
    c >>= 2;
    graph >>= 1;
    return graph + a + b + c;
}


void filter (ui start, ui end){
    ui Graph,tgraph;
    /* Flip through all symmetries.  We use the following
     * observations from basic group theory:
     *
     *  - Orbits form a partition of the set of graphs
     *  - The cardinality of an orbit is |G|/|fixing subgroup|
     *
     *  We use a hash table checklist.  If a graph has already been
     *  seen, we skip it.
     */

    ui count = 0;
    ui nfge; // number of fixing group elements
    for(graph=start;graph<end;graph++){
        if(likely(isset(graph))) continue;
        Graph = fixbp2source(graph);
        int2table(Graph);
        if(likely(!IsPoset())) continue;

        /* flip through all symmetries */
        uch pflip,tcycle,tau,xorop;
        nfge = 0;
        for(pflip=0;pflip<4;pflip++)
            for(tcycle=0;tcycle<3;tcycle++)
                for(tau=0;tau<2;tau++)
                    for(xorop=0;xorop<16;xorop++){
                        tgraph = symmetry(Graph,pflip,tcycle,tau,xorop);
                        nfge += ( tgraph == Graph );
                        if(unlikely(IsBaseSource(tgraph))){
                            tgraph = invfixbp2source(tgraph);
                            if(tgraph > graph)
                                setbit(tgraph);
                        }
                    }
        assert(G % nfge == 0);
        count += G/nfge;
    }
    printf("%u\n",count);
}
