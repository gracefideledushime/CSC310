// persistent BST
// path copying
// shared pointers - keep track of refcount  for delete()
// thread-safe

#include <iostream>
#include <memory>

using namespace std;

struct Node {
    int key;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
    // int refcount;  it is not thread-safe 

    Node(int k, shared_ptr<Node> l = nullptr, shared_ptr<Node> r = nullptr)
    {
        key = k;
        left = l;
        right = r;
    }
};

class persistentBST {
    private:
        shared_ptr<Node> root;

        shared_ptr<Node> insert(shared_ptr<Node> node, int key)
        {
            if(!node)
                return make_shared<Node>(key);
            
            if(key < node->key)
            {
                return make_shared<Node>(node->key, insert(node->left, key), node->right);
            } else if (key > node->key)
            {
                return make_shared<Node>(node->key, node->left, insert(node->right, key));
            } else {
                return node;
            }
        }

        bool search(shared_ptr<Node> node, int key){
            if (!node)
                return false;
            if(key == node->key)
                return true;
            if (key < node->key)
                return search (node->left, key);
            return search(node->right, key);
        }

        void inorder(shared_ptr<Node> node)
        {
            if(!node)
                return;
            inorder(node->left);
            cout << node->key << " ";
            inorder(node->right);
        }

    public:
        persistentBST(){
            root = nullptr;
        }
        persistentBST(shared_ptr<Node> r){
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
            cout<<endl;
        }
};

int main()
{
    persistentBST t1;

    auto t2 = t1.insert(10);
    auto t3 = t2.insert(20);
    auto t4 = t3.insert(30);
    auto t5 = t4.insert(40);

    cout << "t3: ";
    t3.inorder();

    cout << "t5: ";
    t5.inorder();

return 0;
}