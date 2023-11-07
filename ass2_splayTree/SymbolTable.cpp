#include "SymbolTable.h"
string out;
void SymbolTable::run(string filename)
{
	fstream file;
	int id = 0;
	file.open(filename, ios::in);
	while (!file.eof()){
		char tp[255];
		file.getline(tp, 255);
		string inst;
		for (int i = 0; tp[i] != '\0'; i++){
			inst += tp[i];
		}
		
	// INSERT
		if (memcmp(tp, "INSERT ", 7) == 0){
			string name;
			string typeString;
			type ty;
			bool sta;
			//int scope;
			int arsi = 0;
			type arr[255];
			int i = 7;
			
			if (inst[7]<'a' || inst[7]>'z'){
				this->clear();
				throw InvalidInstruction(inst);
			}
			while (inst[i] != ' '){
				if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&&inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == '_'){
					name += inst[i];
					i++;
				}
				else{
					this->clear();
					throw InvalidInstruction(inst);
				}
			}
			i++;
			while (inst[i] != ' ' && inst[i]!='\0'){
				typeString += inst[i];
				i++;
			}
			if ((typeString == "string" || typeString == "number")){
				if (typeString == "string"){
					ty = str;
				}
				else{
					ty = num;
				}
			}
			else if (typeString[0] == '('){
				int i = 0;
				int j = 0;
				type para;
				while (typeString[i] != ')'){

					string tmp = "";
					i++;
					while (typeString[i] != ',' && typeString[i] != ')'){
						tmp += typeString[i];
						i++;
					}
					if (tmp == "number"){
						para = num;
					}
					else if (tmp == "string"){
						para = str;
					}
					else if (tmp == ""){
						break;
					}
					else{
						this->clear();
						throw InvalidInstruction(inst);
					}
					arr[j] = para;
					j++;
				}
				i++;
				if (typeString[i] == '-' && typeString[i + 1] == '>'){
					string tmp = "";
					while (typeString[i + 2] != '\0'){
						tmp += typeString[i + 2];
						i++;
					}
					if (tmp == "number"){
						para = num;
					}
					else if (tmp == "string"){
						para = str;
					}
					else{
						this->clear();
						throw InvalidInstruction(inst);
					}
					arr[j] = para;
					arsi = j + 1;

				}
				else{
					this->clear();
					throw InvalidInstruction(inst);
				}
				ty = fun;

			}
			else{
				this->clear();
				throw InvalidInstruction(inst);
			}
			if (inst[i]!='\0'){
				if (inst.compare(i, 5, " true") == 0){
					sta = true;
					//scopre =0
				}
				else if (inst.compare(i, 6, " false") == 0){
					sta = false;
				}
				else{
					this->clear();
					throw InvalidInstruction(inst);
				}
			}
			else{
				this->clear();
				throw InvalidInstruction(inst);
			}
			if (ty != fun){
				int numcom = 0;
				int numsplay = 0;
				if (insert(name, ty, numcom, numsplay, sta,id)){
					cout << numcom << " " << numsplay << endl;
					id++;
					count++;
				}
				else{
					this->clear();
					throw  Redeclared(inst);
				}
			}
			else{
				if (sta || this->beg == 0){
					int numcom = 0;
					int numsplay = 0;
					if (insertFun(name, arr, arsi, numcom, numsplay,id)){
						cout << numcom << " " << numsplay << endl;
						id++;
						count++;
					}
					else{
						this->clear();
						throw  Redeclared(inst);
					}
				}
				else if (!sta && this->beg){
					this->clear();
					throw InvalidDeclaration(inst);
				}
				else {
					this->clear();
					throw  Redeclared(inst);
				}
			}
		}
// ASSIGN
		else if (memcmp(tp, "ASSIGN ", 7) == 0){
			int numcom = 0;
			int numsplay = 0;
				string name = "";
				string String = "";
				bool isFun = false;
				parame arr[255];
				int arsi = 0;
				int i = 7;
				if (inst[7]<'a' || inst[7]>'z'){
					this->clear();
					throw InvalidInstruction(inst);
					
				}
				while (inst[i] != ' '){
					if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&&inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == '_'){
						name += inst[i];
						i++;
					}
					else{
						this->clear();
						throw InvalidInstruction(inst);
						
					}
				}
				i++;
				if (inst[i] == '\''){
					i++;
					while (true){
						if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&& inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == ' '){
							String += inst[i];
							i++;
							if (inst[i] == '\''){
								break;
							}
						}
						else{
							this->clear();
							throw InvalidInstruction(inst);
						}
					}
					if (assign(name, String, str, numcom, numsplay, inst))
					{
						cout << numcom << " " << numsplay << endl;
					}
					else{
						this->clear();
						throw Undeclared(inst);
					}
				}
				else if (inst[i] >= '0'&& inst[i] <= '9'){
					while (inst[i] != '\0'){
						if ((inst[i] >= '0' && inst[i] <= '9')){
							String += inst[i];
							i++;
						}
						else{
							this->clear();
							throw InvalidInstruction(inst);
						}
					}
					if (assign(name, String, num, numcom, numsplay, inst))
					{
						cout << numcom << " " << numsplay << endl;
					}
					else{
						this->clear();
						throw Undeclared(inst);
					}

				}
				else if (inst[i] >= 'a'&& inst[i] <= 'z'){

					while (inst[i] != '\0' && inst[i] != ')'){
						if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&&inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == '_'){
							String += inst[i];
							i++;
						}
						else if (inst[i] == '('){
							
							isFun = true;
							int j = 0;
								i++;
							while (inst[i] != ')'){

								if (inst[i] >= '0'&& inst[i] <= '9'){
									while (inst[i] != ',' && inst[i] != ')'){
										if ((inst[i] < '0' && inst[i] > '9')){
											this->clear();
											throw InvalidInstruction(inst);
										}
										i++;

									}
									arr[j].ty = num;
									arr[j].na = "noname";
									j++;
									if (inst[i] == ','){
										i++;
									}
									else if (inst[i] == ')'){
										break;
									}
								}
								else if (inst[i] == '\''){   ///hmmmmm
									i++;
									while (inst[i] != '\''){
										if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&& inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == ' '){

											i++;

										}
										else{
											this->clear();
											throw InvalidInstruction(inst);
										}

									}
									i++;
									
									arr[j].ty = str;
									arr[j].na = "noname";
									j++;
									if (inst[i] == ','){
										i++;
									}
									else if (inst[i] == ')'){
										break;
									}
									
								}
								else if (inst[i] >= 'a'&& inst[i] <= 'z'){
									string tmp;
									while (inst[i] != ')'&& inst[i] != ','){
										if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&&inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == '_'){
											tmp += inst[i];
											i++;
										}
										else{
											this->clear();
											throw InvalidInstruction(inst);
										}

									}
									arr[j].na = tmp;
									j++;
									if (inst[i] == ','){
										i++;
									}
									else if (inst[i] == ')'){
										break;
									}
								}
								else{
									this->clear();
									throw InvalidInstruction(inst);
								}
							}
							arsi = j;
						}
						else{
							this->clear();
							throw InvalidInstruction(inst);
						}
					}
					if (isFun){
						if (assignFun(name, String, arr,arsi, numcom, numsplay, inst)){
							cout << numcom << " " << numsplay << endl;
						}
						else{
							this->clear();
							throw Undeclared(inst);
						}
					}
					else{
						if (assignOld(name, String, numcom, numsplay, inst)){
							cout << numcom << " " << numsplay << endl;
						}
						else{
							this->clear();
							throw Undeclared(inst);                                                
						}
					}

				}
				else{
					this->clear();
					throw InvalidInstruction(inst);
				}



				/*cout << "name:" << name << endl;
				cout << "String:" << String << endl;
				cout << "isF:" << isFun << endl;
				cout << "s:" << arsi << endl;
				for (int i = 0; i < arsi; i++){
					cout << arr[i].ty << " " << arr[i].na << endl;
				}*/
			}
			else if (memcmp(tp, "BEGIN",5)==0){
				this->beg++;
			}
			else if (memcmp(tp, "END", 3) == 0){
				this->end++;
				this->beg--;
				if (this->beg < 0){
					this->clear();
					throw UnknownBlock();
				}
				//del(this->beg+1);
				
				int cd = 0;
				if (this->root != NULL){
					Node* p = this->root;
					while (findNoRe(p, cd)<=this->count){
						//cout << findNoRe(p, cd) << " is min";
						if (p == NULL){
							break;
						}
						if (p->scope == this->beg + 1){
							delSplay(p);
						}

						
						//findIdMin(p, cd);
						if (cd == this->count){
							break;
						}
							
						p = this->root;
						cd++;
					}
				}
			}
			else if (memcmp(tp, "LOOKUP ", 7)==0){
				string name;
				int numcom = 0;
				int numsplay = 0;
				int i = 7;
				
				if (inst[7]<'a' || inst[7]>'z'){
					//clear();
					this->clear();
					throw InvalidInstruction(inst);
				}

				while (inst[i] != '\0'){
					if ((inst[i] >= 'a'&& inst[i] <= 'z') || (inst[i] >= 'A'&&inst[i] <= 'Z') || (inst[i] >= '0' && inst[i] <= '9') || inst[i] == '_'){
						name += inst[i];
						i++;
					}
					else{
						this->clear();
						throw InvalidInstruction(inst);
					}
				}
				int sco = this->beg;
				bool find = false;
				for (int i = sco; i >= 0; i--){
					int o = lookup(name, i, numcom, numsplay);
					if ( o!= -1){
						cout << o << endl;
						find = true;
						break;
					}
				}
				if (!find){
					this->clear();
					throw Undeclared(inst);
				}
			}
			else if (memcmp(tp, "PRINT", 5) == 0){
				print(this->root);
				if (out.length() > 4){
					out.erase(out.length() - 1, 1);
					cout << out << endl;
					out = "";
				}
			}
		}
		if (this->beg != 0){
			this->clear();
			throw UnclosedBlock(this->beg);
		}
}
void SymbolTable::print(Node* node){
	if (!node){
		return;
	}
	out+= node->name + "//" +to_string(node->scope)+" " ;
	
	print(node->left);
	print(node->right);
}
//int SymbolTable::findIdMin(Node*& node/*need???*/,int cd){
//	if (!node){
//		return INT_MAX;
//	}
//	int min = this->count;
//	Node* tmpl = node->left;
//	Node* tmpr = node->right;
//	int l = findIdMin( tmpl,cd);
//	int r = findIdMin( tmpr,cd);
//		if (l <= min && l>=cd ){
//			node = tmpl;
//			min = l;
//		}
//		if (r <= min && r>=cd){
//			node = tmpr;
//			min = r;
//		}
//		return min;
//		
//}
int SymbolTable::findNoRe(Node*& node,int cd){
	if (!node){
		return -1;
	}
	int min = this->count;
	Node* cur = node;
	Node*next;
	while (cur){
		if (!cur->left){
			if (min > cur->id && cur->scope == this->beg + 1 && cur->id>=cd){
				min = cur->id;
				node = cur;
			}
				cur = cur->right;
		}
		else{
			next = cur->left;
			while (next->right && next->right != cur){
				next = next->right;
			}
			if (!next->right){
				next->right = cur;
				cur = cur->left;
			}
			else{
				next->right = NULL;
				if (min > cur->id && cur->scope == this->beg + 1 && cur->id >= cd){
					min = cur->id;
					node = cur;
				}
				cur = cur->right;
			}
		}
	}
	return min;
}
//void SymbolTable::del(int scope){
//	Node* tmp = this->root;
//	Node* p = NULL;
//	while (tmp){
//		p = tmp;
//		if (scope > tmp->scope){
//			tmp = tmp->right;
//		}
//		else if (scope == tmp->scope){
//			if (tmp->scope != 0){
//				delSplay(p);
//				tmp = this->root;
//			}
//		}
//		else{
//			tmp = tmp->left;
//		}
//		
//	}
//}
void SymbolTable::delSplay(Node* key){
	if (!key){
		return;
	}
	int i = 0;
	this->splayRec(key,i);
	SymbolTable* sublef = new SymbolTable;
	sublef->root = this->root->left;
	SymbolTable* subrig = new SymbolTable;
	subrig->root = this->root->right;


	if (sublef->root){
		sublef->root->parent = NULL;
	}
	if (subrig->root){
		subrig->root->parent = NULL;
	}
	delete this->root;
	if (sublef->root != NULL){
		if (subrig->root != NULL){
			Node*max = sublef->root->max();
			int i = 0;
			sublef->splayRec(max, i);
			sublef->root->right = subrig->root;
			//how about parent
			if (subrig->root){
				subrig->root->parent = sublef->root;
			}
			else{
				delete subrig;
			}
		}
		else{
			delete subrig;
		}
		this->root = sublef->root;
		
	}
	else{
		delete sublef;
		this->root = subrig->root;
	}
}
int SymbolTable::compa(Node* a, Node*b){
	if (a->scope > b->scope){
		return 1;
	}
	else if (a->scope < b->scope){
		return -1;
	}
	else{
		if (a->name.compare(b->name) < 0){
			return -1;
		}
		else if (a->name.compare(b->name)>0){
			return 1;
		}
		else{
			return 0;
		}
	}
}
bool SymbolTable::insert(string name, type ty, int &numcom, int &numsplay,bool sta,int id){
	Node* nN;
	if (sta){
		nN = new Node(name, ty, 0,id);
	}
	else{
		nN = new Node(name, ty, this->beg,id);
	}
	Node* tmp = this->root;
	Node* p = NULL;
	
	while (tmp){
		p = tmp;
		if (compa(nN, tmp) < 0){
			numcom++;
			tmp = tmp->left;
		}
		else if(compa(nN,tmp)>0){
			tmp = tmp->right;
			numcom++;
		}
		else{
			delete nN;
			return false;
		}
	}
	nN->parent = p;
	if (!p){
		this->root = nN;
	}
	else if (compa(nN, p) < 0){
		p->left = nN;
	}
	else{
		p->right = nN;
	}
	splayRec(nN,numsplay);
	
	return true;
}
bool SymbolTable::insertFun(string name, type *arr,int arrsize, int &numcom, int &numsplay,int id){
	Node* nN = new Node(name, fun, arr, arrsize, 0,id);
	Node* tmp = this->root;
	Node* p = NULL;

	while (tmp){
		p = tmp;
		if (compa(nN, tmp) < 0){
			numcom++;
			tmp = tmp->left;
		}
		else if (compa(nN, tmp)>0){
			tmp = tmp->right;
			numcom++;
		}
		else{
			if(nN->arr){
			delete[] nN->arr;
			}
			delete nN;
			return false;
		}
	}
	nN->parent = p;
	if (!p){
		this->root = nN;
	}
	else if (compa(nN, p) < 0){
		p->left = nN;
	}
	else{
		p->right = nN;
	}
	splayRec(nN, numsplay);

	return true;
}
bool SymbolTable::assign(string name, string String, type ty, int &numcom, int &numsplay, string inst){
	Node* tmp = this->root;
	bool find = false;
	Node*p = NULL;
	while (tmp){
		p = tmp;
		if (this->beg < tmp->scope){
			tmp = tmp->left;
			numcom++;
		}
		else if (this->beg>tmp->scope){
			tmp = tmp->right;
			numcom++;
		}
		else{
			if (name.compare(tmp->name) < 0){
				tmp = tmp->left;
				numcom++;
			}
			else if (name.compare(tmp->name)>0){
				tmp = tmp->right;
				numcom++;
			}
			else{
				numcom++;
				find = true;
				break;
			}
		}
	}
	if (find){
		if (ty != p->ty){
			this->clear();
			throw TypeMismatch(inst);
		}
		splayRec(p, numsplay);
		return true;
	}
	return false;
}
bool SymbolTable::assignOld(string name, string nN, int& numcom, int& numsplay, string inst){
	int sco = this->beg;
	bool findnN = false;
	for (int i = sco; i >= 0; i--){
		if (lookup(nN, i, numcom, numsplay) != -1){
			findnN= true;
			break;
		}
	}
	if (findnN){
		type tmp = this->root->ty;
		bool findname = false;
		for (int i = sco; i >= 0; i--){
			if (lookup(name, i, numcom, numsplay) != -1){
				findname = true;
				break;
			}
		}
		if (findname){
			if (this->root->ty == tmp &&  this->root->ty!= fun){
				return true;
			}
			else{
				this->clear();
				throw TypeMismatch(inst);
			}
		}
		else{
			return false;
		}
	}
	return false;
}

