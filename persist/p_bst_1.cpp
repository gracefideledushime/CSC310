// persistent BST
// path copying

#include <iostream>
#include <chrono>
#include <vector>
#include "customErrorClass.h"

using namespace std;

struct Node
{
    int key;
    Node *left;
    Node *right;
    // int refcount;  it is not thread-safe

    Node(int k, Node *l = nullptr, Node *r = nullptr)
    {
        key = k;
        left = l;
        right = r;
    }
};

class persistentBST
{
private:
    Node *root;

    Node *insert(Node *node, int key)
    {
        if (!node)
            return new Node(key);

        if (key < node->key)
        {
            return new Node(node->key, insert(node->left, key), node->right);
        }
        else if (key > node->key)
        {
            return new Node(node->key, node->left, insert(node->right, key));
        }
        else
        {
            return node;
        }
    }

    bool search(Node *node, int key)
    {
        if (!node)
            return false;
        if (key == node->key)
            return true;
        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    void inorder(Node *node)
    {
        if (!node)
            return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    Node *deleteNode(Node *root, int key)
    {
        if (!root)
            return nullptr;

        if (key < root->key)
            return new Node(root->key, deleteNode(root->left, key), root->right);
        else if (key > root->key)
            return new Node(root->key, root->left, deleteNode(root->right, key));
        else
        {
            if (!root->left)
                return root->right;
            if (!root->right)
                return root->left;

            Node *succ = root->right;
            while (succ->left)
                succ = succ->left;

            return new Node(succ->key, root->left, deleteNode(root->right, succ->key));
        }
    }

public:
    persistentBST()
    {
        root = nullptr;
    }
    persistentBST(Node *r)
    {
        root = r;
    }

    persistentBST insert(int key)
    {
        return persistentBST(insert(root, key));
    }
    bool search(int key)
    {
        return search(root, key);
    }

    void inorder()
    {
        inorder(root);
        cout << endl;
    }

    persistentBST remove(int key)
    {
        return persistentBST(deleteNode(root, key));
    }
};

void testLargeInput(int numTrees)
{
    cout << "=== Large p_bst_1 Test (" << numTrees << " trees) ===" << endl;
    vector<persistentBST> trees;
    trees.resize(numTrees);

    srand(42);
    auto start2 = chrono::high_resolution_clock::now();
    cout << "Timing Large input p_bst_1.cpp" << endl;
    try
    {
        for (int i = 0; i < numTrees - 1; ++i)
        {
            trees[i + 1] = trees[i].insert(rand() % 257 + 1);
        }

        cout << "Last tree:" << endl;
        trees[numTrees - 1].inorder();
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

    // // Test delete
    // for (int i = 0; i < numTrees - 1; ++i)
    // {
    //     trees[i + 1] = trees[i].remove(); // using rand here could be chaos
    // }
}

int main()
{
    persistentBST t1;
    auto start1 = chrono::high_resolution_clock::now();
    cout << "Timing small input p_bst_1.cpp" << endl;

    auto t2 = t1.insert(10);
    auto t3 = t2.insert(20);
    auto t4 = t3.insert(30);
    auto t5 = t4.insert(40);

    auto t6 = t5.remove(30); // delete node with one child
    auto t7 = t5.remove(10); // delete root (two children)
    auto t8 = t5.remove(40); // delete leaf

    cout << "t5 (original): ";
    t5.inorder(); // 10 20 30 40
    cout << "t6 (del 30):   ";
    t6.inorder(); // 10 20 40
    cout << "t7 (del 10):   ";
    t7.inorder(); // 20 30 40
    cout << "t8 (del 40):   ";
    t8.inorder(); // 10 20 30
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = end1 - start1;
    cout << "Time taken: " << elapsed1.count() << " seconds" << endl
         << endl;

    // Large input

    testLargeInput(10000);

    return 0;
}
