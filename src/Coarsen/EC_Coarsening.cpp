#include "../../inc/Coarsen/EC_Coarsening.hpp"
#include "../inc/Hypergraph.hpp"
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>


// main function of EC coarsening
std::vector<std::pair<int, std::vector<int>*>>* EC_Coarsening::Coarsening(Hypergraph* graph,
                                                                            LayerInfo* inst,
                                                                            std::vector<double>& spaceLimit,
                                                                            bool restricted)
{
    std::vector<double> nodeWeights = computeNodeWeights(graph);
    std::vector<int> nds;
    std::vector<bool> ndsRemain(graph->getAllNodes().size(), true);
    auto coarsenInfo = new std::vector<std::pair<int, std::vector<int>*>>();
    int nodeNum = graph->getNodeNum();

    nds.reserve(graph->getAllNodes().size());
    for (int i=0; i<graph->getAllNodes().size(); i++)
    {
        if (!graph->getNodeExistOf(i))
            ndsRemain[i] = false;
        nds.push_back(i);
    }

    std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(nds.begin(), nds.end(), g);
    std::random_shuffle(nds.begin(), nds.end());

    for (auto it=nds.begin(); it!=nds.end(); it++)
    {
        if (!ndsRemain[*it]) continue;

        int idx = chooseMaxWeight(graph, inst, nodeWeights, spaceLimit, ndsRemain, *it, restricted);
        ndsRemain[*it] = false;

        if (*it != idx) 
        {
            ndsRemain[idx] = false;

            std::vector<int>* nodeVec = new std::vector<int>();
            nodeVec->push_back(*it);
            nodeVec->push_back(idx);

            auto info = graph->coarseNodes(nodeVec);
            coarsenInfo->push_back(info);

            nodeNum--;
        }
    }

    graph->setNodeNum(nodeNum);
    return coarsenInfo;
}

// return vector of the weight of each node
// node weight: the sum of weights of all the edges connected to a node
std::vector<double> EC_Coarsening::computeNodeWeights(Hypergraph* graph)
{
    std::vector<double> nodeWeights;
    nodeWeights.reserve(graph->getAllNodes().size());

    for (int i=0; i<graph->getAllNodes().size(); i++)
    {
        double w = DBL_MAX;

        if (graph->getNodeExistOf(i))
        {
            std::vector<int>& edges = graph->getEdgesOf(i);

            for (int j=0; j<edges.size(); j++) 
            {
                w += graph->getEdgeWeightOf(edges[j]);
            }
        }

        nodeWeights.push_back(w);
    }

    return nodeWeights;
}

// return neighbor node of n1 with maximum weight
int EC_Coarsening::chooseMaxWeight(Hypergraph* graph,
                                LayerInfo *inst,
                                std::vector<double>& nodeWeights,
                                std::vector<double>& spaceLimit,
                                std::vector<bool>& ndsRemain,
                                int n1,
                                bool restricted)
{
    // edges connected to node n1
    std::vector<int>& edges = graph->getEdgesOf(n1);
    int nMax = n1, part = inst->getPartitionOf(n1);
    double maxWeight = 0, size = graph->getNodeSizeOf(n1, part);

    for (auto it=edges.begin(); it!=edges.end(); it++) 
    {
        std::vector<int>& neigs = graph->getNodesOf(*it);
        int count = graph->getNodeNumOf(*it);

        if (count == 1) continue;

        for (auto itt=neigs.begin(); itt!=neigs.begin()+count; itt++)
        {
            if (restricted && inst->getPartitionOf(*itt)!=part) continue;
            
            if (ndsRemain[*itt] && nodeWeights[*itt] > maxWeight 
                && graph->getNodeSizeOf(*itt, part)+size <= spaceLimit[part]) 
            {
                maxWeight = nodeWeights[*itt];
                nMax = *itt;
            }
        }
    }

    return nMax;
}