For no-collision data: CHAINING_VECTOR performed the best in both insertion and search. Interesting Unordered_map was the third fastest of seven in insertion but the worst in searching
DOUBLE_HASHING performed consistenly terrible even for the categories below

For low-collision data: Unordered_map performed the best in both insertion and search. All the data structures maintain a similar for both insertion and search

For high-collision data: CHAINING_LIST performed among the best in both insertion and search.

To test, run "make" then "make run"