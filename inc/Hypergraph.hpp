#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <cfloat>
#include <iostream>


class HyperNode
{
    private:
        std::vector<int> edges;
        std::vector<int> neighbors;
        double size0, size1;

    public:
        HyperNode(): edges({}), 
                        neighbors({}),
                        size0(0.0),
                        size1(0.0) {};
        HyperNode(double nodeSize0, double nodeSize1, int part): edges({}),
                                                                                    neighbors({}), 
                                                                                    size0(nodeSize0),
                                                                                    size1(nodeSize1) {};
        // add an edge to the hypernode
        void addEdge(int);

        // return all edges of the hypernode
        std::vector<int> &getEdges();

        // return all neighbors of the hypernode
        std::vector<int> &getNeighbors();

        // return node size on layer 0
        double getNodeSize0();

        // return node size on layer 1
        double getNodeSize1();

        // set node size on layer 0
        void setNodeSize0(double);

        // set node size on layer 1
        void setNodeSize1(double);
};


class HyperEdge
{
    private:
        double weight;
        int count;
        std::vector<int> nodes;

    public:
        HyperEdge(std::vector<int>& nodes, double weight): nodes(nodes),
                                                            count(nodes.size()),
                                                            weight(weight) {};

        // get all nodes in the hyperedge
        std::vector<int> &getNodes();

        // return weight of the edge
        double getEdgeWeight();

        void relinkNode(int, int);

        void deleteNode(int);

        int getNodeNum();

        void setNodeNum(int);
};


class Hypergraph
{
    private:
        std::vector<HyperNode*> nodes;
        std::vector<HyperEdge*> edges;
        std::vector<bool> nodeExist, edgeExist;
        double terminalSize;
        int nodeNum;

    public:
        Hypergraph(): nodes({}), edges({}), terminalSize(0){};
        Hypergraph(int nvtxs, int nedges): edges({}),
                                            nodeExist(std::vector<bool>(nvtxs, true)),
                                            edgeExist(std::vector<bool>(nedges, true)),
                                            terminalSize(0),
                                            nodeNum(nvtxs) {
                                                nodes.reserve(nvtxs);
                                                for (int i=0; i<nvtxs; i++)
                                                    nodes.push_back(new HyperNode());
                                            };
        ~Hypergraph() {
            for (int i=0; i<nodes.size(); i++)
                delete nodes[i];
            for (int i=0; i<edges.size(); i++)
                delete edges[i];
        }

        // add a new edge and the given node list
        // which are connected to the new edge
        void addNodeList(int edge_id, std::vector<int> &vtxs);

        // add a hypernode to the hypergraph and increase partition size
        void addNode(HyperNode*);

        // return all hyperedges in the current graph
        std::vector<HyperEdge*> &getAllEdges();

        // return all hypernodes in the current graph
        std::vector<HyperNode*> &getAllNodes();

        // return connected edges of the given node index
        std::vector<int> &getEdgesOf(int);

        // return connected nodes of the given edge index
        std::vector<int> &getNodesOf(int);

        // return node number
        int getNodeNum();

        // return edge number
        int getEdgeNum();

        // return node size of the given layer
        double getNodeSizeOf(int, int);

        // return edge weight of the node
        double getEdgeWeightOf(int);

        // return neighbors of the given node index
        std::vector<int>& getNeighborsOf(int);

        // return terminal size
        double getTerminalSize();

        std::pair<int, std::vector<int>*> coarseNodes(std::vector<int>*);

        int getNodeNumOf(int);

        bool getEdgeExistOf(int);

        bool getNodeExistOf(int);

        void deleteNodeOf(int, int);

        void relinkNodeOf(int, int);

        void setEdgeExistOf(int, bool);

        void setNodeExistOf(int, bool);

        void setNodeNumOf(int, int);

        void setNodeNum(int);

        // set node size on the given layer
        void setNodeSizeOf(int, double, int);

        // set terminal size
        void setTerminalSize(double);
        
        // build and merge neighbors of all nodes after coarsening
        void buildNeighbors();

        void revertGraph(std::vector<std::pair<int, std::vector<int>*>>*);

        // check if the size of the given node vector is under space limit
        bool sizeUnderLimit(std::vector<int>&, int, int, double);

        // calculate and set terminal size sum
        void countTotalTerminalSize();

        // display current graph in hmetis format
        void displayGraph();
};


class LayerInfo
{
    private:
        std::vector<int> part;
        double p0size, p1size, partScore, totalTerminalSize;

    public:
        LayerInfo(Hypergraph* g): part(std::vector<int>(g->getAllNodes().size(), 1)),
                                    p0size(0),
                                    partScore(DBL_MAX),
                                    totalTerminalSize(0.0) {
                                        p1size = 0;
                                        for (int i=0; i<g->getAllNodes().size(); i++)
                                            if (g->getNodeExistOf(i)) p1size += g->getNodeSizeOf(i, 1);
                                    }

        double getp0size();

        double getp1size();

        double getPartitionScore();

        int getPartitionOf(int);

        double getTotalTerminalSize();
        
        void setp0size(double);

        void setp1size(double);

        void setPartitionScore(double);

        void setPartitionOf(int, int);

        void setTotalTerminalSize(double);

        void computeTotalTerminalSize(Hypergraph*);

        void computePartitionScore(Hypergraph*);

        void mapPartition(std::vector<std::pair<int, std::vector<int>*>>*);

        bool bfsPlacing(Hypergraph*, std::vector<double>&);
};