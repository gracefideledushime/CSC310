#include "mst.h"

void testSmallInput() {
    cout << "=== Small Graph Test ===" << endl;
    MST graph(6);

    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 2, 2);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 5);
    graph.addEdge(2, 4, 11);
    graph.addEdge(3, 4, 2);
    graph.addEdge(4, 5, 1);
    graph.addEdge(3, 5, 7);

    cout << "Running Baseline MST..." << endl;
    auto start1 = chrono::high_resolution_clock::now();
    int cost1 = graph.kruskalV1();
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = end1 - start1;
    cout << "MST Cost: " << cost1 << endl;
    cout << "Time taken: " << elapsed1.count() << " seconds" << endl << endl;

    cout << "Running Optimized MST..." << endl;
    auto start2 = chrono::high_resolution_clock::now();
    int cost2 = graph.kruskalV2();
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = end2 - start2;
    cout << "MST Cost: " << cost2 << endl;
    cout << "Time taken: " << elapsed2.count() << " seconds" << endl << endl;
}

void testLargeInput(int numNodes, int numEdges) {
    cout << "=== Large Graph Test (" << numNodes << " nodes, " << numEdges << " edges) ===" << endl;
    MST graph(numNodes);
    srand(42); 

    for (int i = 0; i < numNodes - 1; ++i) {
        int weight = 1 + rand() % 100;
        graph.addEdge(i, i + 1, weight);
    }

    int addedEdges = numNodes - 1;
    while (addedEdges < numEdges) {
        int u = rand() % numNodes;
        int v = rand() % numNodes;
        if (u != v) {
            int weight = 1 + rand() % 1000;
            graph.addEdge(u, v, weight);
            ++addedEdges;
        }
    }

    cout << "Running Baseline MST..." << endl;
    auto start1 = chrono::high_resolution_clock::now();
    int cost1 = graph.kruskalV1();
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = end1 - start1;
    cout << "MST Cost: " << cost1 << endl;
    cout << "Time taken: " << elapsed1.count() << " seconds" << endl << endl;

    cout << "Running Optimized MST..." << endl;
    auto start2 = chrono::high_resolution_clock::now();
    int cost2 = graph.kruskalV2();
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = end2 - start2;
    cout << "MST Cost: " << cost2 << endl;
    cout << "Time taken: " << elapsed2.count() << " seconds" << endl << endl;
}


int main() {
    
    testSmallInput();

    testLargeInput(100000, 500000); 

    return 0;
}
