### TASK 5

=== Small Graph Test ===
Running Baseline MST...
MST Cost: 14
Time taken: 1.1118e-05 seconds

Running Optimized MST...
MST Cost: 14
Time taken: 3.602e-05 seconds

=== Large Graph Test (100000 nodes, 500000 edges) ===
Running Baseline MST...
MST Cost: 3750795
Time taken: 6.87773 seconds

Running Optimized MST...
MST Cost: 3750795
Time taken: 0.352163 seconds

TASK 8
- Binomial heaps improve Kruskal's algorithm efficiency by enabling O(log n) or amortized O(1) merging of priority queues, significantly faster than the O(n) required for binary heaps.
Because Kruskal's requires sorting and extracting minimum-weight edges, the fast extraction of min nodes combined with efficient merging allows better performance when managing large, sparse graphs.

- Union-find allows cycle detection and component merging in Kruskal's algorithm. Path compression reduce the find time complexity to nearly O(1) while union by size keeps the balance when merging.

### Observation

Correctness
Both versions produce identical MST costs (14 and 3750795), which confirms both implementations are correct. They differ only in efficiency.

The heap difference is minor since both are O(log E) per extraction. The dominant factor is the union-find:

V1: Quick Union without path compression
	• Trees can grow tall and linear (degenerate chain)
	• find() walks the entire chain every time: O(n) worst case
	• With 500,000 edges × O(n) per find → approaches O(E·V) overall
	• This explains the 6.87 second runtime on 100k nodes

V2: Union-Find with path compression + union by rank
	• Union by rank keeps trees shallow and balanced
	• Path halving in find() flattens the tree as it runs
	• find() costs O(α n) where α is the inverse Ackermann function, effectively constant for any realistic input
	• Overall complexity stays close to O(E log E), dominated entirely by heap extraction
    • This explains the 0.35 second runtime on the same input