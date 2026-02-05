#include <iostream>
#include "bst.h"
using namespace std;


int main(){
    BST bst(nullptr);
    // bst.printBST(bst.root);
    bst.root = bst.insert(bst.root, 5); //root = 5
    bst.root = bst.insert(bst.root, 3); //root = 
    bst.root = bst.insert(bst.root, 9); //root = 5
    bst.root = bst.insert(bst.root, 8); //root = 5
    bst.root = bst.insert(bst.root, 10); //root = 5
    bst.root = bst.insert(bst.root, 2);
    bst.printBST(bst.root);

    cout << endl << "count: " << bst.nodeCount << endl;
    
    bst.root = bst.del(bst.root, 10);
    bst.root = bst.del(bst.root, 9);
    bst.printBST(bst.root);

    cout << endl << "count: " << bst.nodeCount << endl;

    cout << "heights: " << bst.find_node_height(bst.root) << " data: " << bst.root->data<< endl;
    
    cout << "balanced?: " << bst.isBalanced(bst.root) << " data: " << bst.root->data<< endl;

    //search
    // cout << endl << bst.search(bst.root, 6)->data << endl;
    return 0;
}