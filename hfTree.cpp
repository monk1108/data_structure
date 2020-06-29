#include <iostream>

using namespace std;

template <class T>
class hfTree
{
private:
    struct Node
    {
        T data;
        int weight;
        int parent, left, right;
    };

    Node *elem;
    int length;

public:
    struct hfCode
    {
        //保存哈夫曼编码的类型
        T data;//待编码的字符
        string code;//对应的哈夫曼编码
    };

    hfTree(const T *s, const int *w, int size);
    void getCode(hfCode result[]);
    ~hfTree(){delete []elem;}
};

template <class T>
hfTree<T>::hfTree(const T *s, const int *w, int size){
    const int MAX = 32767;
    int min1, min2;//最小、次最小节点的权值
    int y, x;//最小、次最小节点的下标

    length = 2 * size;
    elem = new Node [length];

    //置初值
    for(int i = size; i < length; ++i){
        elem[i].weight = w[i - size];
        elem[i].data = s[i - size];
        elem[i].parent = elem[i].left = elem[i].right = 0;
    }

    //归并森林中的树
    for(int i = size - 1; i > 0; i--){
        min1 = min2 = MAX;
        y = x = 0;

        for(int j = i + 1; j < length; j++){
            if(elem[j].parent == 0){
                if(elem[j].weight < min1){
                    min2 = min1;
                    min1 = elem[j].weight;
                    x = y;
                    y = j;
                }
                else if(elem[j].weight < min2){
                    min2 = elem[j].weight;
                    x = j;
                }
            }
        }

        elem[i].weight = min1 + min2;
        elem[i].left = x;
        elem[i].right = y;
        elem[i].parent = 0;
        elem[x].parent = i;
        elem[y].parent = i;
    }
}

template <class T>
void hfTree<T>::getCode(hfTree<T>::hfCode result[]){
    //获得每一个节点的哈夫曼编码，由'0'和'1'组成
    int size = length / 2;
    int p, s;//s是正在处理的节点的下标，p是其父节点的下标

    for(int i = size; i < length; i++){
        result[i - size].data = elem[i].data;
        result[i - size].code = "";
        p = elem[i].parent;
        s = i;
    
        while(p){
            //向根追溯
            if(elem[p].left == s)   result[i - size].code = result[i - size].code + '0';
            else  result[i - size].code = result[i - size].code + '1';
            s = p;
            
            p = elem[p].parent;   
            //cout<<"p == "<<p<<'\n';     
        }
    }
}

int main(){
    char ch[] = {"aeistdn"};
    int w[] = {10, 15, 12, 3, 4, 13, 1};

    hfTree<char> tree(ch, w, 7);
    hfTree<char>::hfCode result[7];


    tree.getCode(result);
    for(int i = 0; i < 7; i++){
        cout<<result[i].data<<" "<<result[i].code<<'\n';
    }

    return 0;
}