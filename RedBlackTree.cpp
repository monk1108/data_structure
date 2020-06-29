//红黑树(AVL触发调整的次数太多)
//如果任何一条从根到空结点的路径上有H个黑色结点，整棵树必须至少有2^H-1个黑色结点，至多有2^(2H)-1个结点。
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

template<class KEY, class OTHER>
class RedBlackTree:public dynamicSearchTable<KEY, OTHER>
{
    enum colourT{RED, BLACK};
    struct RedBlackNode
    {
        SET<KEY, OTHER> data;
        RedBlackNode *left;
        RedBlackTree *right;
        colourT colour;

        RedBlackNode(const SET<KEY, OTHER> &element, RedBlackNode *lt=NULL,
            RedBlackNode *rt=NULL, colourT h=RED)
            :data(element), left(lt), right(rt), colour(h){}
    };

    RedBlackNode *root;

public:
    RedBlackTree(RedBlackNode *t=NULL){root=t;}
    ~RedBlackTree(){makeEmpty(root);}
    SET<KEY, OTHER> *find(const KEY &x)const;
    void insert(const SET<KEY, OTHER> &x);
    void remove(const KEY &x);

private:
    void makeEmpty(RedBlackNode *&t);
    void insertAdjust(RedBlackNode *gp, RedBlackNode *p, RedBlackNode *t);
    void removeAdjust(RedBlackNode *&p, RedBlackNode *&c, RedBlackNode *&t, KEY del);
    void LL(RedBlackNode *gp);
    void LR(RedBlackNode *gp);
    void RL(RedBlackNode *gp);
    void RR(RedBlackNode *gp);
};

