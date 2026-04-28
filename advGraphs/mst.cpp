#include "mst.h"
#include "binaryHeap.h"   // Custom binary heap
#include "binomialHeap.h" // Custom binomial heap

// ===== QuickUnion =====
QuickUnion::QuickUnion(int n)
{
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
    }
}

int QuickUnion::find(int x)
{
    while (x != parent[x])
    {
        x = parent[x];
    }
    return x;
}

void QuickUnion::Union(int x, int y)
{
    int rootX = find(x);
    int rootY = find(y);

    if (rootX != rootY)
    {
        rootX = rootY;
    }
}

// ===== UnionFind =====
UnionFind::UnionFind(int n)
{
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
    }
}

int UnionFind::find(int x)
{
    while (x != parent[x])
    {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

void UnionFind::Union(int x, int y)
{
}

// ===== MST =====
MST::MST(int vertices)
{
    n = vertices;
}

void MST::addEdge(int u, int v, int weight)
{
    edges.push_back({u, v, weight});
}

int MST::kruskalV1()
{
}

int MST::kruskalV2()
{
}
