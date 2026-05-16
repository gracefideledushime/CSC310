#include "binomialHeap.h"
#include <climits>

binomialNode::binomialNode(Edge e)
{
    edge = e;
    degree = 0;
    parent = nullptr;
    sibling = nullptr;
    child = nullptr;
}

// ---------------------- PRIVATE FUNCTIONS ------------------------------
binomialNode *BinomialHeap::unionHeaps(binomialNode *heap1, binomialNode *heap2)
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
    if (tree1->edge.weight > tree2->edge.weight)
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
        if (cur->edge.weight <= next->edge.weight)
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

void BinomialHeap::insert(Edge edge)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(edge);
    merge(tempHeap);
}

void BinomialHeap::merge(BinomialHeap &other)
{
    head = unionHeaps(head, other.head);
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

Edge BinomialHeap::findMin()
{
    if (!head)
        return Edge{-1, -1, INT_MAX};
    ;

    binomialNode *minNode = head;

    binomialNode *walker = head->sibling;
    while (walker)
    {
        if (walker->edge.weight < minNode->edge.weight)
        {
            minNode = walker;
        }
        walker = walker->sibling;
    }
    return minNode->edge;
}

void BinomialHeap::printHeap()
{
    cout << "Binomial Heap:\n";
    binomialNode *current = head;

    while (current != nullptr)
    {
        cout << endl
             << "Tree of degree " << current->degree << endl;
        printTsree(current, 0);
        current = current->sibling;
    }
}

void BinomialHeap::printTsree(binomialNode *node, int space)
{
    if (node == nullptr)
        return;

    cout << setw(space * 2) << node->edge.weight << endl;

    binomialNode *child = node->child;
    while (child)
    {
        printTsree(child, space + 3);
        child = child->sibling;
    }
}

binomialNode *BinomialHeap::findBinomialNode(binomialNode *h, Edge edge)
{
    if (!h)
        return nullptr;

    if (h->edge.src == edge.src && h->edge.dest == edge.dest) // or full Edge operator==
        return h;

    binomialNode *found = findBinomialNode(h->child, edge);
    if (found)
        return found;

    return findBinomialNode(h->sibling, edge);
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
        if (cur->edge.weight < minNode->edge.weight)
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

void BinomialHeap::decreaseKey(Edge oldEdge, Edge newEdge)
{
    if (newEdge.weight > oldEdge.weight)
        return;

    binomialNode *node = findBinomialNode(head, oldEdge);
    if (!node)
        return;

    node->edge = newEdge;

    binomialNode *cur = node;
    binomialNode *par = cur->parent;
    while (par && cur->edge.weight < par->edge.weight)
    {
        swap(cur->edge, par->edge);
        cur = par;
        par = cur->parent;
    }
}

void BinomialHeap::deleteKey(Edge edge)
{
    binomialNode *node = findBinomialNode(head, edge);
    if (!node)
        return;

    // sentinel minimum edge to force node to root
    Edge minSentinel = {-1, -1, INT_MIN};
    decreaseKey(edge, minSentinel);
    deleteMin();
}

binomialNode *BinomialHeap::reverseList(binomialNode *node)
{
    binomialNode *prev = nullptr;
    while (node)
    {
        binomialNode *next = node->sibling;
        node->sibling = prev;
        node->parent = nullptr;
        prev = node;
        node = next;
    }
    return prev;
}

bool BinomialHeap::isEmpty()
{
    return head == nullptr;
}
