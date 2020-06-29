//列表的双链表实现
#include <iostream>

using namespace std;

template <class T>
class dlistQueue
{
private:
    struct node
    {
        T data;
        node *next, *prev;

        node(const T &x, node *N=NULL, node *H = NULL):data(x), next(N), prev(H){}
        node():next(NULL), prev(NULL){}
        ~node(){}
    };

    node *front, *rear;

public:
    dlistQueue();
    ~dlistQueue();
    bool isempty() const;
    void enQueue(const T &x);//入队
    T deQueue();//弹出表头元素，有返回值
    void pop_front();//弹出表头元素，无返回值
    void pop_rear();//弹出表尾元素，无返回值
    T gethead() const;//返回表头元素
    T getrear() const;//返回表尾元素
    bool isfrontempty()const;
    bool isrearempty()const;

};

template <class T>
bool dlistQueue<T>::isfrontempty()const{
    return front == NULL;
}

template <class T>
bool dlistQueue<T>::isrearempty()const{
    return rear == NULL;
}

template <class T>
dlistQueue<T>::dlistQueue(){
    front = rear = NULL;
}

template <class T>
dlistQueue<T>::~dlistQueue(){
    node *tmp;

    while(front != NULL){
        tmp = front;
        front = front->next;
        delete tmp;
    }
}

template <class T>
void dlistQueue<T>::enQueue(const T &x){
    if(rear == NULL){
        front = rear = new node(x);
    }
    else{
        node *tmp = new node(x);
        rear->next = tmp;
        tmp->prev = rear;
        rear = rear->next;
    }
}

template <class T>
T dlistQueue<T>::deQueue(){
    node *tmp = front;
    front = front->next;

    if(front == NULL)  rear = NULL;//出队后为空队列

    T meow = tmp->data;
    delete tmp;
    return meow;
}

template <class T>
bool dlistQueue<T>::isempty()const{
    return (front == NULL && rear == NULL);
}

template <class T>
T dlistQueue<T>::gethead() const{

    return front->data;
}

template <class T>
void dlistQueue<T>::pop_front(){
    node *tmp = front;
    if(front == rear){front = NULL; rear = NULL;}
    else  front = front->next;
    delete tmp;
}

template <class T>
T dlistQueue<T>::getrear() const{

    return rear->data;
}

template <class T>
void dlistQueue<T>::pop_rear(){
    node *tmp = rear;
    if(front == rear){front = NULL; rear = NULL;}
    else  rear = rear->prev;

    delete tmp;
}
