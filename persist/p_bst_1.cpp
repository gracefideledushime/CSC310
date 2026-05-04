// persistent BST
// path copying

#include <iostream>

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

int main()
{
    persistentBST t1;

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

    return 0;
}