//二叉树的链接实现(二叉链表类)
#include <iostream>
#include "listQueue.cpp"
#include "linkStack.cpp"

using namespace std;

template <class T> class binaryTree;
template <class T> void printTree(const binaryTree<T> &t, T flag);
//在类定义前先对类进行预定义，并在其后声明函数

template <class T>
class binaryTree
{
    friend void printTree<>(const binaryTree<T> &t, T flag);//注意类中定义时要加<>
    //或者友元函数直接在类中实现即可

private:
    struct node
    {
        node *left, *right;
        T data;

        node():left(NULL), right(NULL){}
        node(T item, node *L=NULL, node *R=NULL):data(item), left(L), right(R){}
        ~node(){}
    };

    struct StNode
    {
        //非递归中序、后序遍历时用来存储根节点入栈次数
        node *n;
        int TimesPop;

        StNode(node *nn = NULL):n(nn), TimesPop(0){}
        //TimesPop记为0，表示首次入栈
    };
    

    node *root;

public:
    binaryTree():root(NULL){}
    binaryTree(T x){root = new node(x);}
    ~binaryTree();
    bool isempty() const;
    T Root(T flag) const;//flag是未找到时的返回值
    T lchild(T x, T flag) const;
    T rchild(T x, T flag) const;
    void delLeft(T x);
    void delRight(T x);
    void clear();//包裹函数
    void preOrder() const;//包裹函数
    void midOrder() const;//包裹函数
    void postOrder() const;//包裹函数（递归实现）
    void preOrderfdg() const;//非递归遍历
    void midOrderfdg() const;
    void postOrderfdg() const;
    void levelOrder() const;//层次遍历
    void createTree(T flag);
    T parent(T x, T flag) const {return flag;}

private:
    node *find(T x, node *t) const;//找到存放x的节点
    void clear(node *&t);//指针的引用，t指向的内存会变化
    void preOrder(node *t) const;
    void midOrder(node *t) const;
    void postOrder(node *t) const;//实现函数，树根作为控制递归结束的参数
};

template <class T>
bool binaryTree<T>::isempty() const{
    return root == NULL;
}

template <class T>
T binaryTree<T>::Root(T flag) const{
    if(root == NULL)  return flag;
    else  return root->data;
}

template <class T>
void binaryTree<T>::clear(binaryTree<T>::node *&t){
    if(t == NULL)  return;
    clear(t->left);
    clear(t->right);
    delete t;
    t = NULL;
}

template <class T>
void binaryTree<T>::clear(){
    clear(root);
}

template <class T>
binaryTree<T>::~binaryTree(){
    clear(root);
}

template <class T>
void binaryTree<T>::preOrder(binaryTree<T>::node *t) const{
    if(t == NULL)  return;
    cout<<t->data<<" ";
    preOrder(t->left);
    preOrder(t->right);
    //前序遍历是相对于根节点来说的
}

template <class T>
void binaryTree<T>::preOrder() const{
    cout<<"\n前序遍历：";
    preOrder(root);
}

template <class T>
void binaryTree<T>::midOrder(binaryTree<T>::node *t) const{
    if(t == NULL)  return;
    midOrder(t->left);
    cout<<t->data<<" ";
    midOrder(t->right);
}

template <class T>
void binaryTree<T>::midOrder() const{
    cout<<"\n中序遍历：";
    midOrder(root);
}

template <class T>
void binaryTree<T>::postOrder(binaryTree<T>::node *t) const{
    if(t == NULL)  return;
    postOrder(t->left);
    postOrder(t->right);
    cout<<t->data<<" ";
}

template <class T>
void binaryTree<T>::postOrder() const{
    cout<<"\n后序遍历：";
    postOrder(root);
}

template<class T>
void binaryTree<T>::preOrderfdg() const{
    //非递归遍历
    linkStack<node*> s;
    node *cur;

    cout<<"前序遍历：";
    s.push(root);
    while(!s.isempty()){
        cur = s.pop();
        cout<<cur->data;
        if(cur->right != NULL)  s.push(cur->right);
        if(cur->left != NULL)  s.push(cur->left);
        //这样左子树可以先出栈  
    }
}

