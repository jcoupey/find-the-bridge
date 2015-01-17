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

  unsigned subgraphs_size = 10;

  const char* optString = "s:h?";
  
  int opt = getopt(argc, argv, optString);

  while(opt != -1) {
    switch(opt) {
    case 's':
      subgraphs_size = std::strtol(optarg, nullptr, 10);
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

  unsigned start, end;
  start = 1;
  end = 12;

  std::list<unsigned> path = g.shortest_path(start, end);

  std::cout << "*******************\n"
            << "Shortest path between "
            << start << " and " << end << ": "
            << std::endl;
  
  for(auto v = path.begin(); v != path.end(); v++){
    std::cout << *v << " ; ";
  }
  
  std::cout << "weight = " << g.path_weight(path) << std::endl;
}
