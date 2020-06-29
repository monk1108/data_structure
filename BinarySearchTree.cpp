//二叉查找树
//平均情况下时间代价为O(logn)，但退化为单链表后时间代价为O(n)
#include <iostream>

using namespace std;

template <class KEY, class OTHER>
struct SET
{
    KEY key;
    OTHER other;
};

//动态查找表的抽象类
template <class KEY, class OTHER>
class dynamicSearchTable{
    public:
        virtual SET<KEY, OTHER> *find(const KEY &X) const = 0;
        virtual void insert(const SET<KEY, OTHER> &x) = 0;
        virtual void remove(const KEY &x) = 0;
        virtual ~dynamicSearchTable(){};
};

//二叉查找树
template <class KEY, class OTHER>
class BinarySearchTree: public dynamicSearchTable<KEY, OTHER>
{
private:
    struct BinaryNode{
        SET<KEY, OTHER> data;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const SET<KEY, OTHER> &thedata, 
            BinaryNode *lt = NULL, BinaryNode *rt = NULL): data(thedata), left(lt), right(rt){}
    };

    BinaryNode *root;

public:
    BinarySearchTree();
    ~BinarySearchTree();
    SET<KEY, OTHER> *find(const KEY &x) const;
    void insert(const SET<KEY, OTHER> &x);
    void remove(const KEY &x);

private:
    SET<KEY, OTHER> *find(const KEY &x, BinaryNode *t) const;
    void insert(const SET<KEY, OTHER> &x, BinaryNode *&t);
    void remove(const KEY &x, BinaryNode *&t);
    void makeEmpty(BinaryNode *t);
};

template <class KEY, class OTHER>
SET<KEY, OTHER>* BinarySearchTree<KEY, OTHER>::find(const KEY &x) const{
    return find(x, root);
}

template <class KEY, class OTHER>
SET<KEY, OTHER>* BinarySearchTree<KEY, OTHER>::find(const KEY &x, BinaryNode *t) const
{
    if(t == NULL || t->data.key == x){
        //找到或不存在
        return (SET<KEY, OTHER> *)t;
    }
    if(x < t->data.key) return find(x, t->left);
    else  return find(x, t->right);
}

template <class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::insert(const SET<KEY, OTHER> &x)
{
    insert(x, root);
}

template <class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::insert(const SET<KEY, OTHER> &x, BinaryNode *&t)
{
    if(t == NULL)  t = new BinaryNode(x, NULL, NULL);
    //此处t被改变，所以要用*&t
    else if(x.key < t->data.key)  insert(x, t->left);
    else if(x.key > t->data.key)  insert(x, t->right);
}

template <class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::remove(const KEY &x)
{
    remove(x, root);
}

template <class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::remove(const KEY &x, BinaryNode *&t)
{
    if(t == NULL) return;
    else if(x < t->data.key)  remove(x, t->left);
    else if(x > t->data.key)  remove(x, t->right);
    else if(t->right != NULL && t->left != NULL)//有两个孩子
    {
        BinaryNode *tmp = t->right;
        while(tmp->left != NULL)  tmp = tmp->left;
        //替身结点是空结点或只有右儿子
        t->data = tmp->data;
        remove(t->data.key, t->left);//删除替身结点
    }
    else{
        //被删结点是叶节点或只有一个儿子
        BinaryNode *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        //此处t被改变，所以要用*&t
        delete oldNode;
    }
}

template <class KEY, class OTHER>
BinarySearchTree<KEY, OTHER>::BinarySearchTree(){
    root = NULL;
}

template <class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::makeEmpty(BinaryNode *t)
{
    if(t == NULL) return;
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
}

template <class KEY, class OTHER>
BinarySearchTree<KEY, OTHER>::~BinarySearchTree(){
    makeEmpty(root);
}

int main(){
    //测试
    SET<int, char *> a[] = {{10, "aaa"}, {8, "bbb"}, {3, "ccc"}, {21, "ddd"}};
    BinarySearchTree<int, char *> tree;
    SET<int, char *> x;
    SET<int, char *> *p;

    for(int i = 0; i < 4; i++)  tree.insert(a[i]);

    p = tree.find(4);
    if(p)  cout<<"\nyes";
    else cout<<"\nno";

    p = tree.find(3);
    if(p)  cout<<"\nyes";
    else cout<<"\nno";
    tree.remove(3);
    p = tree.find(3);
    if(p)  cout<<"\nyes";
    else cout<<"\nno";

    x.key = 30;
    x.other = "fff";
    tree.insert(x);
    p = tree.find(30);
    if(p)  cout<<"\nyes";
    else cout<<"\nno";

    return 0;
}