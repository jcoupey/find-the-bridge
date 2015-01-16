objects = main.o\
	undirectedgraph.o

ftb : $(objects)
	g++ -std=c++11 -o ../bin/find-the-bridge $(objects)

undirectedgraph.o : undirectedgraph.cpp undirectedgraph.h exceptions.h
	g++ -std=c++11 -c undirectedgraph.cpp

main.o : main.cpp undirectedgraph.h exceptions.h
	g++ -std=c++11 -c main.cpp

clean:
	rm ../bin/find-the-bridge $(objects)
