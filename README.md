# find-the-bridge

Solving the following problem.

> A given graph consists of two connected sub-graphs in which every
> node has at least three neighbours. The sub-graphs are only
> connected by a single edge, the bridge. Given a node in each
> sub-graph, find the bridge. You can assume there is only one bridge.

## Algorithm

### Description

Let's denote by _u_ and _v_ the vertices given in the different sub-graphs.

#### Step 1: finding a path _p_ between _u_ and _v_

There is no condition on the path-finding algorithm used here. The
next steps will be valid no matter the form of _p_.

Note that:

* we are assured that such a path exists ;

* it **must** contain the bridge we're seeking because of its uniqueness.

#### Step 2: removing edges in _p_ from the graph

The specificity of the bridge is that when removed, the two sub-graphs
are no more connected. Since the seeked bridge is contained in _p_, we
now obtain two connected components corresponding to the now isolated
sub-graphs.

Note that each sub-graph stay itself connected even when removing the part of
_p_ it contains. This is because each vertex has at least 3 neighbours
(may be proved by induction on the size of the sub-graph).

#### Step 3: dichotomic search of the bridge using the vertices in _p_

Let _m_ be the vertex on _p_ that is "in the middle" between _u_ and
_v_. There are two alternatives :

1. There is a path between _u_ and _m_: in that case, they both are in
the same sub-graph (remember the bridge has been removed). So the
bridge is now to be found between _m_ and _v_.

2. There is no path between _u_ and _m_: in that case, they must be in
separate sub-graphs (because sub-graphs themselves remained connected,
see above). So the bridge is now to be found between _u_ and _m_.

This step starts with the list of vertices of path _p_, and the
updated list's length is divided by 2 at each iteration. The search
stops when it only contains two vertices, wich are apart the bridge.

### Notes on implementation

As previously said, any path would do in step 1. The implementation
here uses undirected weighted graphs, and a simple Dijkstra algorithm
for shortest path (in term of weight) is implemented. But if we really
want to shorten the third step, we'd better use a shortest path in
term of number of edges. So a breadth-first search is used for
path-finding in step 1 (and also for testing whether two vertices are
connected in step 3).

Step 2 removes edges in _p_ for convenience, one could also leave the
graph alone and implement the path search used in step 3 with an
additional argument forbidding to use any edge in _p_.

## Complexity

If _|V|_ and _|E|_ denote the number of vertices and edges in the
graph, then breadth-first search of the path in step 1 is performed in
_O(|V| + |E|)_.

Step 2 is linear in _length(p)_, this length being bounded by the
diameter of the graph, in any case smallest than _|E|_.

Step 3 requires something like _log(length(p))_ iterations, so less
than _log(|E|)_. Each of theses iterations consist of a breadth-first
search, again _O(|V| + |E|)_.

In any case, the algorithm runs in _O(log(|E|) x (|V| + |E|))_.

## Usage

### Build

To build the executable, run:

```bash
mkdir bin && cd src/ && make && cd ..
```

### Handmade example

Toy around with an example built from scratch:

```bash
./bin/find-the-bridge -e
```

See ```main``` file to modify graph and vertices used for bridge
search.

### Solving random examples

Use:

```bash
./bin/find-the-bridge -r size
```

to build a random graph matching the above description (see
introduction) on wich the bridge search is performed. 

The two subgraphs contain each ```size``` vertices.


