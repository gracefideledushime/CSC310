#include "mst.h"
#include "binaryHeap.h"   // Custom binary heap
#include "binomialHeap.h" // Custom binomial heap
#include <algorithm>

using namespace std;

// ===== QuickUnion =====
QuickUnion::QuickUnion(int n)
{
    parent.resize(n);
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
        parent[rootX] = rootY;
    }
}

// ===== UnionFind =====
UnionFind::UnionFind(int n)
{
    parent.resize(n);
    rank.resize(n);
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
        rank[i] = 0;
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
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY)
        return;

    if (rank[rootX] < rank[rootY])
    {
        parent[rootX] = rootY;
        rank[rootY] += rank[rootX];
    }
    else
    {
        parent[rootY] = rootX;
        rank[rootX] += rank[rootY];
    }
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
    // 1. Load all edges into custom binary min-heap
    HEAP heap(edges.size());
    for (const auto &edge : edges)
        heap.insertH(edge);

    // 2. Init Quick Union — each node is its own root
    QuickUnion qu(n);

    int totalWeight = 0;
    int edgesUsed = 0;

    // 3. Greedily extract cheapest edge, skip if it forms a cycle
    while (!heap.isEmpty() && edgesUsed < n - 1)
    {
        auto edge = heap.peek();

        int u = edge.src;
        int v = edge.dest;
        int w = edge.weight;
        heap.deleteMin(); // O(log E) amortized

        int rootU = qu.find(u); // O(n) worst case — no path compression
        int rootV = qu.find(v);

        if (rootU != rootV) // safe to add: no cycle
        {
            qu.Union(u, v);
            totalWeight += w;
            edgesUsed++;
        }
    }

    return totalWeight;
}

int MST::kruskalV2()
{
    // 1. Load all edges into custom binomial min-heap
    HEAP heap(edges.size());
    for (const auto &edge : edges)
        heap.insertH(edge);

    // 2. Init Union-Find — path compression + union by rank
    UnionFind uf(n);

    int totalWeight = 0;
    int edgesUsed = 0;

    // 3. Same greedy logic, but faster union-find operations
    while (!heap.isEmpty() && edgesUsed < n - 1)
    {
        auto edge = heap.peek();

        int u = edge.src;
        int v = edge.dest;
        int w = edge.weight;
        heap.deleteMin(); // O(log E) amortized

        int rootU = uf.find(u); // O(α n) — near constant via path compression
        int rootV = uf.find(v);

        if (rootU != rootV)
        {
            uf.Union(u, v);
            totalWeight += w;
            edgesUsed++;
        }
    }

    return totalWeight;
}
