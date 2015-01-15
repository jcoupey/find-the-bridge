#include "undirectedgraph.h"

UndirectedGraph::UndirectedGraph(){}

unsigned UndirectedGraph::number_of_vertices() const{
  return _vertices.size();
};

unsigned UndirectedGraph::number_of_edges() const{
  return _edges.size();
};

void UndirectedGraph::add_vertex(unsigned id){
  // Nothing done if some vertex already have this id
  _vertices.emplace(id, Vertex{});
};

void UndirectedGraph::remove_vertex(unsigned id){
  auto target_vertex = _vertices.find(id);
  if(target_vertex != _vertices.end()){
    // Removing all edges to neighbours of given vertex
    std::list<unsigned> neighbours = this->neighbours_for_vertex(id);
    for(auto vertex = neighbours.begin();
        vertex != neighbours.end();
        vertex++){
      Edge target_edge (id, *vertex, 0);
      _edges.erase(target_edge);
      // Updating neighbour degree
      _vertices.find(*vertex)->second._degree--;
    }
    
    // Removing vertex
    _vertices.erase(target_vertex);
  }
};

bool UndirectedGraph::has_vertex(unsigned vertex_id) const{
  return _vertices.find(vertex_id) != _vertices.end();
};

std::list<unsigned> UndirectedGraph::neighbours_for_vertex(unsigned vertex_id) const{
  if(! this->has_vertex(vertex_id)){
    throw ArgsErrorException("Not a valid vertex id!");
  }
  std::list<unsigned> neighbours;
  for(auto vertex_pair = _vertices.cbegin();
      vertex_pair != _vertices.cend();
      vertex_pair++){
    if(this->has_edge(vertex_id, vertex_pair->first)){
      neighbours.push_back(vertex_pair->first);
    }
  }
  return neighbours;
};

void UndirectedGraph::add_edge(unsigned first_vertex_id,
                               unsigned second_vertex_id,
                               unsigned weight){
  if(first_vertex_id == second_vertex_id){
    return;
  }
  auto first_vertex_pair = _vertices.find(first_vertex_id);
  auto second_vertex_pair = _vertices.find(second_vertex_id);
  if((first_vertex_pair != _vertices.end())
     and (second_vertex_pair != _vertices.end())){
    // Only if given ids are valid vertices ids
    _edges.emplace(first_vertex_id, second_vertex_id, weight);
    // Updating vertices degrees
    first_vertex_pair->second._degree++;
    second_vertex_pair->second._degree++;
  }
};

void UndirectedGraph::remove_edge(unsigned first_vertex_id,
                                  unsigned second_vertex_id){
  // Removing edge
  Edge target_edge (first_vertex_id, second_vertex_id, 0);
  if(_edges.erase(target_edge)){
    // Updating degrees if erase is successful (an edge really exists
    // between the vertices)
    auto first_vertex_pair = _vertices.find(first_vertex_id);
    auto second_vertex_pair = _vertices.find(second_vertex_id);
    first_vertex_pair->second._degree--;
    second_vertex_pair->second._degree--;
  }
};

bool UndirectedGraph::has_edge(unsigned first_vertex_id,
                               unsigned second_vertex_id) const{
  Edge target_edge (first_vertex_id, second_vertex_id, 0);
  auto target = _edges.find(target_edge);
  return target != _edges.end();
};

void UndirectedGraph::log() const{
  std::cout << "****************** Graph log ******************\n"
    << "* Vertices:\n";
  for(auto vertex = _vertices.begin(); vertex != _vertices.end(); vertex++){
    std::cout << "** Id: "
              << vertex->first
              << std::endl
              << "*** ";
    vertex->second.log();
    std::cout << std::endl;
  }
  std::cout << "* Edges:\n";
  for(auto edge = _edges.begin(); edge != _edges.end(); edge++){
    edge->log();
  }
  std::cout << std::endl;
};
