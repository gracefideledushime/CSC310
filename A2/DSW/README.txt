This code implements a custom version of DSW

We get different starting points for different trees as we rotate only nodes with subtrees of height > 2 to reduce the number of rotations.

Then, we rotate until the height is double that of a balanced tree (2log2n)
