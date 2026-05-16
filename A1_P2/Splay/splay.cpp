#include "splay.h"

SplayTree::SplayTree()
{
    root = nullptr;
}

// aka Zig
SplayTree::Node *SplayTree::rotateRight(Node *x)
{
    Node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// aka Zag
SplayTree::Node *SplayTree::rotateLeft(Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Bring node with given key (or its closest predecessor/successor) to root
SplayTree::Node *SplayTree::splay(Node *root, int key)
{
    // Base case: empty tree or key is already at root
    if (!root || root->key == key)
        return root;

    if (key < root->key)
    {
        // Key is not in tree
        if (!root->left)
            return root;

        if (key < root->left->key)
        {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root); // rotate grandparent
        }
        else if (key > root->left->key)
        {
            // Zig-Zag (Left-Right): splay within left-right subtree first,
            // then rotate parent left, then grandparent right
            root->left->right = splay(root->left->right, key);
            if (root->left->right)
                root->left = rotateLeft(root->left); // rotate parent
        }

        // Final right rotation (or none if left child is null)
        return root->left ? rotateRight(root) : root;
    }
    else // key > root->key
    {
        // Key is not in tree
        if (!root->right)
            return root;

        if (key < root->right->key)
        {
            root->right->left = splay(root->right->left, key);
            if (root->right->left)
                root->right = rotateRight(root->right); // rotate parent
        }
        else if (key > root->right->key)
        {

            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root); // rotate grandparent
        }

        // Final left rotation (or none if right child is null)
        return root->right ? rotateLeft(root) : root;
    }
}

SplayTree::Node *SplayTree::insertNode(Node *root, int key)
{
    // Empty tree: just create the root node
    if (!root)
        return new Node(key);

    // Splay the closest key to root
    root = splay(root, key);

    // Duplicate: do nothing
    if (root->key == key)
        return root;

    Node *newNode = new Node(key);

    if (key < root->key)
    {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr;
    }
    else
    {
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr;
    }

    return newNode;
}

// Splay the key to root, then merge the two remaining subtrees
SplayTree::Node *SplayTree::deleteNode(Node *root, int key)
{
    if (!root)
        return nullptr;

    // Bring the target key (or nearest) to root
    root = splay(root, key);

    // Key not found: nothing to delete
    if (root->key != key)
        return root;

    Node *toDelete = root;

    if (!root->left)
    {
        // No left subtree: promote right subtree
        root = root->right;
    }
    else
    {

        Node *leftTree = splay(root->left, key);
        leftTree->right = root->right; // reattach right subtree
        root = leftTree;
    }

    delete toDelete;
    return root;
}

void SplayTree::insert(int key)
{
    root = insertNode(root, key);
}

void SplayTree::remove(int key)
{
    root = deleteNode(root, key);
}

// search() already implemented via splay: splays key to root, checks root
bool SplayTree::search(int key)
{
    root = splay(root, key);
    return (root && root->key == key);
}

void SplayTree::printTree(Node *root, int space)
{
    const int COUNT = 10;
    if (!root)
        return;

    space += COUNT;

    printTree(root->right, space);

    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout << root->key << "\n";

    printTree(root->left, space);
}

void SplayTree::display()
{
    printTree(root, 0);
    cout << "\n";
}