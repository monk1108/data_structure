//图（假设都是加权图）
#include <iostream>
using namespace std;

template <class TypeofVer, class TypeofEdge>
class graph
{
protected:
    int Vers, Edges;//能被子类的函数访问

public:
    virtual void insert(TypeofVer x, TypeofVer y, TypeofEdge w) = 0;//两个结点间添加一条边，w是权值
    virtual void remove(TypeofVer x, TypeofVer y) = 0;//删除一条边
    virtual bool exist(TypeofVer x, TypeofVer y) = 0;//判断两结点间是否有边存在
    int numOfVer()const {return Vers;}//结点总数
    int numOfEdge()const {return Edges;}//边总数
};


//基于邻接矩阵的图类定义
template <class TypeofVer, class TypeofEdge>
class adjMatrixGraph:public graph<TypeofVer, TypeofEdge>
{
public:
    void insert(TypeofVer x, TypeofVer y, TypeofEdge w) = 0;//两个结点间添加一条边，w是权值
    void remove(TypeofVer x, TypeofVer y) = 0;//删除一条边
    bool exist(TypeofVer x, TypeofVer y) = 0;//判断两结点间是否有边存在
    adjMatrixGraph(int vSize, const TypeOfVer d[], const TypeofEdge noEdgeFlag);
    ~adjMatrixGraph();

private:
    TypeofEdge **edge;//存放邻接矩阵
    TypeofVer *ver;//存放结点值
    TypeofEdge noEdge;//邻接矩阵中∞的表示值
    int find(TypeofVer v)const{
        for(int i = 0; i < Vers; i++){
            if(ver[i] == v)  return i;
        }
    }
};

template <class TypeofVer, class TypeofEdge>
adjMatrixGraph<class TypeofVer, class TypeofEdge>::adjMatrixGraph(
    int vsize, const TypeofVer d[], const TypeofEdge noEdgeFlag
)
{
    int i, j;

    Vers = vsize;//结点数
    Edges = 0;//边数
    noEdge = noEdgeFlag;//∞的表示值

    ver = new TypeOfVer [vsize];
    for(i = 0; i < vsize; i++)  ver[i] = d[i];

    edge = new TypeOfEdge *[vSize];
    for(i = 0; i < vSize; i++){
        edge[i] = new TypeOfEdge [vSize];
        for(j = 0; j < vSize; j++)  edge[i][j] = noEdge;
        edge[i][i] = 0;
    }
}

template <class TypeofVer, class TypeofEdge>
adjMatrixGraph<TypeOfVer, TypeOfEdge>::~adjMatrixGraph()
{
    delete [] ver;
    for(int i = 0; i < Vers; i++)  delete [] edge[i];
    delete []edge;
}

template <class TypeofVer, class TypeofEdge>
void adjMatrixGraph<TypeOfVer, TypeOfEdge>::insert(TypeOfVer x, TypeOfVer y, TypeOfEdge w)
{
    int u = find(x), v = find(y);
    edge[u][v] = w;
    ++Edges;
}

template <class TypeofVer, class TypeofEdge>
void adjMatrixGraph<TypeOfVer, TypeOfEdge>::remove(TypeOfVer x, TypeOfVer y)
{
    int u = find(x), v = find(y);
    edge[u][v] = noEdge;
    --Edges;
}

template <class TypeofVer, class TypeofEdge>
bool adjMatrixGraph<TypeOfVer, TypeOfEdge>::exist(TypeOfVer x, TypeOfVer y)
{
    int u = find(x), v = find(y);
    if(edge[u][v] == noEdge)  return false;
    else return true;
}