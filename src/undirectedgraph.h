#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include <iostream>
#include <algorithm>
#include <set>
#include <list>
#include <map>
#include <array>
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
    unsigned _first_vertex_id;
    unsigned _second_vertex_id;
    unsigned _weight;
    
    Edge(unsigned first_vertex_id,
         unsigned second_vertex_id,
         unsigned weight):
      _first_vertex_id(std::min(first_vertex_id, second_vertex_id)),
      _second_vertex_id(std::max(first_vertex_id, second_vertex_id)),
      _weight(weight) {}
    
    // Operator used in a set<Edge>
    bool operator<(const Edge& rhs) const{
      return (this->_first_vertex_id < rhs._first_vertex_id)
        or ((this->_first_vertex_id == rhs._first_vertex_id)
            and (this->_second_vertex_id < rhs._second_vertex_id));
    };

    // Print edge information
    void log() const{
      std::cout << _first_vertex_id << "<--" << _weight << "-->"
                << _second_vertex_id
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

  bool has_vertex(unsigned vertex_id) const;

  std::list<unsigned> neighbours_for_vertex(unsigned vertex_id) const;

  void add_edge(unsigned first_vertex_id,
                unsigned second_vertex_id,
                unsigned weight);

  void remove_edge(unsigned first_vertex_id, unsigned second_vertex_id);

  bool has_edge(unsigned first_vertex_id, unsigned second_vertex_id) const;
  
  void log() const;
};

#endif