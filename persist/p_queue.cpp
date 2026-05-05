#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include "customErrorClass.h"
using namespace std;

template <typename T>
struct Node
{
    T value;
    shared_ptr<Node<T>> next;

    Node(T val, shared_ptr<Node<T>> nxt = nullptr)
    {
        value = val;
        next = nxt;
    }
};

template <typename T>
shared_ptr<Node<T>> reverse(shared_ptr<Node<T>> head)
{
    shared_ptr<Node<T>> result = nullptr;

    while (head)
    {
        result = make_shared<Node<T>>(head->value, result);
        head = head->next;
    }

    return result;
}

template <typename T>
class persistentQueue
{
private:
    shared_ptr<Node<T>> front;
    shared_ptr<Node<T>> rear;
    int frontSize;
    int rearSize;

    persistentQueue balance()
    {
        // maintain the invariant
        if (rearSize <= frontSize)
            return *this;

        // trigger expensive operation
        // step 1 - reverse the rear stack
        auto newFront = front;
        auto reversedRear = ::reverse(rear);
        // step 2 - add the reversed list to the front stack
        auto temp = reversedRear;
        while (temp)
        {
            newFront = make_shared<Node<T>>(temp->value, newFront);
            temp = temp->next;
        }

        return persistentQueue(newFront, nullptr, frontSize + rearSize, 0);
    }

public:
    persistentQueue()
    {
        front = nullptr;
        rear = nullptr;
        frontSize = 0;
        rearSize = 0;
    }

    persistentQueue(shared_ptr<Node<T>> f, shared_ptr<Node<T>> r, int fsize, int rsize)
    {
        front = f;
        rear = r;
        frontSize = fsize;
        rearSize = rsize;
    }

    bool isEmpty()
    {
        return frontSize == 0;
    }

    persistentQueue enqueue(T x)
    {
        auto newRear = make_shared<Node<T>>(x, rear);
        return persistentQueue(front, newRear, frontSize, rearSize + 1).balance();
    }

    persistentQueue dequeue()
    {
        if (isEmpty())
            throw runtime_error("Queue is empty");

        return persistentQueue(front->next, rear, frontSize - 1, rearSize).balance();
    }

    T peek()
    {
        if (isEmpty())
            throw runtime_error("Queue is empty");

        return front->value;
    }

    persistentQueue reverse()
    {
        // Collect all elements in order
        vector<T> elements;
        auto temp = front;
        while (temp != nullptr)
        {
            elements.push_back(temp->value);
            temp = temp->next;
        }
        auto revRear = ::reverse(rear);
        temp = revRear;
        while (temp != nullptr)
        {
            elements.push_back(temp->value);
            temp = temp->next;
        }
        // Reverse the elements
        std::reverse(elements.begin(), elements.end());
        // Build a new queue from the reversed elements
        persistentQueue<T> newQueue;
        for (auto e : elements)
        {
            newQueue = newQueue.enqueue(e);
        }
        return newQueue;
    }

    void print()
    {
        auto temp = front;
        while (temp)
        {
            cout << temp->value << " ";
            temp = temp->next;
        }

        auto revRear = ::reverse(rear);
        temp = revRear;
        while (temp)
        {
            cout << temp->value << " ";
            temp = temp->next;
        }

        cout << endl;
    }
};

void testLargeInput(int numTrees)
{
    cout << "=== Large p_queue Test (" << numTrees << " trees) ===" << endl;
    vector<persistentQueue<int>> trees;
    trees.resize(numTrees);

    srand(42);
    auto start2 = chrono::high_resolution_clock::now();
    cout << "Timing Large input p_queue.cpp" << endl;
    try
    {
        for (int i = 0; i < numTrees - 1; ++i)
        {
            trees[i + 1] = trees[i].enqueue(rand() % 257 + 1);
        }

        cout << "Last tree:" << endl;
        trees[numTrees - 1].print();

        // cout << "Last tree reverse" << endl;
        // trees[numTrees - 1].reverse().print();
    }
    catch (MyException &e)
    {
        cerr << "MST Error: " << e.what() << endl;
    }
    catch (exception &e) // fallback for any other std exceptions
    {
        cerr << "Unexpected error: " << e.what() << endl;
    }

    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = end2 - start2;
    cout << "Time taken for large input: " << elapsed2.count() << " seconds" << endl
         << endl;
}

int main()
{
    persistentQueue<int> t1;
    auto start1 = chrono::high_resolution_clock::now();
    cout << "Timing small input p_bst_1.cpp" << endl;
    auto t2 = t1.enqueue(10);
    auto t3 = t2.dequeue();
    auto t4 = t3.enqueue(30);
    auto t5 = t4.enqueue(40);
    auto t6 = t5.dequeue();

    t2.print();
    t3.print();
    t4.print();
    t5.print();
    t6.print();

    // Test reverse
    auto t7 = t6.reverse();
    cout << "Reversed t6: ";
    t7.print();

    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = end1 - start1;
    cout << "Time taken: " << elapsed1.count() << " seconds" << endl
         << endl;

    // Large input

    testLargeInput(10000);
    return 0;
}