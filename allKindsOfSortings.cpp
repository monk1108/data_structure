#include <iostream>

using namespace std;

template <class KEY, class OTHER>
struct SET
{
    KEY key;
    OTHER other;
};

//直接插入排序, O(n^2)
template <class KEY, class OTHER>
void simpleInsertSort(SET<KEY, OTHER> a[], int size)
{
    int k;
    SET<KEY, OTHER> tmp;

    for(int j = 0; j < size; j++){
        tmp = a[j];//待排序的元素
        for(int k = j - 1; k >= 0 && a[k].key > tmp.key; --k){
            a[k + 1] = a[k];
        }
        a[k + 1] = tmp;
    }
}

//希尔排序，最坏O(n^2)，平均O(n^1.5)
template <class KEY, class OTHER>
void shellSort(SET<KEY, OTHER> a[], int size)
{
    int step, i, j;
    SET<KEY, OTHER> tmp;

    for(step = size/2; step > 0; step /= 2){
        //step为希尔增量
        for(i = step; i < size; i++){
            //对相距step的元素序列直接插入排序
            tmp = a[i];//待排序元素
            for(j = i - step; j >= 0 && a[j].key > tmp.key; j -= step){
                a[j + step] = a[j];
            }
            a[j + step] = tmp;
        }
    }
}

//直接选择排序，选择最小元素时用顺序查找
template <class KEY, class OTHER>
void simpleSelectSort(SET<KEY, OTHER> a[], int size)
{
    int i, j, min;
    SET<KEY, OTHER> tmp;

    for(i = 0; i < size-1; i++){
        min = i;
        for(j = i + 1; j < size; j++){
            if(a[j].key < a[min].key)  min = j;
        }
        tmp = a[i];
        a[i] = a[min];
        a[min] = tmp;
    }
}

//堆排序（最终得到递增序列，用最大堆），O(NlogN)
template <class KEY, class OTHER>
void heapSort(SET<KEY, OTHER>a[], int size)
{
    int i;
    SET<KEY, OTHER> tmp;

    //创建初始的堆
    for(i = size/2 - 1; i >= 0; i--)  percolateDown(a, i, size);

    //执行n-1次deQueue
    for(i = size - 1; i > 0; --i){
        tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;//删除a[0]
    }
}

template <class KEY, class OTHER>
void percolateDown(SET<KEY, OTHER> a[], int hole, int size)
{
    int child;
    SET<KEY, OTHER> tmp = a[hole];

    for(; hole*2 + 1 < size; hole = child){
        child = hole *2 + 1;
        if(child != size - 1 && a[child+1].key > a[child].key)  child++;
        if(a[child].key > tmp.key)  a[hole] = a[child];
        else break;
    }
    a[hole] = tmp;
}

//冒泡排序，O(N^2)
template <class KEY, class OTHER>
void bubbleSort(SET<KEY, OTHER> a[], int size)
{
    int i, j;
    SET<KEY, OTHER> tmp;
    bool flag = true;

    for(i = 1; i < size && flag; ++i){
        flag = false;
        for(j = 0; j < size - i; j++){
            if(a[j + 1].key < a[j].key){
                tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                flag = true;
            }
        }
    }
}

//快速排序，O(NlogN)
template <class KEY, class OTHER>
void quickSort(SET<KEY, OTHER> a[], int low, int high)
{
    int mid;
    if(low >= high)  return;//待分段的元素只有0个或1个，递归终止
    mid = divide(a, low, high);
    quickSort(a, low, mid - 1);
    quickSort(a, mid + 1, high);
}

//包裹函数
template <class KEY, class OTHER>
void quickSort(SET<KEY, OTHER> a[], int size)
{
    quickSort(a, 0, size -1);
}

//划分函数
template <class KEY, class OTHER>
int divide(SET<KEY, OTHER> a[], int low, int high)
{
    SET<KEY, OTHER> k = a[low];
    do{
        while(low < high && a[high].key >= k.key)  --high;
        if(low < high){
            a[low] = a[high];
            ++low;
        }
        while(low < high && a[low].key <= k.key)  ++low;
        if(low < high){
            a[high] = a[low];
            --high;
        }
    }while(low < high);

    a[low] = k;
    return low;
    
}

//归并排序，O(NlogN)
template <class KEY, class OTHER>
void mergeSort(SET<KEY, OTHER> a[], int size)
{
    //包裹函数
    mergeSort(a, 0, size - 1);
}

template <class KEY, class OTHER>
void mergeSort(SET<KEY, OTHER> a[], int left, int right)
{
    int mid = (left + right) / 2;

    if(left == right)  return;
    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    merge(a, left, mid + 1, right);
}

template <class KEY, class OTHER>
void merge(SET<KEY, OTHER> a[], int left, int mid, int right)
{
    SET<KEY, OTHER> *tmp = new SET<KEY, OTHER> [right - left + 1];
    int i = left, j = mid, k = 0;
    
    while(i < mid && j <= right){
        //两表都未结束
        if(a[i].key < a[j].key)  tmp[k++] = a[i++];
        else  tmp[k++] = a[j++];
    }

    while(i < mid)  tmp[k++] = a[i++];//前半部分未结束
    while(j <= right)  tmp[k++] = a[j++];//后半部分未结束

    for(i = 0, k = left; k <= right; )  a[k++] = tmp[i++];
    delete []tmp;
}


//口袋排序，O(len*n)，最大的数据是len位数。
template <class OTHER>
struct node
{
    SET<int, OTHER> data;
    node *next;

    node(){next = NULL;}
    node(SET<int, OTHER> d):data(d){next = NULL;}
};

template <class OTHER>
void bucketSort(node<OTHER> *&p)
{
    //p是链表头
    node<OTHER> *bucket[10], *last[10], *tail;//bucket, last:10个口袋的头和尾
    int i, j, k, base = 1, max = 0, len = 0;//base是分配的基数

    for(tail = p; tail != NULL; tail = tail->next){
        //找最大键值
        if(tail->data.key > max)  max = tail->data.key;
    }//此时tail指向待排序的链表尾

    //寻找最大键值的位数
    if(max == 0)  len == 0;
    else  while(max > 0){
        ++len;
        max /= 10;
    }

    for(i = 1; i <= len; i++){
        //执行len次分配和重组
        for(j = 0; j <= 9; j++){
            bucket[j] = last[j] = NULL;
            //清空口袋
        }
        while(p != NULL){
            //执行一次分配
            k = p->data.key / base % 10;//k为节点所在的口袋
            if(bucket[k] == NULL)  bucket[k] = last[k] = p;//此口袋原来是空的
            else  last[k] = last[k]->next = p;//此口袋非空
            p = p->next;
        }

        p = NULL;//分配完成
        for(j = 0; j <= 9; ++j){
            //执行重组（倒出口袋）
            if(bucket[j] == NULL)  continue;
            if(p == NULL)  p = bucket[j];
            else  tail->next = bucket[j];
            tail = last[j];//此句不论if还是else都要执行
        }
        tail->next = NULL;//表尾置空
        base *= 10;//准备下一次分配
    }
}

