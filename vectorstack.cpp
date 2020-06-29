//栈的数组实现
#include <iostream>

using namespace std;

template <class T>
class seqStack
{
private:
    T *data;
    int top_p, maxsize;
    void doubleSpace();

public:
    seqStack(int size = 10);
    ~seqStack();
    bool isempty() const;
    void push(const T &x);
    T top() const;
    T pop();
};

template <class T>
seqStack<T>::seqStack(int size){
    data = new T [size];
    maxsize = size;
    top_p = -1;
}

template <class T>
seqStack<T>::~seqStack(){
    delete [] data;
}

template <class T>
bool seqStack<T>::isempty() const{
    return top_p == -1;
}

template <class T>
void seqStack<T>::push(const T &x){
    if(top_p == maxsize - 1)  doubleSpace();

    data[++top_p] = x;
}

template <class T>
T seqStack<T>::top() const{
    return data[top_p];
}

template <class T>
T seqStack<T>::pop(){
    return data[top_p--];
}

template <class T>
void seqStack<T>::doubleSpace(){
    T *tmp = data;

    data = new T [maxsize * 2];
    for(int i = 0; i < maxsize; ++i){
        data[i] = tmp[i];
    }
    maxsize *= 2;
    delete [] tmp;
}

