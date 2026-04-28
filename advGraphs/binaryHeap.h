#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include "mst.h" 

using namespace std;

class HEAP 
{
private:
    struct Edge *array;
    int capacity;
    int heapSize;

    int leftChild(int index);
    int rightChild(int index);
    int parent(int index);

    void heapifyUpMin(int index);
    void heapifyDownMin(int index);

public:
    HEAP(int size);
    ~HEAP();

    void insertH(Edge data);
    void deleteMin();
    Edge peek();
};

#endif
