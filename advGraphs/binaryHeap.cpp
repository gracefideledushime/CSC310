#include "binaryHeap.h"

HEAP::HEAP(int size) 
{
    capacity = size;
    heapSize = 0;
    array = new Edge[capacity];
}

HEAP::~HEAP() 
{
    delete[] array;
}

int HEAP::leftChild(int index) 
{
    return 2 * index + 1;
}

int HEAP::rightChild(int index) 
{
    return 2 * index + 2;
}

int HEAP::parent(int index) 
{
    return (index - 1) / 2;
}

void HEAP::insertH(Edge data) 
{
    if (heapSize == capacity) {
        cout << "Heap overflow: Cannot insert more elements." << endl;
        return;
    }

    array[heapSize] = data;
    heapifyUpMin(heapSize);
    heapSize++;
}

void HEAP::heapifyUpMin(int index) 
{
    while (index != 0 && array[parent(index)].weight > array[index].weight) 
    {
        swap(array[parent(index)], array[index]);
        index = parent(index);
    }
}

Edge HEAP::peek() 
{
    if (heapSize <= 0) 
    {
        cout << "Heap is empty!" << endl;
        return Edge{-1, -1, -1};  // Dummy edge
    }
    return array[0];
}

void HEAP::deleteMin() 
{
    if (heapSize <= 0) 
    {
        cout << "Heap is empty!" << endl;
        return;
    }

    if (heapSize == 1) 
    {
        heapSize--;
        return;
    }

    array[0] = array[heapSize - 1];
    heapSize--;
    heapifyDownMin(0);
}

void HEAP::heapifyDownMin(int index) 
{
    int left = leftChild(index);
    int right = rightChild(index);
    int smallest = index;

    if (left < heapSize && array[left].weight < array[smallest].weight)
        smallest = left;

    if (right < heapSize && array[right].weight < array[smallest].weight)
        smallest = right;

    if (smallest != index) 
    {
        swap(array[index], array[smallest]);
        heapifyDownMin(smallest);
    }
}
