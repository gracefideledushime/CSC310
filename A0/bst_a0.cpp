#include <iostream>
#include <cassert>
using namespace std;

struct Node {
    int data;
    Node* leftChild = nullptr;
    Node* rightChild = nullptr;
};

class BST{
    public:
        Node* root;
        int nodeCount;    

    public:
        BST(Node* r): root(r), nodeCount(0){}

        void printBST(Node* root){
            if(root == nullptr) {
                // cout << "Emptyyy" << endl;
                return;
            }
            printBST(root->leftChild);
            // cout << root << " -> " << root->data << endl;
            cout << root->data << endl;
            printBST(root->rightChild);
        }

        Node* search (Node* root, int key){
            if(root == nullptr) return nullptr;

            // check if the root has the matching value
            if (root->data == key) return root;
            Node* ans;
            //check if the key is less or greater than the root
            if(key < root->data){
                ans = search(root->leftChild, key);
            }else{
                ans = search(root->rightChild, key);
            }
            return ans;
        }

        Node* insert(Node* root, int data){            
            //if empty
            if(root == nullptr){
                //create a node
                Node* newNode = new Node;
                //assign it data
                newNode->data = data;
                nodeCount++;
                return newNode;
            }
            //traverse checking where it fits
            assert(root != nullptr);

            if(data < root->data){
                root->leftChild = insert(root->leftChild, data);
            }
            else{
                root->rightChild = insert(root->rightChild, data);
            }
            return root;
        }

        Node* del(Node* root, int data) {
            if (root == NULL) return NULL;
            assert(root != nullptr);

            if (data < root->data) {
                root->leftChild = del(root->leftChild, data);
            }
            else if (data > root->data) {
                root->rightChild = del(root->rightChild, data);
            }
            else {
                // FOUND NODE
                // Case 1: leaf
                if (root->leftChild == NULL && root->rightChild == NULL) {
                    delete root;
                    nodeCount--;
                    return NULL;
                }

                //Case 2: one child
                if (root->leftChild != NULL && root->rightChild == NULL) {
                    Node* candidate = root;
                    root = root->leftChild;
                    delete candidate;
                    nodeCount--;
                    return root;
                }

                if (root->rightChild != NULL && root->leftChild == NULL) {
                    Node* candidate = root;
                    root = root->rightChild;
                    delete candidate;
                    nodeCount--;
                    return root;
                }

                //case 3 - two children
                Node* succ = root->rightChild;
                while(succ->leftChild != NULL){
                    succ = succ->leftChild;
                }
                //copy smallest node data into root
                root->data = succ->data;
                root->rightChild = del(root->rightChild, succ->data);
            }

            return root;
        }

        void inorder_traversal(Node* root){
            if(root == nullptr) {
                // cout << "Emptyyy" << endl;
                return;
            }
            inorder_traversal(root->leftChild);
            cout << root->data << endl;
            inorder_traversal(root->rightChild);
        }

        void preorder_traversal(Node* root){
            if(root == nullptr) {
                // cout << "Emptyyy" << endl;
                return;
            }
            cout << root->data << endl;
            preorder_traversal(root->leftChild);
            preorder_traversal(root->rightChild);
        }

        void post_order_traversal(Node* root){
            if(root == nullptr) {
                return;
            }
            post_order_traversal(root->leftChild);
            post_order_traversal(root->rightChild);
            cout << root->data << endl;
        }

        int find_node_height(Node* root) {
            if (!root) return 0;

            int lh = find_node_height(root->leftChild);
            if (lh == -1) return -1;

            int rh = find_node_height(root->rightChild);
            if (rh == -1) return -1;

            if (abs(lh - rh) > 1) return -1;

            return 1 + max(lh, rh);
        }

        bool isBalanced(Node* root) {
            return find_node_height(root) != -1;
        }
    };



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