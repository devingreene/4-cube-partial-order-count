# Building

For best results (and least irritation), create a directory or folder with a name of your choosing, and clone this repository in that directory.  The following command executed with your new directory will build, along with object files, the binary of interest: <tt>printAllIsoPoz3</tt>  (Note, '$' is the shell prompt.) 

<tt>$ make</tt>

To clean up the object files as well as the binaries, do this.

<tt>$ make clean</tt>

Unit testing on the group operations used in the main routine can be done as follows.

<tt>$ make test</tt>
<tt>$ ./tester.py

# What This Software Computes

Consider the set of cube-shaped directed graphs, with nodes at the vertices and arrows at the edges.  We say that a graph is <i>acyclic</i> if there is no path of non-zero length along the edges which respects the direction of the arrows and returns to the starting point of the path.  We say that two graphs are isomorphic if one can be transformed to the other via a cube isometry [1].  To output a complete list of acyclic directed graphs, with only one member from each isomorphism class, enter the following command in a shell. 

<tt>$ ./printAllIsoPoz4</tt>

Please read the comments in <tt>main.c</tt> for an explanation of the encoding of these graphs.

This project was motivated mostly by a similar analysis done with the 3-cube in relation to [2] (see [3]).

[1] Coxeter, Harold Scott MacDonald (1973). <i>Regular Polytopes</i>. Courier Corporation.

[2] Kristina Crona, Alex Gavryushkin, Devin Greene, Niko Beerenwinkel (2018) Inferring genetic interactions from comparative fitness data.  eLife 2017;6:e28629 doi: 10.7554/eLife.28629 

[3] https://github.com/devingreene/3-cube-partial-order-count.git