template <class T>
void binaryTree<T>::midOrderfdg() const{
    //非递归遍历
    linkStack<StNode> s;
    StNode current(root);

    cout<<"中序遍历：";
    s.push(current);//根节点进栈
    wihle(!s.isempty()){
        current = s.pop();
        if(++current.TimesPop == 2){
            //根节点第二次出栈，可以输出
            cout<<current.node->data;
            if(current.node->right != NULL)  s.push(current.node->right);
        }
        else{
            //根节点第一次出栈，不能输出，重新入栈
            s.push(current);
            if(current.node->left != NULL)  s.push(current.node->left);
        }
    }
}

template <class T>
void binaryTree<T>::postOrderfdg() const{
    //非递归遍历
    linkStack<StNode> s;
    StNode current(root);

    cout<<"后序遍历：";
    s.push(current);
    while(!s.isempty()){
        current = s.pop();
        if(++current.TimesPop == 3){
            //第三次出栈，可以输出
            cout<<current.node->data;
            continue;
        }

        s.push(current);//重新入栈
        if(current.node->TimesPop == 1){
            //第一次出栈
            if(current.node->left != NULL)  s.push(current.node->left);
        }
        else{
            if(current.node->right != NULL)  s.push(current.node->right);
        }
    }
}


template <class T>
void binaryTree<T>::levelOrder() const{
    listQueue<node *> que;
    node *tmp;

    cout<<"\n层次遍历：";
    que.enQueue(root);

    while(!que.isempty()){
        tmp = que.deQueue();
        cout<<tmp->data<<" ";
        if(tmp->left)  que.enQueue(tmp->left);
        if(tmp->right)  que.enQueue(tmp->right);
    }
}

template <class T>
typename binaryTree<T>::node *binaryTree<T>::find(T x, binaryTree<T>::node *t) const{
    //参数*t表示启示寻找的根节点
    if(t == NULL)  return NULL;
    if(t->data == x)  return t;
    node *tmp;
    if(tmp = find(x, t->left))  return tmp;//注意是赋值号！
    else  return find(x, t->right);
}

template <class T>
void binaryTree<T>::delLeft(T x){
    //删除值为x节点的左子树
    node *tmp = find(x, root);//从根节点开始找x
    if(tmp == NULL)  return;
    clear(tmp->left);
}

template <class T>
void binaryTree<T>::delRight(T x){
    //删除值为x节点的右子树
    node *tmp = find(x, root);//从根节点开始找x
    if(tmp == NULL)  return;
    clear(tmp->right);
}

template <class T>
T binaryTree<T>::lchild(T x, T flag) const{
    //返回值为x的节点的左孩子的值，找不到则返回flag
    node *tmp = find(x, root);
    if(tmp == NULL || tmp->left == NULL)  return flag;

    return(tmp->left->data);
}

template <class T>
T binaryTree<T>::rchild(T x, T flag) const{
    //返回值为x的节点的右孩子的值，找不到则返回flag
    node *tmp = find(x, root);
    if(tmp == NULL || tmp->right == NULL)  return flag;

    return(tmp->right->data);
}

template <class T>
void binaryTree<T>::createTree(T flag){
    //flag用来标记空节点
    listQueue<node *> que;
    node *tmp;
    T x, ldata, rdata;

    cout<<"\n输入根节点：";
    cin>>x;
    root = new node(x);
    que.enQueue(root);

    while(!que.isempty()){
        tmp = que.deQueue();
        cout<<"\n输入"<<tmp->data<<"的两个儿子（"<<flag<<"表示空节点）：";
        cin>>ldata>>rdata;
        if(ldata != flag)  que.enQueue(tmp->left = new node(ldata));
        if(rdata != flag)  que.enQueue(tmp->right = new node(rdata));
    }

    cout<<"create completed!\n";
}

template <class T>
void printTree(const binaryTree<T> &t, T flag){
    //以层次遍历的顺序输出每个节点和它的左右孩子
    listQueue<T> que;
    que.enQueue(t.root->data);
    cout<<endl;
    while (!que.isempty())
    {
        T p, l, r;
        p = que.deQueue();
        l = t.lchild(p, flag);
        r = t.rchild(p, flag);
        cout<<p<<" "<<l<<" "<<r<<"\n";
        if(l != flag) que.enQueue(l);
        if(r != flag) que.enQueue(r);
    }
}

