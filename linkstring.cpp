//单链表实现stringlei
#include <iostream>
#include <cmath>

using namespace std;

class linkString
{
    friend linkString operator+(const linkString &a, const linkString *b);
    friend bool operator==(const linkString &a, const linkString &b);
    friend bool operator!=(const linkString &a, const linkString &b);
    friend bool operator>=(const linkString &a, const linkString &b);
    friend bool operator<=(const linkString &a, const linkString &b);
    friend bool operator>(const linkString &a, const linkString &b);
    friend bool operator<(const linkString &a, const linkString &b);
    friend ostream &operator<<(ostream &os, const linkString &s);

private:
    struct node{
        int size;//节点中现有的字符数
        char *data;//单个节点中存储的字符
        node *next;

        node(int s = 1, node *n = NULL){size = s; size = 0; next = n;}
    };

    node *head;
    int len;//字符串总长度(包括'\0')
    int nodeSize;//节点的最大容量

    void clear();
    void findPos(int start, int &pos, node *&p);
    void split(node *p, int pos);
    int merge(node *p);

public:
    linkString(const char *s = "");
    linkString(const linkString &s);//拷贝构造函数
    ~linkString();
    int length() const;
    linkString &operator=(const linkString &other);
    linkString subStr(int start, int num) const;//在start位置取长度为num的子串
    linkString insert(int start, const linkString &s);//在start位置插入子串（也是linkString的形式）
    void remove(int start, int num);//从start位置移走num个字符
};

linkString::linkString(const char *s){
    node *p;

    for(len = 0; s[len] != '\0'; ++len);//计算字符串的总长度
    nodeSize = sqrt(len);//已知以sqrt(len)为单个节点容量时效率最高

    p = head = new node(1);//头结点(不存放数据)

    while(*s){
        p = p->next = new node(nodeSize);

        for(; p->size < nodeSize && *s; ++s, p->size++){
            p->data[p->size] = *s;
        }
    }
}

linkString::linkString(const linkString &right){
    node *p, *otherp = right.head->next;

    p = head = new node(1);//头结点
    len = right.len;
    nodeSize = right.nodeSize;

    while (otherp)
    {
        p = p->next = nodeSize;
        for(; p->size < otherp->size; ++p->size){
            p->data[p->size] = otherp->data[p->size];
        }// 0 ~ size-1
        otherp = otherp->next;
    }
}

void linkString::clear(){
    node *p = head->next;
    node *q;

    while(p != NULL){
        q = p->next;
        delete p;
        p = q;
    }
}

linkString::~linkString(){
    clear();
    delete head;
}

int linkString::length(){
    return len;
}

linkString &linkString::operator=(const linkString &right){
    node *p = head, *otherp = right.head->next;

    if(this == &right)  return this;
    clear();
    len = right.len;
    nodeSize = right.nodeSize;

    while(otherp){
        p = p->next = new node(nodeSize);
        for(; p->size < otherp->size; ++p->size){
            p->data[p->size] = otherp->data[p->size];
        }
        otherp = otherp->next;
    }

    return *this;
}

void linkString::findPos(int start, int &pos, node *&p){
    int count = 0;
    p = head->next;

    while (count < start)
    {
        if(count + p->size < start){
            count += p->size;
            p = p->next;
        }
        else{
            pos = start - count;
            return;
        }
    }
}//函数完成后，p指向第start个字符所在的节点，第start个字符在该节点中处于第pos个位置

//取子串
linkString linkString::subStr(int start, int num) const{
    linkString tmp;//存放取出的子串
    int pos;
    node *p, *tp = tmp.head;

    if(start < 0 || start >= len - 1)  return tmp;//返回空串，len包含'\0'，所以用len - 1

    num = (start + num > len)? len - start: num;//字符串的实际长度
    tmp.len = num;
    tmp.nodeSize = sqrt(num);

    findPos(start, pos, p);
    
    //取子串
    for(int i = 0; i < tmp.len; ){
        tp = tp->next = new node(tmp.nodeSize);

        for(; tp->size < tmp.nodeSize && i < tmp.len; ++tp->size, ++i){
            if(pos == p->size){
                p = p->next;
                pos = 0;
            }
            tp->data[tp->size] = p->data[pos++];
        }
    }

    return tmp;
}

