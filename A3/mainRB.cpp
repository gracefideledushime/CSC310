#include "llrb.h"
#include <random>

int main()
{
    RBTREE llrb;

    random_device rd;

    mt19937 gen(rd());
    uniform_int_distribution<> distrib(12, 40);

    for (int i = 1; i < 10; i++)
        llrb.insert(distrib(gen));
    // cout << "-------------------------------------" << endl;
    llrb.printTree();
    cout << "number of rotations: " << llrb.rotations << endl;

    return 0;
}
