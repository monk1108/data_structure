//AA树，在红黑树基础上增加条件：左儿子不能为红色。
//结点的层次：结点到空结点的路径上左链的数量。如果结点是叶子结点，层次是1，空结点的层次是0。
//水平链：连接相同层次的父子节点，只能是右儿子链。
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
        virtual SET<KEY, OTHER> *find(const KEY &x) const = 0;
        virtual void insert(const SET<KEY, OTHER> &x) = 0;
        virtual void remove(const KEY &x) = 0;
        virtual ~dynamicSearchTable(){};
};

template <class KEY, class OTHER>
class AATree<KEY, OTHER>:public dynamicSearchTable<KEY, OTHER>
{
    struct AANode
    {
        SET<KEY, OTHER> data;
        AANode *left;
        AANode *right;
        int level;

        AANode(const SET<KEY, OTHER> &thedata, AANode *lt, AANode *rt, int lv=1):
            data(thedata), left(lt), right(rt), level(lv){}     
    };

    AANode *root;

public:
    AATree(){root = NULL;}
    ~AATree(){makeEmpty(root);}
    SET<KEY, OTHER> *find(const KEY &x) const;
    void insert(const SET<KEY, OTHER> &x){insert(x, root);}
    void remove(const KEY &x){remove(x, root);}

private:
    void insert(const SET<KEY, OTHER> &x, AANode *&t);
    void remove(const KEY &x, AANode *&t);
    void makeEmpty(AANode *t);
    void LL(AANode *&t);
    void RR(AANode *&t);
    int min(int a, int b){return a < b ? a : b;}
};

template <class KEY, class OTHER>
SET<KEY, OTHER> *AATree<KEY, OTHER>::find(const KEY &x) const
{
    AANode *t = root;
    while(t != NULL && t->data.key != x){
        if(t->data.key < x)  t = t->left;
        else t = t->right; 
    }
    if(t == NULL)  return NULL;
    else return (SET<KEY, OTHER> *)t;
}

template <class KEY, class OTHER>
void AATree<KEY, OTHER>::insert(const KEY<SET, OTHER> &x, AANode *&t)
{
    if(t == NULL) t = new AANode(x, NULL, NULL);
    else{
        if(x.key < t->data.key) insert(x, t->left);
        else if(x.key > t->data.key) insert(x, t->right);
        else return;
    }

    LL(t);
    RR(t);
}

template <class KEY, class OTHER>
void AATree<KEY, OTHER>::LL(AANode *&t)
{
    if(t->left != NULL && t->left->level == t->level){
        AANode *t1 = t->left;
        t->left = t1->right;
        t1->right = t;
        t = t1;
    }
}

template <class KEY, class OTHER>
void AATree<KEY, OTHER>::RR(AANode *&t)
{
    if(t->right != NULL && t->right->right != NULL && t->right->right->level == t->level)
    {
        AANode *t1 = t->right;
        t->right = t1->left;
        t1->left = t;
        t = t1;
        ++t->level;
    }
}

template <class KEY, class OTHER>
void AATree<KEY, OTHER>::remove(const KEY &x, AANode *&t)
{
    if(t == NULL)  return;
    if(x < t->data.key) remove(x, t->left);
    else if(x > t->data.key)  remove(x, t->right);
    else{
        //x是被删结点
        if(t->left != NULL && t->right != NULL){
            AANode *tmp = t->right;
            while(tmp->left != NULL)  tmp = tmp->left;
            t->data = tmp->data;
            remove(t->data.key, t->right);
        }
        else{
            AANode *oldNode = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete oldNode;
            return;
        }
    }

    if(t->left == NULL && t->right == NULL) t->level = 1;
    else t->level = min(t->left->level, t->right->level) + 1;
    if(t->right != NULL && t->right->level > t->level)  t->right->level = t->level;
    LL(t);
    if(t->right != NULL)  LL(t->right);
    if(t->right != NULL && t->right->right != NULL)  LL(t->right->right);
    RR(t);
    if(t->right != NULL)  RR(t->right);




}