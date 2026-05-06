Binomial heaps enables O(log n) merging, compared to O(n) for binary heaps. They achieve this by structuring the heap as a collection of up to \(\log n\) ordered binomial trees rather than a single tree, allowing merging to behave like binary addition of their tree degrees.

Time complexity
- findMIn(): O(log n) since the minimum value is the root of one of the binomial trees
- insert: O(1) amortized: insert is treated as a union of the current heap with a signle-node binomial heap
- Union (Merge): O(log n): combines two heaps by merging their root lists 0(1) and then linking trees of the same order - the O(log n) part
- deleteMin(): O(log n): finds and removes the minimum root and then merge() its children
- decreaseKey(): O(log n)
- deleteNode(): O(log n):


Left-child right-sibling (LCRS) representation is used in binomial heaps to efficiently manage multi-way trees with an unknown number of children, restricting each node to just two pointers (leftmost-child and right-sibling) rather than a variable-sized array. This structure allows O(1) merging of binomial trees and O(log n) total merge time for heaps by treating children as a simple linked

CFR with binomial heap looks promising. Low priority tasks would be the minimum nodes still.