#pragma once

#include "../Hypergraph.hpp"
#include "EC_Coarsening.hpp"
#include "HEC_Coarsening.hpp"
#include <random>
#include <ctime>

class Coarsen
{
    private:
        EC_Coarsening EC;
        HEC_Coarsening HEC;

    public:
        Coarsen(): EC(EC_Coarsening()), HEC(HEC_Coarsening()) {srand(time(NULL));};

        // coarsen nodes in a hypergraph
        // return coarsened hypergraph
        // bool for if the coarsening is "restricted"
        std::vector<std::pair<int, std::vector<int>*>>* Coarsening(Hypergraph*, 
                                                                    LayerInfo*, 
                                                                    std::vector<double>&, 
                                                                    bool, 
                                                                    std::string&);
};