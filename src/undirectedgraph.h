#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include <iostream>
#include <algorithm>
#include <set>
#include <list>
#include <map>
#include <limits>
#include "exceptions.h"

class UndirectedGraph{
private:
  // Vertex description
  class Vertex{
  public:
    unsigned _degree;                // vertex degree in the graph

    Vertex():
      _degree(0){}

    // Print vertex information
    void log() const{
      std::cout << "Degree: " << _degree;
    };
  };
  
  // Edge description
  class Edge{
  public:
    unsigned _first_vertex;
    unsigned _second_vertex;
    unsigned _weight;
    
    Edge(unsigned first_vertex,
         unsigned second_vertex,
         unsigned weight):
      _first_vertex(std::min(first_vertex, second_vertex)),
      _second_vertex(std::max(first_vertex, second_vertex)),
      _weight(weight) {}
    
    // Operator used in a set<Edge>
    bool operator<(const Edge& rhs) const{
      return (this->_first_vertex < rhs._first_vertex)
        or ((this->_first_vertex == rhs._first_vertex)
            and (this->_second_vertex < rhs._second_vertex));
    };

    // Print edge information
    void log() const{
      std::cout << _first_vertex << "<--" << _weight << "-->"
                << _second_vertex
                << " ; ";
    };
  };

  std::map<unsigned, Vertex> _vertices; // graph vertices mapped with
                                        // their ids in the graph

  std::set<Edge> _edges;                // set of graph edges

public:

  UndirectedGraph();

  unsigned number_of_vertices() const;
  
  unsigned number_of_edges() const;

  void add_vertex(unsigned id);

  void remove_vertex(unsigned id);

  bool has_vertex(unsigned vertex) const;

  std::list<unsigned> neighbours_for_vertex(unsigned vertex) const;

  void add_edge(unsigned first_vertex,
                unsigned second_vertex,
                unsigned weight);

  void remove_edge(unsigned first_vertex, unsigned second_vertex);

  bool has_edge(unsigned first_vertex,
                unsigned second_vertex) const;

  bool are_connected(unsigned first_vertex,
                     unsigned second_vertex) const;

  unsigned edge_weight(unsigned first_vertex,
                       unsigned second_vertex) const;

  unsigned path_weight(std::list<unsigned> path) const;

  std::list<unsigned> shortest_path(unsigned first_vertex,
                                    unsigned second_vertex) const;

  std::list<unsigned> smallest_path(unsigned first_vertex,
                                    unsigned second_vertex) const;

  void log() const;
};

#endif
