Consider the class of directed graph structures on the four
dimensional hypercube, Q4; where the nodes are the vertices and the
edges are, well, the edges of Q4.  Two such directed graphs are
*isomorphic* if one can be rotated into the other where the directions
of the edges match.  By "rotation", we are including all the elements
of the group of symmetries on Q4, of which there are 384 in total
(cf. Coxeter, Regular Polytopes, 1973 Courier Corporation).  Any list
of directed graphs in Q4 is considered redundant if it contains
two or more isomorphic graphs.  

This codes therefore does something very simple.  It produces an
exhaustive *non-redundant* list of directed graphs on Q4.  The list
produced consists of 8 digit hexadecimal numbers.  These should be
interpreted as a sequence of bits.  How these bit sequences are to be
interpreted as directed graphs on Q4 is detailed in the comments in the
file `main.c'
