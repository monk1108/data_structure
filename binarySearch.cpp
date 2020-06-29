//静态查找表，二分查找
//时间复杂度是O(logN)
#include <iostream>

using namespace std;

template <class KEY, class OTHER>
struct SET
{
    KEY key;
    OTHER other;
};

template <class KEY, class OTHER>
int binarySearch(SET<KEY, OTHER> data[], int size, const KEY &x){
    int low = 1;
    int high = size, mid;
    while(low <= high){
        mid = (high + low) / 2;
        if(x == data[mid].key)  return mid;
        if(x < data[mid].key)  high = mid - 1;
        else  low = mid + 1;  
    }

    return 0; //没找到
}