void linkString::split(node *p, int pos){
    node *q = new node(nodeSize, p->next);
    p->next = q;
    for(int i = pos; i < p->size; i++){
        q->data[i-pos] = p->data[i];
    }

    q->size = p->size - pos;
    p->size = pos;
}

void linkString::merge(node *p){
    node *q = p->next;

    if(p->size + q->size <= nodeSize){
        for(int i = 0; i < q->size; i++, p->size++){
            p->data[p->size] = q->data[i];
        }

        p->next = q->next;
        delete q;
    }
}

void linkString::insert(int start, const linkString &s){
    if(start < 0 && start > len)  return;

    node *p, *nextp, *tmp;
    int pos;

    findPos(start, pos, p);
    split(p, pos);
    nextp = p->next;
    tmp = s.head->next;

    while(tmp){//注意s和p的nodeSize不同
        for(pos = 0; pos < tmp.size; pos++){
            if(p->size == nodeSize)  p = p->next = new node(nodeSize);

            p->data[p->size] = tmp->data[pos];//从节点p开始插入
            p->size++;            
        }
        tmp = tmp->next;
    }
    
    len += s.len;
    p->next = nextp;//nextp中是p节点split后剩下的字符串
    merge(p);
}

void linkString::remove(int start, int num){
    if(start < 0 || start >= len - 1)  return;

    node *startp;
    int pos;

    findPos(start, pos, startp);
    split(startp, pos);

    //计算实际被删长度
    if(start + num > len){
        len = start;
        num = len - start;
    }
    else  len -= num;

    while(true){
        node *nextp = startp->next;

        if(num > nextp->size){//删除整个节点
            num -= nextp->size;
            startp->next = nextp->next;
            delete nextp;
        }

        else{
            split(nextp, num);
            startp->next = nextp->next;
            delete nextp;
            break;
        }
    }

    merge(startp);
}

linkString operator+(const linkString &s1, const linkString &s2){
    char *tmp = new char[s1.len + s2.len + 1];//先用字符串存放
    linkString::node *p;
    int count = 0;

    for(p = s1.head->next; p != NULL; p = p->next)
        for(int i = 0; i < p->size; i++) 
            tmp[count++] = p->data[i];//复制s1

    for(p = s2.head->next; p != NULL; p = p->next)
        for(int i = 0; i < p-size; i++)
            tmp[count++] = p->data[i];

    tmp[count] = '\0';

    linkString result(tmp);//构造函数，返回linkString类对象
    delete tmp;
    return result;
}

bool operator==(const linkString &s1, const linkString &s2){
    if(s1.len != s2.len)  return false;

    linkString::node *p1 = s1.head->next, *p2 = s2.head->next;
    int pos1 = 0, pos2 = 0;

    while(p1 && p2){
        if(p1->data[pos1] != p2->data[pos2])  return false;

        if(++pos1 == p1->size){
            p1 = p1->next;
            pos1 = 0;
        }
        if(++pos2 == p2->size){
            p2 = p2->next;
            pos2 = 0;
        }
    }
    return true;
}

bool operator!=(const linkString &s1, const linkString &s2){
    return !(s1 == s2);
}

bool operator>(const linkString &s1, const linkString &s2){
    linkString::node *p1 = s1.head->next, *p2 = s2.head->next;
    int pos1 = 0, pos2 = 0;

    while(p1){//p1未结束
        if(p2 == NULL)  return true;//p2结束
        if(p1->data[pos1] > p2->data[pos2])  return true;
        if(p1->data[pos1] < p2->data[pos2])  return false;

        if(++pos1 == p1.size){
            p1 = p1->next;
            pos1 = 0;
        }
        if(++pos2 = p2.size){
            p2 = p2->next;
            pos2 = 0;
        }

        return false;
    }
}

bool operator>=(const linkString &s1, const linkString &s2){
    return (s1 == s2 || s1 > s2);
}

bool operator<(const linkString &s1, const linkString &s2){
    return !(s1 > s2);
}

bool operator<=(const linkString &s1, const linkString &s2){
    return (s1 == s2 || s1 < s2);
}

ostream operator<<(ostream &os, const linkString &s){
    linkString::node *p = s.head->next;
    int pos;

    while(p){
        for(pos = 0; pos < p->size; pos++){
            os<<p->data[pos];
        }
        p = p->next;
    }

    return os;
}