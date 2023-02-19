#pragma once

#include "Hypergraph.hpp"
#include <random>
#include <ctime>

class EC_Coarsening
{
    public:
        EC_Coarsening() = default;

        // coarsen nodes in a hypergraph
        // return coarsened hypergraph
        // bool for if the coarsening is "restricted"
        std::vector<std::pair<int, std::vector<int>*>>* Coarsening(Hypergraph*, 
                                                                    LayerInfo*, 
                                                                    std::vector<double>& spaceLimit, 
                                                                    bool);

        // return the id of maximal weight matching node (EC coarsening)
        int getMaxWeightMatch(const int);

        // return vector of the weight for each node
        // node weight: the sum of weights of all the edges connected to a node
        std::vector<double> computeNodeWeights(Hypergraph*);
        
        // return connected maximum weight node of n1
        // bool for choose node in the same partition
        int chooseMaxWeight(Hypergraph*, LayerInfo *, std::vector<double>&, std::vector<double>&, std::vector<bool>&, int, bool);

        // set edge weights for lower level graph from upper level graph
        void computeWeights(Hypergraph*, Hypergraph*);
};