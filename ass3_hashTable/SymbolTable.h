#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
int linearProbing(unsigned long long key, int i, int m, int c, int c0);
int quadraticProbing(unsigned long long key, int i, int m, int c1, int c2);
int doubleHashing(unsigned long long key, int i, int m, int c, int c0);
unsigned long long mahoa(int scope, string name);
enum STATUS_TYPE{NIL, NON_EMPTY, DELETED};
enum type{strtype,numtype,oldtype,funtype,unknow};

struct Node {
public:
    string name = "";
    int scope = 0;
    unsigned long long key = 0;
    type Nodetype = unknow;
    type* arr = nullptr;
    int arrsize = 0;
    type funreturn = unknow;
    /*Node(string name, int scope, int key, type Nodetype) {
        this->name = name;
        this->scope = scope;
        this->key = key;
        this->Nodetype = Nodetype;
        this->arr = NULL;
        this->arrsize = 0;
    }*/
    /*Node(string name, int scope, int key, type Nodetype, int arrsize) {
        this->name = name;
        this->scope = scope;
        this->key = key;
        this->Nodetype = funtype;
        this->arr = new type[arrsize];
        for (int i = 0; i < arrsize; i++) {
            this->arr[i] = unknow;
        }
        this->arrsize = arrsize;
    }*/
};
class OpenHash {
public:
    int(*hp)(unsigned long long, int, int, int, int) = NULL;
    STATUS_TYPE* status = NULL;
    Node* data = NULL;
    int size = 0;
    int c = 0;
    int c0 = 0;
    int begin = 0;
    int end = 0;
public:
   // OpenHash() {};
    ~OpenHash();
    OpenHash(int size,int c,int c0, int(*hp)(unsigned long long, int, int, int, int)) {
        this->hp = hp;
        this->size = size;
        this->c = c;
        this->c0 = c0;
        this->data = new Node[size];
        this->status = new STATUS_TYPE[size];
        for (int i = 0; i < size; i++) {
            this->status[i] = NIL;
        }
        /*cout << "tao thanh cong hash" << " ";
        cout << "size: " << this->size << " ";
        cout << "c: " << this->c << " ";
        cout << "c0: " << this->c0 << endl;*/
    }
    int insert(string name, type nametype,int scope, int numofpara);
    int assignInt(string name,type nametype);
    int assignFun(string name,  string funame, type* para, int parasize,string namepara[],string& nameUndeclared);
    int assignOld(string name, string oldname);
    int callFun(string name, type* para, int sizepara, string namepara[],string& nameUndeclared);
    int lookup(string name,int& slot);
    void clear();
    void print();
};
class SymbolTable
{
public:
    OpenHash* root = NULL;
public:
    SymbolTable() {};
    ~SymbolTable() {
        delete root;
        root = nullptr;
    };
    void run(string filename);
};

#endif
