#include <iostream>
#include <memory>

using namespace std;

template <typename T>
struct Node {
    T value;
    shared_ptr<Node<T>> next;

    Node(T val, shared_ptr<Node<T>> nxt = nullptr)
    {
        value = val;
        next = nxt;
    }
};

template <typename T>
shared_ptr<Node<T>> reverse(shared_ptr<Node<T>> head)
{
    shared_ptr<Node<T>> result = nullptr;

    while (head)
    {
        result = make_shared<Node<T>>(head->value, result);
        head = head->next;
    }

    return result;
}

template <typename T>
class persistentQueue
{
    private:
        shared_ptr<Node<T>> front;
        shared_ptr<Node<T>> rear;
        int frontSize;
        int rearSize;

        persistentQueue balance()
        {
            // maintain the invariant
            if(rearSize <= frontSize)
                return *this;

            // trigger expensive operation
              // step 1 - reverse the rear stack
            auto newFront = front;
            auto reversedRear = reverse(rear);
              // step 2 - add the reversed list to the front stack
            auto temp = reversedRear;
            while(temp){
                newFront = make_shared<Node<T>>(temp->value, newFront);
                temp = temp->next;
            }

            return persistentQueue(newFront, nullptr, frontSize + rearSize, 0);
        }

    public:
        persistentQueue(){
            front = nullptr;
            rear = nullptr;
            frontSize = 0;
            rearSize = 0;
        }

        persistentQueue(shared_ptr<Node<T>> f, shared_ptr<Node<T>> r, int fsize, int rsize)
        {
            front = f;
            rear = r;
            frontSize = fsize;
            rearSize = rsize;
        }

        bool isEmpty()
        {
            return frontSize == 0;
        }

        persistentQueue enqueue(T x)
        {
            auto newRear = make_shared<Node<T>>(x, rear);
            return persistentQueue(front, newRear, frontSize, rearSize + 1).balance();
        }

        persistentQueue dequeue()
        {
            if (isEmpty())
                throw runtime_error("Queue is empty");

            return persistentQueue(front->next, rear, frontSize - 1, rearSize).balance();
        }

        T peek()
        {
            if (isEmpty())
                throw runtime_error("Queue is empty");
            
            return front->value;
        }

        void print() 
        {
            auto temp = front;
            while (temp) {
                cout << temp->value << " ";
                temp = temp->next;
            }
        
            auto revRear = reverse(rear);
            temp = revRear;
            while (temp) {
                cout << temp->value << " ";
                temp = temp->next;
            }

            cout << endl;
        }

};

int main()
{
    persistentQueue<int> t1;

    auto t2 = t1.enqueue(10);
    auto t3 = t2.dequeue();
    auto t4 = t3.enqueue(30);
    auto t5 = t4.enqueue(40);
    auto t6 = t5.dequeue();

    t2.print();
    t3.print();
    t4.print();
    t5.print();
    t6.print();

return 0;
}