#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
	class Node;
	int scbe;
	int scen;
private:
	Node* head;
	Node* tail;
	int count;

public:
	SymbolTable();
	~SymbolTable();
	bool insert(bool isNum, int number, string String, string name);
	bool assignS(string name, string String, string instruction);
	bool assignO(string name, string oldname, string instruction);
	bool assignN(string name, int number, string instruction);
	int lookup(string name);
	bool contain(Node* a);
	string printS();
	string RprintS();
	void printall();
	void clear();
	void run(string filename);

public:
	class Node{
	public:
		bool isNum;
		string name;
		int number;
		string String;
		int scope;
		Node* next;
		Node* prev;
	public:
		Node(bool isNum, int number, string name, int scope);
			
		Node(bool isNum, string String, string name, int scope);
			

	};
};
#endif