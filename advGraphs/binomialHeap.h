#ifndef BINOMIAL_HEAP
#define BINOMIAL_HEAP

#include <iostream>
#include <iomanip>
#include "mst.h"  

using namespace std;

struct binomialNode 
{
    struct Edge edge;
    int degree;
    binomialNode* parent;
    binomialNode* child;
    binomialNode* sibling;

    binomialNode(Edge e);
};

class BinomialHeap 
{
    private:
        binomialNode* head;

        binomialNode* unionHeaps(binomialNode* heap1, binomialNode* heap2);
        binomialNode* mergeTrees(binomialNode* tree1, binomialNode* tree2);
        void linkTrees(binomialNode*& prev, binomialNode*& curr, binomialNode*& next);

        binomialNode* reverseList(binomialNode* node);
        binomialNode* findNode(binomialNode* node, Edge edge);

    public:
        BinomialHeap();

        void insert(Edge edge);
        void merge(BinomialHeap& other);
        Edge findMin();

        void deleteMin();
        void decreaseKey(Edge oldEdge, Edge newEdge);
        void deleteKey(Edge edge);

        void printHeap();
        void printTree(binomialNode* node, int space);
};

#endif
