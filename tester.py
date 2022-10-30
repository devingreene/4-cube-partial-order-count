#!/usr/bin/env python3
""" Testing for graph transformations in symmetries.c """
import random
import subprocess

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

def transform(graph:dict,f)->dict:
    """Transforms graph with function f.
    No checking for graph validity is done."""
    outgraph = {}
    for src in graph:
        outgraph.update({f(src):{f(n) for n in graph[src]}})
    return outgraph

def pflip(graph:dict,case:int)->dict:
    if case == 1:
        def f(k):
            return 2*(k&0x5) + (k&0xa)//2
    elif case == 2:
        def f(k):
            return 4*(k&0x3) + (k&0xc)//4
    elif case == 3:
        def f(k):
            k = 2*(k&0x5) + (k&0xa)//2
            return 4*(k&0x3) + (k&0xc)//4
    else:
        raise ValueError("Invalid case")
    return transform(graph,f)

def cycle(graph:dict,case:int)->dict:
    if case == 1:
        def f(k):
            kh,km,kl = k&0x8,k&0x6,k&0x1
            km //= 2
            kl *= 4
            return kh + km + kl
    elif case == 2:
        def f(k):
            kh,km,kl = k&0x8,k&0x4,k&0x3
            km //= 4
            kl *= 2
            return kh + km + kl
    else:
        raise ValueError("Invalid case")
    return transform(graph,f)

def tau(graph:dict,case:int)->dict:
    if case == 1:
        def f(k):
            kh,km,kl = k&0xc,k&0x2,k&0x1
            km //= 2
            kl *= 2
            return kh + km + kl
    else:
        raise ValueError("Invalid case")
    return transform(graph,f)

def xorop(graph:dict,case:int)->dict:
    if not 0 <= case <= 3:
        raise ValuError("Invalid case")
    return transform(graph,lambda x:x^2**case)

if __name__ == '__main__':

    def report_error(opname,case,n,alt1,alt2):
        print("""Test failed at op {}, case {}
    For start string 0x{:08x}: {} != {}""".
                format(opname,case,n,alt1,alt2))

    samples = [
            0x00000000,
            0xeeeeeeee,
            0x77777777,
            0xf8f8f8f8,
            0xcacacaca,
            0x018c018c,
            0xaefec810,
            0x12341234,
            0x12345678,
            0xfedcba98,
            random.randrange(2**32), # One random graph
            ]

    opnames = ["pflip", "cycle", "tau", "xorop"]
    ops = [pflip, cycle, tau, xorop]
    casesets = [[1,2,3], [1,2], [1], [0,1,2,3]]
    error = False
    for n in samples:
        graph = int2graph(n)
        for opname,op,cases in zip(opnames,ops,casesets):
            for case in cases:
                # C routine
                cproc = subprocess.Popen(
                        ["./unittest",str(opname),str(case),str(n)],
                        stdin=subprocess.DEVNULL,
                        stdout=subprocess.PIPE)
                alt1 = cproc.communicate()[0].decode().strip()
                # Python routine
                alt2 = "0x{:08x}".format(graph2int(op(graph,case)))
                try:
                    assert ( alt1 == alt2 or report_error(opname,case,n,alt1,alt2) )
                except AssertionError:
                    error = True
    if not error:
        print("All tests passed!")
