#include "mst.h"
#include "binaryHeap.h"   // Custom binary h2
#include "binomialHeap.h" // Custom binomial h2
#include "Includes/customErrorClass.h"
#include <algorithm>

using namespace std;

// ===== QuickUnion =====
QuickUnion::QuickUnion(int n)
{
    if (n <= 0)
        throw MyException("QuickUnion: cannot initialize with n <= 0, vector would be unresized.");
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
    if (n <= 0)
        throw MyException("UnionFind: cannot initialize with n <= 0, vector would be unresized.");
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
    // 1. Load all edges into custom binary min-h2
    HEAP h2(edges.size());
    for (const auto &edge : edges)
        h2.insertH(edge);

    // 2. Init Quick Union — each node is its own root
    QuickUnion qu(n);

    int totalWeight = 0;
    int edgesUsed = 0;

    // 3. Greedily extract ch2est edge, skip if it forms a cycle
    while (!h2.isEmpty() && edgesUsed < n - 1)
    {
        auto edge = h2.peek();

        int u = edge.src;
        int v = edge.dest;
        int w = edge.weight;
        h2.deleteMin(); // O(log E) amortized

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
    // 1. Load all edges into custom binomial min-h2
    BinomialHeap h2;
    for (const auto &edge : edges)
        h2.insert(edge);

    // 2. Init Union-Find — path compression + union by rank
    UnionFind uf(n);

    int totalWeight = 0;
    int edgesUsed = 0;

    // 3. Same greedy logic, but faster union-find operations
    while (!h2.isEmpty() && edgesUsed < n - 1)
    {
        auto edge = h2.findMin();

        int u = edge.src;
        int v = edge.dest;
        int w = edge.weight;
        h2.deleteMin(); // O(log E) amortized

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
