//二叉树的应用：计算表达式
#include <iostream>

using namespace std;

class calc
{
private:
    enum Type{DATA, ADD, SUB, MULTI, DIV, OPAREN, CPAREN, EOL};

    struct node{
        Type type;
        int data;
        node *lchild, *rchild;
        
        node(Type t, int d = 0, node *l = NULL, node *r = NULL){
            type = t;  data = d;  lchild = l;  rchild = r;
        }
    };

    node *root;

    node *create(char *&s);//从中缀表达式创建一棵表达式树
    Type getToken(char *&s, int &value);//从s中获取一个语法单位
    int result(node *t);//计算以t为根的表达式结果

public:
    calc(char *s){root = create(s);}

    int result(){
        if(root == NULL)  return 0;
        return result(root);
    }
};

typename calc::node *calc::create(char *&s){
    //从中缀表达式创建一棵表达式树
    calc::node *p, *root = NULL;
    Type returnType;
    int value;

    while(*s){
        //扫描中缀表达式中的每个元素
        returnType = getToken(s, value);
        //如果该元素是数字，value在执行该函数后改变，为其数值

        switch (returnType)
        {
            case DATA: case OPAREN:
                if(returnType == DATA) p = new calc::node(DATA, value);
                else  p = create(s);
                if(root != NULL){
                    if(root->rchild == NULL)  root->rchild = p;
                    else root->rchild->rchild = p;
                }
                break;
        
            case CPAREN: case EOL:  return root;

            case ADD: case SUB:
                if(root == NULL)  root = new calc::node(returnType, 0, p);
                //因为'+''-'不可能第一个出现，而是数字第一个出现，它保存在p中
                else  root = new calc::node(returnType, 0, root);
                break;

            case MULTI: case DIV:
                if(root == NULL)  root = new calc::node(returnType, 0, p);
                else if(root->type == MULTI || root->type == DIV)  
                    root = new calc::node(returnType, 0, root);
                else root->rchild = new calc::node(returnType, 0, root->rchild);
        }
    }

    return root;
}

typename calc::Type calc::getToken(char *&s, int &data){

    char type;

    while(*s == ' ')  ++s;//跳过表达式中空格

    if(*s >= '0' && *s <= '9'){
        data = 0;
        while (*s >= '0' && *s <= '9'){
            data = data * 10 + *s - '0';
            ++s;
        }
        return DATA; 
    }

    if(*s == '\0')  return EOL;

    type = *s;   ++s;
    switch (type)
    {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MULTI;
        case '/': return DIV;
        case '(': return OPAREN;
        case '0': return CPAREN;
        default:  return EOL;
    }
}

int calc::result(calc::node *t){
    int num1, num2;
    
    if(t->type == DATA)  return t->data;
    num1 = result(t->lchild);
    num2 = result(t->rchild);
    
    switch(t->type)
    {
        case ADD: t->data = num1 + num2;  break;
        case SUB: t->data = num1 - num2;  break;
        case MULTI: t->data = num1 * num2;  break;
        case DIV: t->data = num1 / num2;  break;
    }

    return t->data;
}

int main(){
    calc exp("2 * 3 + (1 *2*3  + 6*6)*(2+3)/5 + 2/2");
    cout<<exp.result()<<endl;
    return 0;
}