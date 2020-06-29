//链接栈
#include <iostream>

using namespace std;

template <class T>
class linkStack{
private:
    struct node{
        T data;
        node *next;

        node():next(NULL){}
        node(const T &d, node *n = NULL):data(d), next(n){}
        ~node(){}
    };

    node *top_p;//指向栈顶

public:
    linkStack();
    ~linkStack();
    bool isempty() const;
    void push(const T &x);
    T pop();
    T top() const;
};

template <class T>
linkStack<T>::linkStack(){
    top_p = NULL;
}

template <class T>
linkStack<T>::~linkStack(){
    node *tmp;
    while(top_p != NULL){
        tmp = top_p;
        top_p = top_p->next;
        delete tmp;
    }
}

template <class T>
bool linkStack<T>::isempty() const{
    return top_p == NULL;
}

template <class T>
void linkStack<T>::push(const T &x){
    top_p = new node(x, top_p);
}

template <class T>
T linkStack<T>::pop(){
    node *tmp;
    T x=  tmp->data;
    top_p = tmp->next;
    delete tmp;
    return x;
}

template <class T>
T linkStack<T>::top() const{
    return top_p->data;
}