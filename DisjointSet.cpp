//不相交集
#include <iostream>

using namespace std;

class DisjointSet
{
private:
    int size;
    int *parent;

public:
    DisjointSet(int s);
    ~DisjointSet(){delete []parent;}
    void Union(int root1, int root2);
    int Find(int x);
};

DisjointSet::DisjointSet(int s)
{
    size = s;
    parent = new int [size];
    for(int i = 0; i < size; i++)  parent[i] = -1;
}

int DisjointSet::Find(int x)
{
    //返回包含给定元素的集合的名字
    //路径压缩，将根节点的下标赋给路径上的各个结点
    if(parent[x] < 0)  return x;//该元素是根节点
    return parent[x] = Find(parent[x]);
}

void DisjointSet::Union(int root1, int root2)
{
    //按规模并
    if(root1 == root2)  return;
    if(parent[root1] > parent[root2]){
        //root1, root2 < 0，root2的规模大
        parent[root2] += parent[root1];
        parent[root1] = root2;
    }
    else{
        //root1的规模大
        parent[root1] += parent[root2];
        parent[root2] = root1;
    }
}

//迷宫生成
#include <ctime>
#include <cstdlib>

void createPuzzle(int size)
{
    //size是迷宫的规模
    int num1, num2;
    DisjointSet ds(size * size);

    srand(time(0));

    while(ds.Find(0) != ds.Find(size * size - 1)){
        //入口与出口不连通
        while(true){
            //选择两个相邻的不连通单元
            num1 = rand() * size * size / (RAND_MAX + 1);
            
            //检查上方的单元
            num2 = num1 - size;
            if(num2 >= 0 && ds.Find(num1) != ds.Find(num2))  break;

            //检查左边的单元
            num2 = num1 - 1;
            if(num1 % size != 0 && ds.Find(num1) != ds.Find(num2))  break;

            //检查下方的单元
            num2 = num1 + size;
            if(num2 < size * size && ds.Find(num1) != ds.Find(num2))  break;

            //检查右边的单元
            num2 = num1 + 1;
            if(num2 % size != 0 && ds.Find(num1) != ds.Find(num2))  break;
        }

        ds.Union(ds.Find(num1), ds.Find(num2));
        cout<<'<'<<num1<<','<<num2<<">,";
    }
}