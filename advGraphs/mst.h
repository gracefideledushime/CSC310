#ifndef MST_H
#define MST_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <vector>


using namespace std;

struct Edge 
{
    int src;
    int dest;
    int weight;
};

class QuickUnion 
{
    private:
        vector<int> parent;

    public:
        QuickUnion(int n);
        int find(int x);
        void Union(int x, int y);
};

class UnionFind 
{
    private:
        vector<int> parent, rank;
    
    public:
        UnionFind(int n);
        int find(int x);
        void Union(int x, int y);
};

class MST {
    private:
        int n;
        vector<Edge> edges;

    public:
        MST(int vertices);
        void addEdge(int src, int dest, int weight);
        int kruskalV1();     // Binary Heap + simple Quick-Union
        int kruskalV2();    // Binomial Heap + optimized Union-Find
};

#endif
