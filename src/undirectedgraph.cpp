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

unsigned UndirectedGraph::edge_weight(unsigned first_vertex_id,
                                      unsigned second_vertex_id) const{
  Edge target_edge (first_vertex_id, second_vertex_id, 0);
  auto target = _edges.find(target_edge);
  if(target == _edges.end()){
    std::string message = "Not edge between vertices: "
      + std::to_string(first_vertex_id)
      + " and "
      + std::to_string(second_vertex_id);
    throw  ArgsErrorException(message);
  }
  else{
    return target->_weight;
  }
};  

unsigned  UndirectedGraph::path_weight(std::list<unsigned> path) const{
  unsigned weight = 0;

  if(path.empty()){
    return weight;
  }

  auto vertex_iter = path.cbegin();
  unsigned vertex = *vertex_iter;
  unsigned next_vertex;
  while(++vertex_iter != path.cend()){
    next_vertex = *vertex_iter;
    weight += this->edge_weight(vertex, next_vertex);

    vertex = next_vertex;
  }

  return weight;
};

std::list<unsigned> UndirectedGraph::shortest_path(unsigned first_vertex_id,
                                                     unsigned second_vertex_id) const{

  if(first_vertex_id == second_vertex_id){
    // Not really interesting
    std::list<unsigned> path ({first_vertex_id});
    return path;
  }

  // Dijkstra algorithm

  // Mapping the vertices id to the current shortest distance from
  // first_vertex_id
  std::map<unsigned, unsigned> shortest_distances;
  shortest_distances.emplace(first_vertex_id, 0);

  unsigned inf = std::numeric_limits<unsigned>::max();
  for(auto vertex = _vertices.cbegin(); vertex != _vertices.cend(); ++vertex){
    // Init to +inf when vertex has not yet been visited
    shortest_distances.emplace(vertex->first, inf);
  }

  // Set of vertices whose shortest distance from first_vertex is not
  // yet known (init with all but first_vertex)
  std::set<unsigned> vertices_to_visit;
  
  for(auto vertex = _vertices.cbegin(); vertex != _vertices.cend(); ++vertex){
    if(vertex->first != first_vertex_id){
      vertices_to_visit.insert(vertex->first);
    }
  }

  // Remembering previous vertex on current shortest path
  std::map<unsigned, unsigned> previous_vertex;

  // Starting with neighbours of first_vertex
  std::list<unsigned> current_neighbours;

  current_neighbours = this->neighbours_for_vertex(first_vertex_id);

  for(auto neighbour = current_neighbours.cbegin();
      neighbour != current_neighbours.cend();
      ++neighbour){
    unsigned current_id = *neighbour;
    // Storing shortest distance
    shortest_distances.find(current_id)->second = 
      this->edge_weight(first_vertex_id, current_id);
  
    // Storing previous vertex on shortest path
    previous_vertex.emplace(current_id, first_vertex_id);
  }

  while(! vertices_to_visit.empty()){
    // Finding vertex to visit with minimum current shortest distance
    auto vertex = vertices_to_visit.cbegin();
    unsigned current_vertex = *vertex;
    unsigned min_distance = shortest_distances.at(current_vertex);
    ++vertex;
    for(; vertex != vertices_to_visit.cend(); ++vertex){
      unsigned current_distance = shortest_distances.at(*vertex);
      if(current_distance < min_distance){
        current_vertex = *vertex;
        min_distance = current_distance;
      }
    }
    
    if(min_distance == inf
       or current_vertex == second_vertex_id){
      // No accessible vertex remains or wanted vertex is reached
      break;
    }

    // Updating shortest known distance to current_vertex neighbours
    current_neighbours = this->neighbours_for_vertex(current_vertex);
    for(auto neighbour = current_neighbours.cbegin();
        neighbour != current_neighbours.cend();
        ++neighbour){
      unsigned neighbour_id = *neighbour;
      if(vertices_to_visit.find(neighbour_id) != vertices_to_visit.end()){
        // Only if shortest distance is not yet know for neighbour
        unsigned possible_shortest_distance =
          shortest_distances.find(current_vertex)->second
          + this->edge_weight(current_vertex, neighbour_id);
        if(shortest_distances.find(neighbour_id)->second >
           possible_shortest_distance){
          // Found a better distance to neighbours
          shortest_distances.find(neighbour_id)->second =
            possible_shortest_distance;
          auto prev = previous_vertex.find(neighbour_id);
          if(prev != previous_vertex.end()){
            // Updating previous
            prev->second = current_vertex;
          }
          else{
            // Adding previous for this vertex
            previous_vertex.emplace(neighbour_id, current_vertex);
          }
        }
      }
    }
    // Removing current vertex (its shortest distance from first_vertex is
    // final in shortest_distances)
    vertices_to_visit.erase(current_vertex);
  }
  
  // Recomposing path from the end
  std::list<unsigned> path;

  auto prev = previous_vertex.find(second_vertex_id);
  if(prev == previous_vertex.end()){
    // Argument vertices are not connected, return empty path
    return path;
  }
  
  unsigned current_vertex = second_vertex_id;
  // Adding the end
  path.push_front(current_vertex);
  
  while(current_vertex != first_vertex_id){
    // Getting previous vertex
    current_vertex = previous_vertex.find(current_vertex)->second;
    // Adding
    path.push_front(current_vertex);
  }
  
  return path;
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
