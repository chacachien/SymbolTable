#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
enum type
{
	str = -1,
	num = 0,
	fun = 1
};
struct parame{
	type ty;
	string na;
};
class SymbolTable
{
public:
	class Node;
public:
	Node* root;
	int beg;
	int end;
	int count;
public:
	
	SymbolTable() : root(NULL),beg(0), end(0),count(0) {};
	~SymbolTable(){
		if(this->root)
		this->clear();
		
	}
	void run(string filename);
	void rotateRight(Node *node);
	void rotateLeft(Node *node);
	void splayRec(Node* node,int &numsplay);
	int compa(Node* a, Node*b);
	bool insert(string name, type ty,int &numcom,int &numsplay,bool sta,int id);
	bool insertFun(string name, type*dstg,int arrsize, int &numcom, int &numsplay,int id);
	bool assign(string name, string String, type ty, int &numcom, int &numsplay,string inst);
	bool assignOld(string name, string nNam, int &numcom, int& numsplay, string inst);
	bool assignFun(string name, string String,parame*arr,int size, int &numcom, int &numsplay, string inst);
	//int search(string name,int &numcom,int &numsplay);
	void delSplay(Node* key);
	//void del(int scope);
	int lookup(string name,int scope,int &numcom,int &numsplay);
	//int findIdMin(Node*& node,int id);
	int findNoRe(Node*& node, int cd);
	void print(Node* node);
	void clearRec(Node*& node);
	void clear();
	
public:
	class Node{
	public:
		string name;
		type ty;
		int number;
		string String;
		type* arr;
		int arrsize;
		int scope;
		int id;
		Node* left;
		Node* right;
		Node* parent;
		
	public:
		Node(string name, type ty, int scope,int id) {
		
			this->name = name;
			this->ty = ty;
			this->scope = scope;
			this->number = 0;
			this->id = id;
			this->String = "";
			this->arr=NULL;
			this->left = NULL;
			this->right = NULL;
			this->parent=NULL;
		}
		Node(string name, type ty, type* dstg,int arrsize,int scope,int id){
			this->name = name;
			this->ty = ty;
			this->arr = new type[arrsize];
			for (int i = 0; i < arrsize; i++){
				arr[i] = dstg[i];
			}
			this->arrsize = arrsize;
			this->scope = scope;
			this->id = id;
			this->left = NULL;
			this->right = NULL;
			this->parent = NULL;
		}
		~Node(){
			if(this->arr){
			delete[] this->arr;
			this->arr=NULL;
			}
				
			
		}
		
		Node* max(){
			Node*tmp = this;
			while (tmp->right != NULL){

				tmp = tmp->right;
			}
			return tmp;
		}
	};
};

#endif
