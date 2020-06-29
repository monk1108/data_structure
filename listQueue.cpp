//队列的单链表实现
#include <iostream>

using namespace std;

template <class T>
class listQueue
{
private:
    struct node
    {
        T data;
        node *next;

        node(const T &x, node *N=NULL){
            data = x;
            next = N;
        }
        node():next(NULL){}
        ~node(){}
    };

    node *front, *rear;

public:
    listQueue();
    ~listQueue();
    void enQueue(const T &x);
    T deQueue();
    bool isempty() const;
    T gethead() const;
};

template <class T>
listQueue<T>::listQueue(){
    front = rear = NULL;
}

template <class T>
listQueue<T>::~listQueue(){
    node *tmp;

    while(front != NULL){
        tmp = front;
        front = front->next;
        delete tmp;
    }
}

template <class T>
void listQueue<T>::enQueue(const T &x){
    if(rear == NULL){
        front = rear = new node(x);
    }
    else{
        node *tmp = new node(x);
        rear = rear->next = tmp;
    }
}

template <class T>
T listQueue<T>::deQueue(){
    node *tmp = front;
    front = front->next;
    if(front == NULL)  rear = NULL;//出队后为空队列

    T meow = tmp->data;
    delete tmp;
    return meow;
}

template <class T>
bool listQueue<T>::isempty()const{
    return front == NULL;
}

template <class T>
T listQueue<T>::gethead() const{
    return front->data;
}