template <class KEY, class OTHER>
SET<KEY, OTHER> *RedBlackTree<KEY, OTHER>::find(const KEY &x)const
{
    RedBlackNode *t = root;
    while(t != NULL && t->data.key != x){
        if(x < t->data.key) t = t->left;
        else t = t->right;
    }

    if(t == NULL)  return NULL;
    else return (SET<KEY, OTHER> *)t;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::makeEmpty(RedBlackNode *&t)
{
    if(t != NULL){
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

//插入（红色结点）时，1.若父结点为黑色，则完成。如果父结点为红色：
//2.父结点的兄弟也是黑色的，只要一次单旋转或一次双旋转
//3.父结点的兄弟是红色的，重新着色，向上调整。
//为避免情况3，在寻找插入位置时，一旦遇到结点x的两个儿子都是红结点时，把x着色为红色，把两个儿子变为黑色。
//这样能转化为情况1和2。
template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::insert(const SET<KEY, OTHER> &x)
{
    RedBlackNode *t, *parent, *grandP;

    if(root == NULL){
        //在空树上插入
        root = new RedBlackNode(x, NULL, NULL, BLACK);//根结点是黑色的
        return;
    }

    parent = grandP = t = root;
    while(true){
        if(t){
            if(t->left && t->left->colour == RED && t->right && t->right->colour == RED){
                t->left->colour = t->right->colour = BLACK;
                t->colour = RED;
                //此处如果是根结点，并未保证根结点为黑色。
                insertAdjust(grandP, parent, t);
            }

            //下移一层
            grandP = parent;
            parent = t;
            t = (x.key < t->data.key ? t->left : t->right);
        }
        
        else{
            //t是空结点
            t = new RedBlackNode(x);//插入的是红结点,此时第112行失效
            if(x.key < parent->data.key)  parent->left = t;
            else parent->right = t;
            insertAdjust(grandP, parent, t);
            root->colour = BLACK;//?
            return;
        }
    }
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::insertAdjust(RedBlackNode *gp, RedBlackNode *p, RedBlackNode *t)
{
    if(p->colour)  return;//父结点是黑结点
    if(p == root){
        p->colour = BLACK;
        return;
    }
    //t一定是红的
    if(gp->left == p){
        if(p->left == t)  LL(gp);
        else  LR(gp);

    }
    else{
        if(p->left == t)  RL(gp);
        else  RR(gp);
    }
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::LL(RedBlackNode *gp)
{
    RedBlackNode *p = gp->left, *t = p->left;
    RedBlackNode tmp = *gp;
    gp->data = p->data;
    gp->left = t;
    gp->right = p;
    p->data = tmp.data;
    p->left = p->right;
    p->right = tmp.right;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::LR(RedBlackNode *gp)
{
    RedBlackNode *p = gp->left, *t = p->right;
    RedBlackNode tmp = *gp;
    gp->data = t->data;
    gp->right = t;
    p->right = t->left;
    t->data = tmp.data;
    t->left = t->right;
    t->right = tmp.right;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::RR(RedBlackNode *gp)
{
    RedBlackNode *p = gp->right, *t = p->right;
    RedBlackNode tmp = *gp;
    gp->data = p->data;
    gp->left = p;
    gp->right = t;
    p->data = tmp.data;
    p->left = tmp.left;
    p->right = p->left;
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::RL(RedBlackNode *gp)
{
    RedBlackNode *p = gp->right, *t = p->left;
    RedBlackNode tmp = *gp;
    gp->data = t->data;
    gp->left = t;
    p->left = t->right;
    t->data = tmp.data;
    t->left = tmp.left;
    t->right = t->left;
}

//红黑树的删除，见书
template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::remove(const KEY &x)
{
    KEY del = x;
    RedBlackNode *t, *p, *c;

    if(root == NULL)  return;
    if(root->data.key == x && root->left == NULL && root->right = NULL){
        delete root;
        root = NULL;
        return;
    }

    p = c = t = root;
    while(true){
        removeAdjust(p, c, t, del);//将当前结点调整为红结点
        if(c->data.key == del && c->left && c->right){
            //c是被删结点且有两个儿子，在右子树上找替身
            RedBlackNode *tmp = c->right;
            while (tmp->left)  tmp = tmp->left;
            c->data = tmp->data;
            del = tmp->data.key;
            p = c;
            c = c->right;
            t = p->left;
            continue;
        }

        if(c->data.key == del){
            delete c;
            if(p->left == c) p->left = NULL;
            else p->right = NULL;
            root->colour = BLACK;
            return;
        }

        p = c;
        c = (del < p->data.key ? p->left : p->right);
        t = (c == p->left ? p->right : p->left);
    }
}

template <class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::removeAdjust(RedBlackNode *&p, RedBlackNode *&c, 
    RedBlackNode *&t, KEY del)
{
    if(c->colour == RED)  return;
    if(c == root){
        if(c->left && c->right && c->right->colour == c->left->colour){
            c->colour = RED;
            c->left->colour = c->right->colour = BLACK;
            return;
        }
    }

    //1.c有两个黑儿子
    if((c->left && c->left->colour || c->left == NULL) && 
        (c->right && c->right->colour || c->right == NULL)){
            if((t->left && t->left->colour || t->left == NULL) && 
                (t->right && t->right->colour || t->right == NULL)){
                //t有两个黑儿子
                p->colour = BLACK;
                t->colour = c->colour = RED;
            }
            else{
                if(p->left == t){
                    if(t->left && t->left->colour == RED){
                        //t有一个外侧的红儿子
                        t->left->colour = BLACK;
                        LL(p);
                        p = t;
                    }
                    else{
                        //t有一个内侧的红儿子
                        LR(p);
                        p = p->right;
                        p->colour = BLACK;
                    }
                }
                else{
                    if(t->right && t->right->colour == RED){
                        t->right->colour = BLACK;
                        RR(p);
                        p = t;
                    }
                    else{
                        RL(p);
                        p = p->left;
                        p->colour = BLACK;
                    }
                }
                c->colour = RED;//将当前结点改为红结点
            }
        }

        //情况2：c至少有一个红儿子
        else{
            if(c->data.key == del){
                //c是被删结点
                if(c->left && c->right){
                    //在右子树上寻找替身
                    if(c->right->colour == BLACK){
                        LL(c);
                        c = c->right;
                    }
                    return;
                }
                if(c->left){
                    LL(c);
                    p = c;
                    c = c->right;
                }
                if(c->right){
                    RR(c);
                    p = c;
                    c = c->left;
                }
            }

            else{
                //c不是被删结点，向下走一层
                p = c;
                c = (del < p->data.key ? p->left : p->right);
                t = (c == p->left ? p->right : p->left);
                if(c->colour == BLACK){
                    if(t == p->right)  RR(p);
                    else LL(p);
                    p = t;
                    t = (c == p->left ? p->right : p->left);
                    removeAdjust(p, c, t, del);
                }
            }
        }
}