bool SymbolTable::assignFun(string name, string String, parame*arr,int size, int &numcom, int &numsplay, string inst){
	
	bool find = false;
	int sco = this->beg;
	for (int i = sco; i >= 0; i--){
		if (lookup(String, i, numcom, numsplay) != -1){
			find = true;
			break;
		}
	}

	Node* reoffun = this->root;
	if (!find){
		return false;
	}
	else{
		
		if (size != this->root->arrsize-1 || this->root->ty!=fun){
			this->clear();
			throw TypeMismatch(inst);
		}
			
		for (int i = 0; i < size; i++){
			if (arr[i].na == "noname"){
				if (arr[i].ty != reoffun->arr[i]){
					this->clear();
					throw TypeMismatch(inst);
				}
			}
			else{
				bool find = false;
				int sco = this->beg;
				for (int j = sco; j >= 0; j--){
					if (lookup(arr[i].na, j, numcom, numsplay) != -1){
						find = true;
						break;
					}
				}
					
				if (!find){
					this->clear();
					throw Undeclared(inst);
				}
				else{
					if (this->root->ty != reoffun->arr[i]){
						this->clear();
						throw TypeMismatch(inst);
					}
				}
			}

		}
	}
	find = false;
	for (int i = sco; i >= 0; i--){
		if (lookup(name, i, numcom, numsplay) != -1){
			find = true;
			break;
		}
	}

	if (!find){
		this->clear();
		throw Undeclared(inst);
	}
	else{
		if (this->root->ty == fun || (this->root->ty != reoffun->arr[size])){
			this->clear();
			throw TypeMismatch(inst);
		}
	}
		
	return true;
}
//int SymbolTable::search(string name,int &numcom,int &numsplay){
//	Node* tmp = this->root;
//	Node* p = NULL;
//	while (tmp){
//		p = tmp;
//		if (name.compare(tmp->name) < 0){
//			tmp = tmp->left;
//			numcom++;
//		}
//		else if (name.compare(tmp->name)>0){
//			tmp = tmp->right;
//			numcom++;
//		}
//		else{
//			numcom++;
//			splayRec(p, numsplay);
//			return p->scope;
//		}
//	}
//	return -1;
//}
int SymbolTable::lookup(string name,int scope,int &numcom,int &numsplay){
	Node* tmp = this->root;
	Node* p = NULL;
	int num_tm = numcom;
	while (tmp){
		p = tmp;
		if (scope > tmp->scope){
			tmp = tmp->right;
			num_tm++;
		}
		else if (scope < tmp->scope){
			tmp = tmp->left;
			num_tm++;
		}
		else{
			if (name.compare(tmp->name) < 0){
				tmp = tmp->left;
				num_tm++;
			}
			else if (name.compare(tmp->name)>0){
				tmp = tmp->right;
				num_tm++;
			}
			else{
				num_tm++;
				numcom = num_tm;
				splayRec(p,numsplay);
				return p->scope;
			}
		}
	}
	return -1;
}

