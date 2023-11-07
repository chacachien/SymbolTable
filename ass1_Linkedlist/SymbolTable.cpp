#include "SymbolTable.h"

SymbolTable::SymbolTable() : scbe(0), scen(0),head(NULL), tail(NULL), count(0) {};
SymbolTable::~SymbolTable(){
	this->clear();
}
SymbolTable::Node::Node(bool isNum, int number, string name, int scope) :
isNum(isNum), name(name), number(number), String("not"), scope(scope), next(NULL), prev(NULL){};

SymbolTable::Node::Node(bool isNum, string String, string name, int scope) :
isNum(isNum), name(name), number(0), String(String), scope(scope), next(NULL), prev(NULL){};

bool SymbolTable::insert(bool isNum, int number, string String, string name){
	if (this->head == NULL){
		//Node* newNode = init(isNum, name, number, String);
		Node* newNode;
		if (isNum == 1){
			newNode = new Node(isNum, number, name, this->scbe);
		}
		else{
			newNode = new Node(isNum, String, name, this->scbe);
		}
		
		this->head = newNode;
		this->tail = newNode;
	}
	else{
		Node* tp = this->tail;
		while (tp != NULL){
			if (tp->name == "end"){
				int scp = tp->scope + 1;
				while (true){
					tp = tp->prev;
					if (tp->prev == NULL || (tp->name == "begin" && tp->scope == scp)){
						tp = tp->prev;
						break;
					}
				}
			}
			if (tp != NULL){
				if (tp->name == "begin"&& tp->String == "begin"){
					break;
				}
				if (name == tp->name && tp->name != "end"){
					tp = NULL; //today
					return false;
				}
				if (tp->name != "end"){
					tp = tp->prev;
				}
			}
		}
		//Node* newNode = init(isNum, name, number, String);
		Node* newNode;
		if (isNum == 1){
			newNode = new Node(isNum, number, name, this->scbe);

		}
		else{
			newNode = new Node(isNum, String, name, this->scbe);
		}


		this->tail->next = newNode;
		newNode->prev = this->tail;
		this->tail = newNode;
	}

	this->count++;
	return true;
}
bool SymbolTable::assignS(string name, string String, string instruction){
	Node*p = this->tail;
	while (p != NULL){
		if (p->name == "end"){
			int scp = p->scope + 1;
			while (true){
				p = p->prev;
				if (p==NULL||(p->name == "begin" && p->scope==scp)){
					break;
				}
			}
		}
		if (name == p->name){
			if (p->isNum == 0){
				p->String = String;
				return true;
			}
			else{
				clear();
				throw TypeMismatch(instruction);
			}

		}
		p = p->prev;
	}

	return false;
}
bool SymbolTable::assignO(string name, string oldname, string instruction){
	Node* p = this->tail;
	Node*q = this->tail;
	while (q != NULL){
		if (q->name == "end"){
			int scp = q->scope + 1;
			while (true){
				q = q->prev;
				if (q==NULL|| (q->name == "begin" && q->scope == scp)){
					break;
				}
			}
		}
		if (q->name == oldname){
			break;
		}
		q = q->prev;
	}
	if (q == NULL){
		clear();
		throw Undeclared(instruction);
	}
	while (p != NULL){
		if (p->name == "end"){
			int scp = p->scope + 1;
			while (true){
				p = p->prev;
				if (p==NULL||(p->name == "begin" && p->scope == scp)){
					break;
				}
			}
		}
		if (name == p->name){
			if (p->isNum == q->isNum){
				if (p->isNum == 1){
					p->number = q->number;
				}
				else{
					p->String = q->String;
				}
				return true;
			}
			else{
				clear();
				throw TypeMismatch(instruction);
			}
		}
		p = p->prev;
	}

	return false;
}
bool SymbolTable::assignN(string name, int number, string instruction){
	Node*p = this->tail;
	while (p != NULL){
		if (p->name == "end"){
			int scp = p->scope + 1;
			while (true){
				p = p->prev;
				if (p==NULL||(p->name == "begin" && p->scope == scp)){
					break;
				}
			}
		}
		if (name == p->name){
			if (p->isNum == 1){
				p->number = number;
				return true;
			}
			else{
				clear();
				throw TypeMismatch(instruction);
			}

		}
		p = p->prev;
	}

	return false;
}
int SymbolTable::lookup(string name){
	Node* p = this->tail;
	while (p != NULL){
		if (p->name == "end"){
			int scp = p->scope + 1;
			while (true){
				p = p->prev;
				if (p == NULL || (p->name == "begin" && p->scope == scp)){
					break;
				}
			}
		}
		if (p->name == name){
			return p->scope;
		}
		p = p->prev;
	}
	return -1;
}
bool SymbolTable::contain(Node* a){
	Node*p = this->tail;
	if (p == a){
		return true;
	}
	while (p != a->prev ){
		
			if (p->name == "end"){
				int scp = p->scope + 1;
				while (true){
					p = p->prev;
					if (p == NULL || (p->name == "begin" && p->scope == scp)){
						
							p = p->prev;
							break;
						
					}
				}
			}
			if (p->name != "end"){
				if (p->name == a->name){
					return true;
				}
				if (p->name == "begin"  &&p->scope == a->scope + 1){

					return false;
				}
				p = p->prev;
			}
	}
	return false;
}
string SymbolTable::printS(){
	if (this->head == NULL){
		return "";
	}
	string s = "";
	Node* p = this->head;
	while (p != NULL){
		if (p->name == "begin"){
			Node* q = p;
			while (q != NULL){
				if (q->name == "end" && p->scope -1==q->scope){
					p = q;
				}
				q = q->next;
			}
		}
		if (p->name != "begin" && p->name != "end"){
			
			if ((!contain(p) && this->scbe > p->scope)|| (this->scbe==p->scope && contain(p)) ){

				s = s + p->name;
				s = s + "//";
				s = s + to_string(p->scope) + " ";

			}
		}


		p = p->next;
	}
	return s;
}
string SymbolTable::RprintS(){
	if (this->head == NULL){
		return "";
	}
	string s = "";
	Node* p = this->tail;
	while (p!=NULL){
		if (p->name == "begin" || p->name == "end"){
			break;
		}
		s = s + p->name;
		s = s + "//";
		s = s + to_string(p->scope) + " ";
		
		p = p->prev;
	}
	while (p != NULL){
		
		if (p->name == "end"){
				int scp = p->scope + 1;
				while (true){
					p = p->prev;
					if (p==NULL||(p->name == "begin" && p->scope == scp)){
						break;
					}
				}
		}
		
		
		if (p->name != "begin" && p->name != "end"){
			Node*q = this->tail;
			bool find = false;
			while (q != p){
				if (q->name == "end"){
					int scp = q->scope + 1;
					while (true){
						q = q->prev;
						if (q == NULL || (q->name == "begin" && q->scope == scp)){
							break;
						}
					}
				}
				if (q != NULL){
					if (q->name == p->name){
						find = true;
						break;

					}
					q = q->prev;
				}

			}
			if (!find) {
				s = s + p->name;
				s = s + "//";
				s = s + to_string(p->scope) + " ";

			}

		}
		p = p->prev;;
	}
	return s;
}
//void SymbolTable::printall(){
//	Node* p = this->head;
//	int i = 0;
//	while (p != NULL){
//		cout << i << "." << "name: " << p->name << "  " << "isNum:" << p->isNum << " " << "number: " << p->number << " String: " << p->String << " Scope: " << p->scope;
//		cout << endl;
//		i++;
//		p = p->next;
//	}
//}
void SymbolTable::clear(){
	Node*p =this->head;
	while (p != this->tail){

		p = p->next;
		
		p->prev->next = NULL;
		
		delete p->prev;
		p->prev = NULL;
		
	}
	delete p;
	p = NULL;

	this->head = NULL;
	this->tail = NULL;
	this->count = 0;


}
void SymbolTable::run(string filename){
	fstream file;
	file.open(filename, ios::in);
	while (!file.eof()){
		char tp[255];
		file.getline(tp, 255);
		string instruction;
		for (int i = 0; tp[i] != '\0'; i++){
			instruction += tp[i];
		}
		if (memcmp(tp, "INSERT ", 7) == 0){
			string name;
			string type;
			int i = 7;
			if (instruction[7]<'a' || instruction[7]>'z'){
				clear();
				throw InvalidInstruction(instruction);
			}

			while (instruction[i] != ' '){
				if ((instruction[i] >= 'a'&& instruction[i] <= 'z') || (instruction[i] >= 'A'&&instruction[i] <= 'Z') || (instruction[i] >= '0' && instruction[i] <= '9') || instruction[i] == '_'){
					name += instruction[i];
					i++;
				}
				else{
					clear();
					throw InvalidInstruction(instruction);
				}


			}
			i++;
			while (instruction[i] != '\0'){
				type += instruction[i];
				i++;
			}
			if ((type == "string" || type == "number")){
				bool isNum = true;
				if (type == "string"){
					isNum = false;
				}
				else{
					isNum = true;
				}

				if (insert(isNum, 0, "", name) == true){
					cout << "success" << endl;
				}
				else{
					clear();
					throw Redeclared(instruction);
				}
			}
			else{
				clear();
				throw InvalidInstruction(instruction);
			}


		}
		else if (memcmp(tp, "ASSIGN ", 7) == 0){
			string name = "";
			string String = "";
			int number = 0;
			int i = 7;
			if (instruction[7]<'a' || instruction[7]>'z'){
				clear();
				throw InvalidInstruction(instruction);
			}

			while (instruction[i] != ' '){
				if ((instruction[i] >= 'a'&& instruction[i] <= 'z') || (instruction[i] >= 'A'&&instruction[i] <= 'Z') || (instruction[i] >= '0' && instruction[i] <= '9') || instruction[i] == '_'){
					name += instruction[i];
					i++;
				}
				else{
					clear();
					throw InvalidInstruction(instruction);
				}
			}
			i++;
			if (instruction[i] == '\''){
				i++;
				while (true){
					if ((instruction[i] >= 'a'&& instruction[i] <= 'z') || (instruction[i] >= 'A'&& instruction[i] <= 'Z') || (instruction[i] >= '0' && instruction[i] <= '9') || instruction[i] == ' '){
						String += instruction[i];
						i++;
						if (instruction[i] == '\''){
							break;
						}
					}
					else{
						clear();
						throw InvalidInstruction(instruction);
					}
				}
				if (assignS(name, String, instruction) == 1){
					cout << "success" << endl;
				}
				else{
					clear();
					throw Undeclared(instruction);
				}
			}
			else if (instruction[i] >= '0'&& instruction[i] <= '9'){
				while (instruction[i] != '\0'){
					if ((instruction[i] >= '0' && instruction[i] <= '9')){
						String += instruction[i];
						i++;
					}
					else{
						clear();
						throw InvalidInstruction(instruction);
					}
				}
				number = stoi(String);
				if (assignN(name, number, instruction) == 1){
					cout << "success" << endl;
				}
				else{
					clear();
					throw Undeclared(instruction);
				}
			}
			else if (instruction[i] >= 'a'&&instruction[i] <= 'z'){
				while (instruction[i] != '\0'){
					if ((instruction[i] >= 'a'&& instruction[i] <= 'z') || (instruction[i] >= 'A'&&instruction[i] <= 'Z') || (instruction[i] >= '0' && instruction[i] <= '9') || instruction[i] == '_'){
						String += instruction[i];
						i++;
					}
					else{
						clear();
						throw InvalidInstruction(instruction);
					}
				}
				if (assignO(name, String, instruction) == 1){
					cout << "success" << endl;
				}
				else{
					clear();
					throw Undeclared(instruction);
				}

			}
			else{
				clear();
				throw InvalidInstruction(instruction);
			}

		}
		else if (memcmp(tp, "BEGIN", 5) == 0){
			this->scbe++;
			insert(false, 0, "begin", "begin");
		}
		else if (memcmp(tp, "END", 3) == 0){
			this->scen++;
			this->scbe--;
			if (this->scbe<0){
				clear();
				throw UnknownBlock();
			}
			insert(false, 0, "end", "end");

		}
		else if (memcmp(tp, "LOOKUP ", 7) == 0){
			string name;
			int i = 7;
			if (instruction[7]<'a' || instruction[7]>'z'){
				clear();
				throw InvalidInstruction(instruction);
			}

			while (instruction[i] != '\0'){
				if ((instruction[i] >= 'a'&& instruction[i] <= 'z') || (instruction[i] >= 'A'&&instruction[i] <= 'Z') || (instruction[i] >= '0' && instruction[i] <= '9') || instruction[i] == '_'){
					name += instruction[i];
					i++;
				}
				else{
					clear();
					throw InvalidInstruction(instruction);
				}
			}
			if (lookup(name) == -1){
				clear();
				throw Undeclared(instruction);
			}
			cout << lookup(name) << endl;
		}
		else if (memcmp(tp, "PRINT", 5) == 0){
			string a = printS();
			if (a.length()>4){
				a.erase(a.length()-1, 1);
				cout << a << endl;
			}
			
		}
		else if (memcmp(tp, "RPRINT", 6) == 0){
			string a = RprintS();
			if (a.length()>4){
				a.erase(a.length() - 1, 1);
				cout << a << endl;
			}
			
		}

		else{
			clear();
			throw InvalidInstruction(instruction);
		}
	}
	if (this->scbe != 0){
		clear();
		throw UnclosedBlock(this->scbe);
	}
	//printall();
}

