#pragma once

#include "../Hypergraph.hpp"
#include <vector>

class HEC_Coarsening
{
    public:
        HEC_Coarsening() = default;

        // main function of HEC coarsening
        // MHEC for "modified" HEC
        std::vector<std::pair<int, std::vector<int>*>>* Coarsening(Hypergraph*, 
                                                                    LayerInfo*, 
                                                                    std::vector<double>&, 
                                                                    bool, 
                                                                    bool);

        // return sorted edge idx with respect to weight and size
        std::vector<int> sortEdge(Hypergraph*);

        // return true if all node indexes in the first vector is not matched
        // otherwise return false
        bool nodesNotMatched(std::vector<int>&, int, std::vector<bool>&);

        // return true if the whole edge belongs to the same partition
        bool preservePartition(LayerInfo*, std::vector<int>&, int);
};

struct sort_edge_info
{
    int idx, nodeNum;
    double weight;

    sort_edge_info(int idx, int nodeNum, double weight): idx(idx),
                                                        nodeNum(nodeNum),
                                                        weight(weight) {};
};