Hypergraph_PATH = ./src/Hypergraph.cpp
Hmetis_PATH = ./src/Hmetis.cpp
Coarsen_PATH = ./src/Coarsen/Coarsening.cpp ./src/Coarsen/EC_Coarsening.cpp ./src/Coarsen/HEC_Coarsening.cpp
Partition_Algo_PATH = ./inc/Partition_Algo/partition_algo.hpp
INCLUDE = -I ./inc -I ./inc/Coarsen
FLAG = -g -std=c++0x -pthread
TARGET = hmetis
Coarsen = Coarsening.o EC_Coarsening.o HEC_Coarsening.o
FILES_TO_LINK = main.o Hmetis.o Hypergraph.o $(Coarsen)


all: $(FILES_TO_LINK)
	g++ $(FLAG) $(FILES_TO_LINK) -o $(TARGET)
	rm *.o

main.o: main.cpp
	g++ $(FLAG) $(INCLUDE) -c main.cpp

Hmetis.o: $(Hmetis_PATH)
	g++ $(FLAG) $(INCLUDE) -c $(Hmetis_PATH)

Hypergraph.o: $(Hypergraph_PATH)
	g++ $(FLAG) $(INCLUDE) -c $(Hypergraph_PATH)

$(Coarsen): $(Coarsen_PATH)
	g++ $(FLAG) $(INCLUDE) -c $(Coarsen_PATH)

clean:
	rm $(TARGET)