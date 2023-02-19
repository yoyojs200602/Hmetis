#pragma once

#include "Hypergraph.hpp"
#include "../parsing.hpp"
#include <string>
#include <stack>
#include <vector>
#include <string>
#include <list>

class Hmetis
{
    private:
        Hypergraph* graph;
        std::list<LayerInfo*> Instances;
        std::vector<double> spaceLimit;
        std::vector<std::vector<std::pair<int, std::vector<int>*>>*> coarsenInfo;

    public:
        Hmetis() = default;

        // int uncoarCnt = 0;
        // multiset<pair<double, pair<int, int>>> sizeOrder;

        // build graph by the given box
        void configureGraph(BOX &box);

        // output partitioned graph to the given box
        void outputGraph(BOX &box);
        
        // hmetis coarsening
        // bool for if the coarsening is "restricted"
        void coarsen(int, bool, std::string, int);

        // hmetis initial partitioning
        // int for the number of Instances desired
        // void initialPartition(int, Hypergraph&);
        void initialPartition(int, std::string);

        // hmetis uncoarsening
        // double stands for 'dropRate'
        void uncoarsen(double, std::string);

        // new_partition uncoarsening
        void newUncoarsen();

        // hmetis refine
        void refine(int, double, std::string, std::string, int);

        // return the size of instances[]
        int getInstanceSize();

        // drop instances which are 'dropRate' worse than the best cut (0.1 stands for 10 percent)
        void dropWorstCut(double);

        void chooseBestInstance();

        // return sorted node idx in non-decreasing node size order
        std::vector<int> sortNode(Hypergraph*);

        // reduce total terminal size to fit the limit in initial partition
        void enableTerminalSizeLimit(Hypergraph*, std::vector<int>&);

        void preIPAdjustment();
};

struct partition_info
{
    Hypergraph* graph;
    LayerInfo* inst;
    std::vector<double> spaceLimit;
    std::string scheme;

    partition_info(Hypergraph* graph,
                    LayerInfo* inst,
                    std::vector<double> spaceLimit,
                    std::string scheme): graph(graph), inst(inst), spaceLimit(spaceLimit), scheme(scheme) {}
};