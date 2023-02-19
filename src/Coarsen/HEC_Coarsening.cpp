#include "../../inc/Coarsen/HEC_Coarsening.hpp"
#include "../inc/Hypergraph.hpp"
#include <algorithm>
#include <vector>
#include <numeric>
#include <utility>
#include <time.h>

// main function of HEC coarsening
// MHEC for "modified" HEC
std::vector<std::pair<int, std::vector<int>*>>* HEC_Coarsening::Coarsening(Hypergraph* graph,
                                                                            LayerInfo* inst,
                                                                            std::vector<double>& spaceLimit,
                                                                            bool restricted,
                                                                            bool MHEC)
{
    std::vector<int> sortedEdge = sortEdge(graph);
    std::vector<bool> nodesRemain(graph->getAllNodes().size(), true);
    auto coarsenInfo = new std::vector<std::pair<int, std::vector<int>*>>();
    int nodeNum = graph->getNodeNum();

    for (auto e_it=sortedEdge.begin(); e_it!=sortedEdge.end(); e_it++)
    {
        std::vector<int>& nodesConnected = graph->getNodesOf(*e_it);
        int count = graph->getNodeNumOf(*e_it);
        int part = inst->getPartitionOf(nodesConnected[0]);
       
        // check if "preserve initial partitioning" is needed and conformed
        // check if all connected nodes in the edge have not been picked
        if ((!restricted || (restricted && preservePartition(inst, nodesConnected, count)))
            && nodesNotMatched(nodesConnected, count, nodesRemain)
            && graph->sizeUnderLimit(nodesConnected, count, part, spaceLimit[part]))
        {
            std::vector<int>* nds = new std::vector<int>();

            for (auto it=nodesConnected.begin(); it!=nodesConnected.begin()+count; it++) 
            {
                nds->push_back(*it);
                nodesRemain[*it] = false;
            }

            auto info = graph->coarseNodes(nds);
            coarsenInfo->push_back(info);

            graph->setEdgeExistOf(*e_it, false);
            nodeNum = nodeNum - count + 1;
        }
    }

    if (MHEC)
    {
        for (auto e_it=sortedEdge.begin(); e_it!=sortedEdge.end(); e_it++)
        {
            if (!graph->getEdgeExistOf(*e_it)) continue;

            std::vector<int> &nodesConnected = graph->getNodesOf(*e_it);
            std::vector<int>* unmatchedNodes0 = new std::vector<int>();
            std::vector<int>* unmatchedNodes1 = new std::vector<int>();
            int count = graph->getNodeNumOf(*e_it);
            double size0 = 0, size1 = 0;
            
            for (auto it=nodesConnected.begin(); it!=nodesConnected.begin()+count; it++)
            {
                if (nodesRemain[*it])
                {
                    if (inst->getPartitionOf(*it) == 0)
                    {
                        unmatchedNodes0->push_back(*it);
                        size0 += graph->getNodeSizeOf(*it, 0);
                    }
                    else
                    {
                        unmatchedNodes1->push_back(*it);
                        size1 += graph->getNodeSizeOf(*it, 1);
                    }
                }
            }

            if (!unmatchedNodes0->empty() && size0 <= spaceLimit[0])
            {
                auto info = graph->coarseNodes(unmatchedNodes0);
                coarsenInfo->push_back(info);
                nodeNum = nodeNum - unmatchedNodes0->size() + 1;
                for (auto it=unmatchedNodes0->begin(); it!=unmatchedNodes0->end(); it++) nodesRemain[*it] = false;
            }
            else
                delete unmatchedNodes0;

            if (!unmatchedNodes1->empty() && size1 <= spaceLimit[1])
            {
                auto info = graph->coarseNodes(unmatchedNodes1);
                coarsenInfo->push_back(info);
                nodeNum = nodeNum - unmatchedNodes1->size() + 1;
                for (auto it=unmatchedNodes1->begin(); it!=unmatchedNodes1->end(); it++) nodesRemain[*it] = false;
            }
            else
                delete unmatchedNodes1;
        }
    }
    graph->setNodeNum(nodeNum);
    
    return coarsenInfo;
}

bool cmp(const sort_edge_info& a, const sort_edge_info& b)
{
    if (a.weight != b.weight)
        return a.weight > b.weight;
    else
        return a.nodeNum < b.nodeNum;
}

// return sorted edge idx with respect to weight and size
std::vector<int> HEC_Coarsening::sortEdge(Hypergraph *graph)
{
    std::vector<sort_edge_info> infos;
    for (int i=0; i<graph->getEdgeNum(); i++)
    {
        if (graph->getEdgeExistOf(i) && graph->getNodeNumOf(i) != 1) 
            infos.push_back(sort_edge_info(i, graph->getNodeNumOf(i), graph->getEdgeWeightOf(i)));
    }

    sort(infos.begin(), infos.end(), cmp);

    std::vector<int> edgeIdxs;
    edgeIdxs.reserve(infos.size());
    for (auto it=infos.begin(); it!=infos.end(); it++)
        edgeIdxs.push_back(it->idx);

    return edgeIdxs;
}

// return true if all node indexes in the first vector is not matched
// otherwise return false
bool HEC_Coarsening::nodesNotMatched(std::vector<int>& nodesConnected, int count, std::vector<bool>& nodesRemain)
{
    for (auto it=nodesConnected.begin(); it!=nodesConnected.begin()+count; it++)
    {
        if (!nodesRemain[*it])
            return false;
    }

    return true;
}


// return true if the whole edge belongs to the same partition
bool HEC_Coarsening::preservePartition(LayerInfo* inst, std::vector<int>& nodesConnected, int count)
{
    int part = inst->getPartitionOf(nodesConnected[0]);

    for (auto it=nodesConnected.begin()+1; it!=nodesConnected.begin()+count; it++)
    {
        if (inst->getPartitionOf(*it) != part)
            return false;
    }

    return true;
}