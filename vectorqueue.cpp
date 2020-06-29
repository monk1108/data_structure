//队列的数组实现（循环数组）
#include <iostream>

using namespace std;

template <class T>
class seqQueue
{
private:
    T *data;
    int front, maxsize, length;

    void doubleSpace();

public:
    seqQueue(int size=10){
        data = new T[size];
        maxsize = size;
        front = -1;//front指向的位置不存放数据元素
        length = 0;
    }

    ~seqQueue(){
        delete []data;
    }

    bool isempty(){
        return length == 0;
    }

    T gethead(){
        return data[(front + 1) % maxsize];
    }

    void enQueue(const T &x);

    T deQueue();

};

template <class T>
void seqQueue<T>::doubleSpace(){
    T *tmp;

    tmp = data;
    data = new T [2 * maxsize];
    for(int i = 0; i < maxsize; i++)  data[i] = tmp[(front + 1 + i) % maxsize];

    front = -1;
    maxsize *= 2;
    delete tmp;
}

template <class T>
void seqQueue<T>::enQueue(const T &x){
    if(length == maxsize)  doubleSpace();

    length++;
    data[(front + length) % maxsize] = x;
}

template <class T>
T seqQueue<T>::deQueue(){
    front = (front + 1) % maxsize;
    length--;
    return data[front];
}