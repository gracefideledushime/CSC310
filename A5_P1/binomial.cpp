#include "binomial.h"
#include <climits>

binomialNode::binomialNode(int k)
{
    key = k;
    degree = 0;
    parent = nullptr;
    sibling = nullptr;
    child = nullptr;
}

// ---------------------- PRIVATE FUNCTIONS ------------------------------
binomialNode *BinomialHeap::unionHeap(binomialNode *heap1, binomialNode *heap2)
{
    if (!heap1)
        return heap2;
    if (!heap2)
        return heap1;

    binomialNode *newHead = nullptr;
    binomialNode **pos = &newHead;

    while (heap1 && heap2)
    {
        if (heap1->degree <= heap2->degree)
        {
            *pos = heap1;
            heap1 = heap1->sibling;
        }
        else
        {
            *pos = heap2;
            heap2 = heap2->sibling;
        }
        pos = &((*pos)->sibling);
    }
    // add remaining trees, if any
    *pos = (heap1) ? heap1 : heap2;

    return newHead;
}

binomialNode *BinomialHeap::mergeTrees(binomialNode *tree1, binomialNode *tree2)
{
    // assume tree 1 is the parent
    // tree 1 has the smallest root
    // to maintain min heap property
    if (tree1->key > tree2->key)
    {
        swap(tree1, tree2);
    }
    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}

void BinomialHeap::linkTrees(binomialNode *&prev, binomialNode *&cur, binomialNode *&next)
{
    if (cur->degree != next->degree || next->sibling && next->sibling->degree == cur->degree)
    {
        prev = cur;
        cur = next;
    }
    else
    {
        if (cur->key <= next->key)
        {
            cur->sibling = next->sibling;
            mergeTrees(cur, next);
        }
        else
        {
            if (!prev)
                head = next;
            else
                prev->sibling = next;
            mergeTrees(next, cur);
            cur = next;
        }
    }
}

// ---------------- PUBLIC FUNCTIONS ------------------------------
BinomialHeap::BinomialHeap()
{
    head = nullptr;
}

void BinomialHeap::insert(int key)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(key);
    merge(tempHeap);
}

void BinomialHeap::merge(BinomialHeap &other)
{
    head = unionHeap(head, other.head);
    if (!head || !head->sibling)
        return;

    binomialNode *prev = nullptr;
    binomialNode *cur = head;
    binomialNode *next = cur->sibling;

    while (next)
    {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }
}

binomialNode *BinomialHeap::findMin()
{
    if (!head)
        return nullptr;

    binomialNode *minNode = head;
    int minKey = head->key;

    binomialNode *walker = head->sibling;
    while (walker)
    {
        if (walker->key < minKey)
        {
            minKey = walker->key;
            minNode = walker;
        }
        walker = walker->sibling;
    }
    return minNode;
}

void BinomialHeap::printHeap()
{
    cout << "Binomial Heap:\n";
    binomialNode *current = head;

    while (current != nullptr)
    {
        cout << endl
             << "Tree of degree " << current->degree << endl;
        printTree(current, 0);
        current = current->sibling;
    }
}

void BinomialHeap::printTree(binomialNode *node, int space)
{
    if (node == nullptr)
        return;

    cout << setw(space * 2) << node->key << endl;

    binomialNode *child = node->child;
    while (child)
    {
        printTree(child, space + 3);
        child = child->sibling;
    }
}

binomialNode *BinomialHeap::findNode(binomialNode *h, int key)
{
    if (!h)
        return nullptr;

    if (h->key == key)
        return h;

    binomialNode *found = findNode(h->child, key);
    if (found)
        return found;

    return findNode(h->sibling, key);
}

void BinomialHeap::deleteMin()
{
    if (!head)
        return;

    // 1. find min node and its predecessor
    binomialNode *minPrev = nullptr;
    binomialNode *minNode = head;
    binomialNode *prev = nullptr;
    binomialNode *cur = head;

    while (cur)
    {
        if (cur->key < minNode->key)
        {
            minNode = cur;
            minPrev = prev;
        }
        prev = cur;
        cur = cur->sibling;
    }

    // 2. splice minNode out of the root list
    if (minPrev)
        minPrev->sibling = minNode->sibling;
    else
        head = minNode->sibling;

    // 3. reverse children into a temporary heap
    BinomialHeap childHeap;
    binomialNode *child = minNode->child;
    while (child)
    {
        binomialNode *next = child->sibling;
        child->sibling = childHeap.head;
        child->parent = nullptr;
        childHeap.head = child;
        child = next;
    }

    // 4. merge handles union + consolidation for us
    merge(childHeap);

    delete minNode;
}

void BinomialHeap::decreaseKey(int oldKey, int newKey)
{
    if (newKey > oldKey)
        return; // can only decrease

    binomialNode *node = findNode(head, oldKey);
    if (!node)
        return;

    node->key = newKey;

    // bubble up: swap keys with parent until heap order is restored
    binomialNode *cur = node;
    binomialNode *par = cur->parent;
    while (par && cur->key < par->key)
    {
        swap(cur->key, par->key);
        cur = par;
        par = cur->parent;
    }
}

void BinomialHeap::deleteKey(int key)
{
    binomialNode *node = findNode(head, key);
    if (!node)
        return;

    // force the target node to become the minimum, then delete it
    decreaseKey(key, INT_MIN);
    deleteMin();
}
