#include "dsw.h"

// ----------------- PRIVATE ----------------------------------

// when left-heavy tree
void BST::rotateRight(Node*& node) // passing the parent
{
    if(node == nullptr || node->left == nullptr)
        return;
    
    // get the node to rotate R
    Node* leftChild = node->left; 
    // 1) leftnode's right child is going to become parent's left child
    node->left = leftChild->right;
    // 2) parent is going to be right child of node that is rotated
    leftChild->right = node;
    
    node = leftChild;
}

// when right-heavy
void BST::rotateLeft(Node*& node)
{
    if(node == nullptr || node->right == nullptr)
        return;

    // get the node to rotate L
    Node* rightChild = node->right;
    // 1) rightnode's left child is going to become parent's right child
    node->right = rightChild->left;
    // 2) parent is going to be left child of node that is rotated
    rightChild->left = node;

    node = rightChild;
}

int BST::subtreeSize(Node* root){
    if (root == nullptr)
        return 0;

    return 1 + subtreeSize(root->right) + subtreeSize(root->left);
           
}

// Phase 1 - right skewed linked list tree
void BST::createVine()
{
    if(root == nullptr)
        return;
    
    Node* grandparent = nullptr;
    Node* parent = root;
    Node* child = root->right;

    while(parent != nullptr){
        if(child != nullptr) {
            if (subtreeSize(parent->right) > 2 ) {
                rotateLeft(parent);
            
                if(grandparent == nullptr)
                    root = parent;
                else
                    grandparent->left = parent;
                    child = parent->right;
            }
            else {
                grandparent = parent;
                parent = parent->left;
                if(parent != nullptr)
                    child = parent->left;
            }
        } else{ // no right child -> just keep moving
            grandparent = parent;
            parent = parent->left;
            if(parent != nullptr)
                child = parent->left;
        }
    }
}

void BST::rebuildTree(int size)
{
    // how many left rotations do we need - based on
    // 1. how many extra nodes
    // 2. 
    int h = (int)log2(size + 1);
    h = h/2;
    int m = (1 << h) - 1; // 2^h - 1 same as pow(2, h) but may give wrong results: nodes in the perfect (complete) subtree
    int extra = size - m; // extra nodes

    // initial rot
    performRotation(extra);

    //subsequent rots
    for(size = m/2; size > 0; size /= 2){
        performRotation(size);
    }

}

// left rotate every second node based count
void BST::performRotation(int count)
{
    Node* grandparent = nullptr;
    Node* parent = root;

    Node* child = parent->right;

    int i = 1;
    while (parent != nullptr && parent->left != nullptr && count > 0){
        if(i%2==1){
            if(grandparent == nullptr){
                rotateRight(root);
                parent = root;
            }
            else{
                rotateRight(grandparent->left);
                parent = grandparent->left;
            }
            count--;
        }else{
            grandparent = parent;
            parent = grandparent->left;
        }
        i++;
    }

}

void BST::printTree(Node* root, int space) {
    const int COUNT = 10; 

    if (root == nullptr) {
        return;
    }

    // Increase the distance between levels
    space += COUNT;

    // Print the right child first (to appear on top)
    printTree(root->right, space);

    // Print the current node after right child

    for (int i = COUNT; i < space; i++) {
        cout << " "; // Indentation for tree depth
    }
    cout << root->data << endl;

    // Print the left child
    printTree(root->left, space);
}

// --------------------- PUBLIC ------------------
BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    deleteTree(root);
}

void BST::deleteTree(Node*& node)
{
    if(node == nullptr)
        return;
    
    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

void BST::insert(int val)
{
    Node* newNode = new Node(val);
    if(root == nullptr){
        root = newNode;
        return;
    }
    
    Node* curr = root;
    Node*parent = nullptr;

    while(curr != nullptr)
    {
        parent = curr;
        if(val < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if(val < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

void BST::dswBalance()
{
    if (root == nullptr)
        return;
    
    // phase 1
    createVine();

    // Node* t = root;
    // while (t) {
    //     if (t->right != nullptr)
    //         cout << "Left child still exists!\n";
    //     t = t->left;
    // }

    cout << "After phase 1:";
    display();

    int size = 0;
    Node* temp = root;

    while(temp != nullptr){
        size++;
        temp = temp->left;
    }

    // phase 2
    rebuildTree(size);

}

void BST::display()
{
    cout << endl;
    printTree(root, 0);
    cout << endl;
}