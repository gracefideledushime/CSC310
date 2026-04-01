#include "llrb.h"

int main()
{
    RBTREE llrb;

    for (int i = 1; i < 10; i++)
        llrb.insert(i);
    // cout << "-------------------------------------" << endl;
    llrb.printTree();
    cout << "number of rotations: " << llrb.rotations << endl;

    return 0;
}
