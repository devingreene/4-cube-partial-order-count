from sage.graphs.graph import DiGraph

def convert_encoding_to_DiGraph(n):
    ''' Convert an encoded 4-cube graphs ( as in 
https://github.com/devingreene/4-cube-partial-order-count ) to a 
sage DiGraph'''
    
    edges = []
    for i in range(32):
        n,bit = divmod(n,2)
        locus,bkground = divmod(i,8)
        # Switch from right-to-left to left-to-right 
        # for list comp below
        lr_locus = 3 - locus
        bkground = '{:03b}'.format(bkground)
        new_edge = [ bkground[:lr_locus] + x + bkground[lr_locus:] \
                for x in bit and ['1','0'] or ['0','1']]
        edges.append(new_edge)

    return DiGraph(edges)

def numpeaks(n):
    ''' Counts the number of peaks in the graph '''
    return len(convert_encoding_to_DiGraph(n).sinks())

def display_graph(g):
    ''' Displays graph in Nina Crona's preferred format'''

    pos = {'0000':(0,-2),
           '0001':(1.5,-1),
           '0010':(0.5,-1),
           '0100':(-0.5,-1),
           '1000':(-1.5,-1),
           '1100':(-2.5,0),
           '1010':(-1.5,0),
           '1001':(-0.5,0),
           '0110':(0.5,0),
           '0101':(1.5,0),
           '0011':(2.5,0),
           '1110':(-1.5,1),
           '1101':(-0.5,1),
           '1011':(0.5,1),
           '0111':(1.5,1),
           '1111':(0,2)}

    g.show(pos=pos)
