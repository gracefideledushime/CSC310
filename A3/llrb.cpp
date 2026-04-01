#include "llrb.h"

// ++++++++++++++++++++++++++++++ PUBLIC +++++++++++++++++++++++++++++++++++++++

RBTREE::RBTREE()
{
    TNULL = new Node(0);
    TNULL->color = BLACK;
    TNULL->left = TNULL;
    TNULL->right = TNULL;
    TNULL->parent = TNULL;

    root = TNULL;
}

RBTREE::~RBTREE()
{
    deleteSubtree(root);
}

void RBTREE::insert(int key)
{
    Node *node = nullptr;
    node = new Node(key);

    node->left = TNULL;
    node->right = TNULL;

    if (root == TNULL)
    {
        root = node;
        root->color = BLACK;
        return;
    }

    insertR(root, node);
    insertFix(node);
}

void RBTREE::remove(int key)
{
    removeR(root, key);
}

void RBTREE::printTree()
{
    if (root == TNULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }
    print(root, 0);
}

// ++++++++++++++++++++++++++++++ PRIVATE +++++++++++++++++++++++++++++++++++++++++++++

void RBTREE::leftRotate(Node *y)
{
    Node *x = y->right;
    if (x == TNULL || x == nullptr)
        return;         // Prevent invalid rotation
    y->right = x->left; // adoption

    if (x->left != TNULL) // updating parent of adopted left child
        x->left->parent = y;

    // update parent of x
    x->parent = y->parent;
    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->left = y;
    y->parent = x;
}

void RBTREE::rightRotate(Node *y)
{
    Node *x = y->left;
    if (x == TNULL || x == nullptr)
        return;         // Prevent invalid rotation
    y->left = x->right; // adoption

    if (x->right != TNULL) // updating parent of adopted left child
        x->right->parent = y;

    // update parent of x
    x->parent = y->parent;
    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void RBTREE::insertR(Node *&root, Node *&node)
{
    if (root == TNULL)
    {
        root = node;
        return;
    }

    if (node->data < root->data)
    {
        if (root->left == TNULL)
        {
            root->left = node;
            node->parent = root;
        }
        else
        {
            insertR(root->left, node);
        }
    }
    else
    {
        if (root->right == TNULL)
        {
            root->right = node;
            node->parent = root;
        }
        else
        {
            insertR(root->right, node);
        }
    }
}

void RBTREE::insertFix(Node *k)
{
    cout << "STARTING";
    Node *sibling;
    while (k != root && k->parent->color == RED)
    {
        cout << "HERE?";

        if (k == k->parent->left)
        {
            if (k->parent->right != nullptr)
            {
                cout << "HALLOO";
                sibling = k->parent->right;
                if (sibling == nullptr)
                {
                    cout << "good?";
                    return;
                }
                if (k->color == BLACK && sibling->color == RED)
                {
                    // Case 1: Red link is on the right – rotate left and swap colors
                    leftRotate(k->parent);
                    k->color = RED;
                    sibling->color = BLACK;
                }
                // Case 3: Two red children – flip the colors
                if (k->color == RED && sibling->color == RED)
                {
                    k->color = RED;
                    sibling->color = BLACK;
                    k = k->parent;
                }
            }
            // Case 2: Two consecutive red nodes on the left – rotate right and swap colors
            if (k->color == RED)
            {
                rightRotate(k->parent);
                k->color = BLACK;
                k->parent->color = BLACK;
                k = k->parent;
            }
        }
    }

    root->color = BLACK;
}

void RBTREE::removeR(Node *&node, int key)
{
    if (node == TNULL)
        return;
    if (key < node->data)
        removeR(node->left, key);
    else if (key > node->data)
        removeR(node->right, key);
    else
    {                   // found node to be deleted
        Node *z = node; // node to delete
        Node *y = node; // successor if any
        Node *x;
        Color y_original_color = node->color;

        if (node->left == TNULL && node->right != TNULL)
        {
            x = node->right;
            transplant(node, node->right);
        }
        else if (node->left != TNULL && node->right == TNULL)
        {
            x = TNULL;
            transplant(node, TNULL);
        }
        else
        { // CASE 3
            y = successor(y->right);
            y_original_color = y->color;
            x = y->right; // to avoid orphans

            if (y->parent != node)
            {
                transplant(y, y->right);

                y->right = node->right;
                y->right->parent = y;
            }
            y->left = node->left;
            y->left->parent = y;

            transplant(node, y);
            y->color = node->color;
        }
        if (y_original_color == BLACK)
        {
            deleteFix(x);
        }
        delete z; // free the memory of the detached node
        x->color = BLACK;
    }
}

void RBTREE::deleteFix(Node *x)
{
    Node *sibling;

    while (x != root && x->color == BLACK)
    {
        // if x is a left child
        if (x == x->parent->left)
        {
            sibling = x->parent->right;
            // CASE 1
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                sibling = x->parent->right;
            }

            // CASE 2
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                x = x->parent;
            }

            else
            {
                // CASE 3
                if (sibling->right->color == BLACK)
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(sibling);
                    sibling = x->parent->right;
                }
                // CASE 4
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        // if x is a right child
        else
        {
            sibling = x->parent->left;
            // CASE 1
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                sibling = x->parent->left;
            }

            // CASE 2
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                x = x->parent;
            }

            else
            {
                // CASE 3
                if (sibling->right->color == BLACK)
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    leftRotate(sibling);
                    sibling = x->parent->left;
                }
                // CASE 4
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                sibling->right->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }

        // CASE 1
        if (sibling->color == RED)
        {
            sibling->color = BLACK;
            x->parent->color = RED;
            leftRotate(x->parent);
            rotations++;
            sibling = x->parent->right;
        }
        // CASE 2
        if (sibling->left->color == BLACK && sibling->right->color == BLACK)
        {
            sibling->color = RED;
            x = x->parent;
        }
        else
        { // CASE 3
            if (sibling->right->color == BLACK)
            {
                sibling->left->color = BLACK;
                sibling->color = RED;
                rightRotate(sibling);
                rotations++;
                sibling = x->parent->right;
            } // CASE 4
            sibling->color = x->parent->color;
            x->parent->color = BLACK;
            sibling->right->color = BLACK;
            leftRotate(x->parent);
            rotations++;
            x = root;
        }
    }
    x->color = BLACK;
}

void RBTREE::transplant(Node *&u, Node *&v)
{
    if (u->parent == nullptr)
    {
        root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != TNULL)
        v->parent = u->parent;
}

Node *RBTREE::successor(Node *node)
{
    if (node == TNULL)
        return TNULL;
    while (node->left != TNULL)
    {
        node = node->left;
    }
    return node;
}

void RBTREE::deleteSubtree(Node *node)
{
    if (node != TNULL)
    {
        deleteSubtree(node->left);
        deleteSubtree(node->right);
        delete node;
    }
}

void RBTREE::print(Node *root, int space)
{
    if (root == TNULL || root == nullptr)
        return;

    space += 10;
    print(root->right, space);

    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";

    string color_code = (root->color == RED) ? RED_COLOR : BLACK_COLOR;
    cout << color_code << root->data << RESET_COLOR;
    cout << endl;
    print(root->left, space);
}
