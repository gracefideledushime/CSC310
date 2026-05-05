// Persistent BST - Fat Node Strategy
// Each node stores a history of (version, value) pairs
// instead of copying nodes on every update

#include <iostream>
#include <vector>

using namespace std;

//-------- version-stamped field-----------
// stores the history of a single field (left, right, or key) across versions

template <typename T>

struct VersionedField
{
    vector<pair<int, T>> history; // [(version, value), ...]

    // Write a new value at a given version
    void set(int version, T value)
    {
        history.push_back({version, value});
    }

    // Read the value visible at a given version
    T get(int version) const
    {
        T result = T();
        for (auto &[v, val] : history)
        {
            if (v <= version)
                result = val;
            else
                break;
        }
        return result;
    }
};

// ----- Fat Node---------
// one physical node that stores the full history of its key and children
struct FatNode
{
    int nodeID;
    VersionedField<int> key;
    VersionedField<FatNode *> left;
    VersionedField<FatNode *> right;
    VersionedField<bool> deleted; // tombston flag per version

    FatNode(int id, int k, int version)
    {
        nodeID = id;
        key.set(version, k);
        left.set(version, nullptr);
        right.set(version, nullptr);
        deleted.set(version, false);
    }

    // Getters for a specific v
    int getKey(int v) const { return key.get(v); }
    FatNode *getLeft(int v) const { return left.get(v); }
    FatNode *getRight(int v) const { return right.get(v); }
    bool isDeleted(int v) const { return deleted.get(v); }
};

// ------------- Persistent BST

class PersistentBST
{
private:
    int currentV;
    vector<FatNode *> roots; // roots[v] = root of verstion v
    int nodeCounter;

    // ------- Insert -----------------------------
    // Walks existing fat nodes, stamps new child pointer or creates new nodes
    FatNode *insert(FatNode *node, int key, int v)
    {
        if (!node)
        {
            return new FatNode(nodeCounter++, key, v);
        }

        if (key == node->getKey(v)) // dup - no change
            return node;

        if (key < node->getKey(v))
        {
            FatNode *newLeft = insert(node->getLeft(v), key, v);
            node->left.set(v, newLeft); // stamp new left into history
            return node;
        }
        else
        {
            FatNode *newRight = insert(node->getRight(v), key, v);
            node->right.set(v, newRight); // stamp new right into history
            return node;
        }
    }

    void inorder(FatNode *node, int version) const
    {
        if (!node || node->isDeleted(version))
            return;
        inorder(node->getLeft(version), version);
        cout << node->getKey(version) << " ";
        inorder(node->getRight(version), version);
    }

public:
    PersistentBST() : currentV(0), nodeCounter(0)
    {
        roots.push_back(nullptr); // V0 = empty tree
    }

    int insert(int key)
    {
        currentV++;
        FatNode *newRoot = insert(roots[currentV - 1], key, currentV);
        roots.push_back(newRoot);
        return currentV;
    }

    void inorder(int version = -1) const
    {
        int v1 = (version == -1) ? currentV : version;
        cout << "v" << v1 << ": ";
        inorder(roots[v1], v1);
        cout << endl;
    }
};

int main()
{
    PersistentBST tree;

    int v1 = tree.insert(30);
    int v2 = tree.insert(10);
    int v3 = tree.insert(50);
    int v4 = tree.insert(20);
    int v5 = tree.insert(40);

    cout << "--- After inserts ---" << endl;
    tree.inorder(v1); // v1: 30
    tree.inorder(v2); // v2: 10 30
    tree.inorder(v3); // v3: 10 30 50
    tree.inorder(v4); // v4: 10 20 30 50
    tree.inorder(v5); // v5: 10 20 30 40 50
}