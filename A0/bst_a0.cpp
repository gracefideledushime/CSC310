#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* leftChild = NULL;
    Node* rightChild = NULL;
};

class BST{
    public:
        Node* root;
        int nodeCount;    

    public:
        BST(Node* r): root(r){
                root = r;
                nodeCount = 0;
        }

        void printBST(Node* root){
            if(root == nullptr) {
                // cout << "Emptyyy" << endl;
                return;
            }
            printBST(root->leftChild);
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
    };



int main(){
    Node* newNode = new Node;
    BST bst(newNode);
    // bst.printBST(bst.root);
    bst.root = bst.insert(bst.root, 5); //root = 5
    bst.root = bst.insert(bst.root, 3); //root = 
    bst.root = bst.insert(bst.root, 9); //root = 5
    bst.root = bst.insert(bst.root, 8); //root = 5
    bst.root = bst.insert(bst.root, 10); //root = 5
    bst.root = bst.insert(bst.root, 2);
    // bst.printBST(bst.root);
    cout << endl << "count: " << bst.nodeCount << endl;
    
    bst.root = bst.del(bst.root, 10);
    bst.root = bst.del(bst.root, 9);
    bst.printBST(bst.root);

    cout << endl << "count: " << bst.nodeCount << endl;

    //search
    // cout << endl << bst.search(bst.root, 6)->data << endl;
    return 0;
}