void SymbolTable::rotateRight(Node *node)
{
	Node* tmp = node->left;
	node->left = tmp->right;
	if (tmp->right){
		tmp->right->parent = node;
	}
	tmp->parent = node->parent;
	if (!node->parent){
		this->root = tmp;
	}
	else if (node->parent->left == node){
		node->parent->left = tmp;
	}
	else{
		node->parent->right = tmp;
	}
	tmp->right = node;
	node->parent = tmp;

}
void SymbolTable::rotateLeft(Node *node){
	Node* tmp = node->right;
	node->right = tmp->left;
	if (tmp->left){
		tmp->left->parent = node;
	}
	tmp->parent = node->parent; //loillllllllllll
	if (!node->parent){
		this->root = tmp;
	}
	else if (node->parent->left == node){
		node->parent->left = tmp;
	}
	else{
		node->parent->right = tmp;
	}
	tmp->left = node;
	node->parent = tmp;

}

void SymbolTable::splayRec(Node* node,int &numsplay){
	if (node == this->root){
		return;
	}
	while (node->parent){
		if (node->parent == this->root){
			if (node->parent->left == node){
				rotateRight(node->parent);
			}
			else{
				rotateLeft(node->parent);
			}
		}
		else{
			Node* pa = node->parent;
			Node* ga = pa->parent;
			if (node->parent->left == node && pa->parent->left == pa){
				rotateRight(ga);
				rotateRight(pa);
			}
			else if (node->parent->right == node && pa->parent->left == pa){
				rotateLeft(pa);
				rotateRight(ga);
			}
			else if (node->parent->right == node && pa->parent->right == pa){
				rotateLeft(ga);
				rotateLeft(pa);
			}
			else{
				rotateRight(pa);
				rotateLeft(ga);
			}
		}
	}
	numsplay++;
}
void SymbolTable::clear(){
	this->clearRec(this->root);
	this->root = NULL;
}
void SymbolTable::clearRec(Node*& node){
	if (node == NULL){
		return;
	}
	clearRec(node->left);
	clearRec(node->right);
	delete node;
}
