//优先级队列是顺序存储的，即将一颗完全二叉树按层次遍历的次序存储在数组中。
//位置0置空，把根放在位置1
//下面是最小化堆的实现

#include <iostream>

using namespace std;

template <class T>
class priorityQueue
{
private:
    int currentSize;//队列长度
    T *array;//数据从array[0]开始存储，array[0]空着
    int maxSize;//容量

    void doubleSpace();
    void percolateDown(int hole);//向下过滤
    void buildHeap();

public:
    priorityQueue(int capacity = 100);
    priorityQueue(const T data[], int size);
    ~priorityQueue();
    bool isempty() const;
    void enQueue(const T &x);
    T deQueue();//有返回值
    void pop();//无返回值
    T getHead() const;
    int getSize()；
};


template <class T>
priorityQueue<T>::priorityQueue(int capacity)
{
    currentSize = 0;
    array = new T [capacity];
    maxSize = capacity;
}

template <class T>
priorityQueue<T>::~priorityQueue()
{
    delete []array;
}

template <class T>
bool priorityQueue<T>::isempty() const{
    return currentSize == 0;
}

template <class T>
T priorityQueue<T>::getHead() const{
    return array[1];
}

template <class T>
void priorityQueue<T>::enQueue(const T &x){
    //平均情况下，时间复杂度是常量；最坏情况下时间复杂度是O(logN)
    if(currentSize == maxSize - 1)  doubleSpace();

    int hole = ++currentSize;
    //从最底部开始向上过滤
    for(; hole > 1 && x < array[hole/2]; hole /= 2)  array[hole] = array[hole/2];
    //此时的hole值为新插入节点的位置
    array[hole] = x;
}

template <class T>
T priorityQueue<T>::deQueue(){
    //时间复杂度是O(logN)
    T min;
    min = array[1];
    array[1] = array[currentSize--];//将层次遍历时的最后一个节点放进根节点
    percolateDown(1);//向下过滤
    return min;
}

template <class T>
void priorityQueue<T>::pop()
{
    //无返回值的deQueue()
    array[1] = array[currentSize--];//将层次遍历时的最后一个节点放进根节点
    percolateDown(1);//向下过滤
}

template <class T>
void priorityQueue<T>::percolateDown(int hole){
    //percolateDown()就是让tmp在正确的位置上
    int child;
    T tmp = array[hole];//暂存层次遍历时的最后一个节点
    

    //向下过滤
    for(; hole*2 <= currentSize; hole = child){
        child = 2 * hole;
        if(child != currentSize && array[child + 1] < array[child])  child++;
        //选定左右儿子中较小的一个
        if(array[child] < tmp)  array[hole] = array[child];
        else  break;
    }
    array[hole] = tmp;
}

template <class T>
void priorityQueue<T>::buildHeap(){
    //以逆向层次的顺序对节点调用percolateDown(叶节点不用)，使其符合堆的有序性
    for(int i = currentSize/2; i > 0; i--)  percolateDown(i);
}

template <class T>
priorityQueue<T>::priorityQueue(const T *items, int size){
    maxSize = size + 10;//避免一开始就doubleSpace()
    currentSize = size;

    array = new T [maxSize];
    for(int i = 0; i < size; i++)  array[i + 1] = items[i];
    buildHeap();
}

template <class T>
void priorityQueue<T>::doubleSpace(){
    T *tmp = array;
    maxSize *= 2;
    array = new T[maxSize];
    for(int i = 0; i < currentSize; i++){
        array[i] = tmp[i];
    }
    delete tmp;
}

template <class T>
int priorityQueue<T>::getSize()
{
    return currentSize;
}
