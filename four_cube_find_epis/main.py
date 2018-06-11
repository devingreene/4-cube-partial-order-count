import sys
from gavryushkin import *
from . utils import *

def numbits_convert(arg):
    ''' e.g. 0b1010 -> 10 or v.v. '''
    if isinstance(arg,int):
        return format(arg,'04b')
    if isinstance(arg,str):
        return int(arg,base=2)
    raise Exception("numbits_convert had bad arg")

def parity(n):
    ''' Return parity of integer '''
    par = 0
    while n:
        n,r = divmod(n,2)
        par += r
    return par % 2

def has_epis(n):
    g = convert_encoding_to_DiGraph(n)
    partition = { v:parity(numbits_convert(v)) and 'O' or 'E' \
            for v in g.vertices() }
    return quick_epistasis_check(g,partition,'O','E')
          
def main():
    while True:
        try:
            istring = raw_input()
            n = int(istring,base=16)
            print istring+':'+str(has_epis(n))
        except EOFError:
            return

import os
# Horrible hack(?)
# Find out if sage is being run interactively
# If not, read stdin
if not os.getenv('TERM',False): main()
