''' From https://cocalc.com/projects/c67c7038-6d14-4820-8f06-ba84fd2ff8e8/files/epistasis_and_posets_public.sagews'''

## Note: The current author needed to apply some patches to the code below, 
## since in its original version, it did not produce correct results.  The
## author cited above has been notified of the situation.

## My import statement
from sage.graphs.graph import DiGraph

## A.G.'s work starts here
#Our efficient implementation for checking whether a partial order implies epistasis, and its helper functions
#input: a directed graph, two labels, and a partition
#output: a bipartite graph whose verties are the elments of the original directed graph. the partition in the graph is between vertices labeled with letter 1 and letter 2. there is an edge between vertex u labeled with letter1 and vertex v labeled with letter2 if there is a directed path from u to v
def one_sided_comparability_bigraph(digraph, letter1, letter2, partition):
    adjacency = {}
    for vertex in digraph.vertices():
        adjacency[vertex]=[]
        if partition[vertex] == letter1:
            reachable = list(digraph.breadth_first_search(vertex))
            neighbors = []
            for neighbor in reachable:
                if partition[neighbor]==letter2:
                    neighbors.append(neighbor)
            adjacency[vertex]=neighbors
    return DiGraph(adjacency)

#input: a graph, a partition, and the partition labels
#output: true if the graph has a perfect matching and false otherwise
def check_matching(graph, partition, letter1, letter2):
    copy = DiGraph(graph)
    size = len(graph.vertices())/2
    for vertex in copy.vertices():
        if partition[vertex]==letter1:
            copy.add_edge('s', vertex)
        if partition[vertex]==letter2:
            copy.add_edge(vertex,'t')
    max_flow = int(copy.flow('s','t'))
    if size == max_flow:
        return True

#input: a directed graph, a partition on the vertices, and the labels used in the partition
#output: "positive epistasis implied", "negative epistasis implied", "no epistasis implied" 
def quick_epistasis_check(digraph, partition, letter1, letter2):
    bigraph1 = one_sided_comparability_bigraph(digraph, letter1, letter2, partition)
    if check_matching(bigraph1, partition, letter1, letter2):
        return 1
    bigraph2 = one_sided_comparability_bigraph(digraph, letter2, letter1, partition)
    if check_matching(bigraph2, partition, letter2, letter1):
        return -1
    else:
        return 0
