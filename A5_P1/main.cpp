#include "binomial.h"
#include "customErrorClass.h"

int main()
{
    BinomialHeap bh;
    bh.insert(10);
    bh.insert(20);
    bh.insert(5);
    bh.insert(30);
    bh.insert(1);
    bh.insert(14);
    bh.insert(27);
    bh.insert(8);
    bh.insert(3);
    bh.insert(12);

    try
    {
        cout << "Heap after insertions:" << endl;
        bh.printHeap();

        cout << endl
             << "Minimum key (initial): " << bh.findMin()->key << endl;

        bh.deleteMin();
        bh.deleteMin();
        bh.deleteMin();

        cout << "Heap after 3 min deletions:" << endl;
        bh.printHeap();

        cout << endl
             << "Minimum key: " << bh.findMin()->key << endl;

        cout << "Heap after decreaseKey deletions:" << endl;
        bh.printHeap();

        bh.deleteKey(24);
        int min = bh.findMin()->key;
        bh.deleteKey(min);
        cout << endl
             << "Minimum key: " << bh.findMin()->key << endl;
    }
    catch (MyException &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}