dim = 4
nnodes=2**dim
nedges = 32

def int2graph(n:int)->dict:
    assert 0 <= n < 2**nedges
    graph = {k:set() for k in range(2**dim)}
    for pos in range(0,nedges):
        # See description in main.c
        k,r = divmod(pos,nnodes//2)
        rl = r % 2**k
        rh = r - rl
        rh *= 2
        src = rh + rl
        dst = src + 2**k
        if n & 2**pos:
            src,dst = dst,src
        graph[src].add(dst)
    return graph

# Assumes that graph is "well-formed".
def graph2int(graph:dict)->int:
    nout = 0
    for src in graph:
        for dst in graph[src]:
            diff = src ^ dst
            for k in range(0,dim):
                if 2**k == diff:
                    pos = 8*k
                    if dst < src:
                        rl = dst % diff
                        rh = dst - rl
                        rh //= 2
                        r = rh + rl
                        assert 0 <= r <= 7
                        pos += r
                        nout += 2**pos
                    break
            else:
                raise AssertionError
    return nout

