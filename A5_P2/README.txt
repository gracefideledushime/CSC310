TASK 5

Small Input

Running Baseline MST...
MST Cost: 14
Time taken: 1.54e-05 seconds

Running Optimized MST...
MST Cost: 14
Time taken: 2.12e-05 seconds

=== Large Graph Test (100000 nodes, 500000 edges) ===
Running Baseline MST...
MST Cost: 4226604
Time taken: 5.03904 seconds

Running Optimized MST...
MST Cost: 4226604
Time taken: 0.110329 seconds

TASK 8
- Binomial heaps improve Kruskal's algorithm efficiency by enabling O(log n) or amortized O(1) merging of priority queues, significantly faster than the O(n) required for binary heaps.
Because Kruskal's requires sorting and extracting minimum-weight edges, the fast extraction of min nodes combined with efficient merging allows better performance when managing large, sparse graphs.

- Union-find allows cycle detection and component merging in Kruskal's algorithm. Path compression reduce the find time complexity to nearly O(1) while union by size keeps the balance when merging.

- 