#include <iostream>
#include <set>
#include <list>
#include <chrono>
#include <random>
#include <unistd.h>
#include "undirectedgraph.h"
#include "exceptions.h"

void display_usage()
{
  std::string usage = "Usage : mwis [-s size]\n";
  usage += "Option:\n";
  usage += "  -s=size\t\t size of the subgraphs\n"; 
  std::cout << usage
            << std::endl;
  exit(0);
}

int main(int argc, char **argv){

  // Parsing command-line arguments
  struct globalArgs_t {
    // -e option
    bool example;
    // -r option
    unsigned random_subgraphs_size;
  } globalArgs;

  globalArgs.example = false;
  globalArgs.random_subgraphs_size = 0;

  const char* optString = "er:h?";
  
  int opt = getopt(argc, argv, optString);

  while(opt != -1) {
    switch(opt) {
    case 'e':
      globalArgs.example = true;
      break;
    case 'r':
      globalArgs.random_subgraphs_size = std::strtol(optarg, nullptr, 10);
      break;
    case 'h': 
    case '?':
      display_usage();
      break;
    default:
      break;
    }
    opt = getopt(argc, argv, optString);
  }

  if(globalArgs.example){
    // Basic example

    UndirectedGraph g;

    // First subgraph
    g.add_vertex(1);
    g.add_vertex(2);
    g.add_vertex(3);
    g.add_vertex(4);
    g.add_vertex(5);
    g.add_vertex(6);
  
    g.add_edge(1, 2, 2);
    g.add_edge(1, 4, 4);
    g.add_edge(1, 6, 3);
    g.add_edge(2, 3, 2);
    g.add_edge(2, 6, 1);
    g.add_edge(3, 4, 6);
    g.add_edge(3, 5, 4);
    g.add_edge(3, 6, 1);
    g.add_edge(4, 5, 3);

    // Second subgraph
    g.add_vertex(7);
    g.add_vertex(8);
    g.add_vertex(9);
    g.add_vertex(10);
    g.add_vertex(11);
    g.add_vertex(12);

    g.add_edge(7, 8, 2);
    g.add_edge(7, 9, 5);
    g.add_edge(7, 12, 7);
    g.add_edge(8, 9, 4);
    g.add_edge(8, 10, 2);
    g.add_edge(9, 11, 6);
    g.add_edge(10, 11, 4);
    g.add_edge(10, 12, 5);
    g.add_edge(11, 12, 8);

    // The brigde
    g.add_edge(3, 9, 10);
  
    g.log();

    // Log bridge search
    unsigned first = 1;
    unsigned second = 12;

    std::pair<unsigned, unsigned> bridge = g.find_the_bridge(first, second);

    std::cout << "*******************\n"
              << "Using vertices "
              << first << " and " << second
              << ", found bridge: "
              << std::get<0>(bridge)
              << " <-> "
              << std::get<1>(bridge)
              << std::endl;
  }

  if(globalArgs.random_subgraphs_size >= 4){
    // Building a random graph with two connected subgraphs linked by
    // a single edge

    unsigned size = globalArgs.random_subgraphs_size;
    
    unsigned max_edge_weight = 50;
    
    // Random generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    // Random distribution for edges weight
    std::uniform_int_distribution<int> weight_distribution(1, max_edge_weight);

    UndirectedGraph g2;

    // Building a first sub-graph: vertices with id from 1 to "size"
    for(int i = 1; i <= size; i++){
      g2.add_vertex(i);
    }

    // Adding at least 3 edges in the first subraph from each vertex
    std::uniform_int_distribution<int> first_subgraph_dist(1, size);
    for(int i = 1; i <= size; i++){
      std::list<unsigned> neighbours;
      while(neighbours.size() < 3){
        unsigned potential_neighbour = first_subgraph_dist(generator);
        if(potential_neighbour != i){
          neighbours.push_back(potential_neighbour);
        }
      }
      for(auto neighbour = neighbours.cbegin();
          neighbour != neighbours.cend();
          ++neighbour){
        g2.add_edge(i,
                    *neighbour,
                    weight_distribution(generator)
                    );
      }
    }
    
    // Building a second sub-graph: vertices with id from "size + 1"
    // to "2 * size"
    for(int i = size + 1; i <= 2 * size; i++){
      g2.add_vertex(i);
    }

    // Adding at least 3 edges in the second subraph from each vertex
    std::uniform_int_distribution<int> second_subgraph_dist(size + 1, 2 * size);
    for(int i = size + 1; i <= 2 * size; i++){
      std::list<unsigned> neighbours;
      while(neighbours.size() < 3){
        unsigned potential_neighbour = second_subgraph_dist(generator);
        if(potential_neighbour != i){
          neighbours.push_back(potential_neighbour);
        }
      }
      for(auto neighbour = neighbours.cbegin();
          neighbour != neighbours.cend();
          ++neighbour){
        g2.add_edge(i,
                    *neighbour,
                    weight_distribution(generator)
                    );
      }
    }
    
    g2.log();

    // Using random vertices to define the bridge (unique edge between
    // the two subgraphs)
    unsigned bridge_first = first_subgraph_dist(generator);
    unsigned bridge_second = second_subgraph_dist(generator);
    g2.add_edge(bridge_first,
               bridge_second,
               weight_distribution(generator)
               );
    std::cout << "*******************\n"
              << "Random subgraph built with bridge: "
              << bridge_first
              << " <-> "
              << bridge_second
              << std::endl;
    
    // Log bridge search

    // Using random vertices for bridge search, one in each subgraph
    unsigned first = first_subgraph_dist(generator);
    unsigned second = second_subgraph_dist(generator);

    std::pair<unsigned, unsigned> bridge = g2.find_the_bridge(first, second);

    std::cout << "*******************\n"
              << "Using vertices "
              << first << " and " << second
              << ", found bridge: "
              << std::get<0>(bridge)
              << " <-> "
              << std::get<1>(bridge)
              << std::endl;

  }

}
