#include "binomial.h"

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

    head = unionHeap(head, tempHeap.head);

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

int BinomialHeap::findMin()
{
    if (!head)
        return -1;

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
    }
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

void BinomialHeap::decreaseKey(int oldKey, int newKey)
{
}
