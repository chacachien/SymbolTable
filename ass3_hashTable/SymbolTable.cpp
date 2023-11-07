#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    fstream file;
    file.open(filename, ios::in);
    char tp[255];
    file.getline(tp, 255);
    if (regex_match(tp, regex("LINEAR[\\s][0-9]+[\\s][0-9]+")) || regex_match(tp, regex("DOUBLE[\\s][0-9]+[\\s][0-9]+"))){
        cmatch m;
        char* sub = tp + 7;
        regex_search(sub, m, regex("^[0-9]+"));
        int size = stoi(m[0]);
        regex_search(sub, m, regex("[0-9]+$"));
        int c = stoi(m[0]);
        if (tp[0] == 'L') {
            this->root = new OpenHash(size, c, 0, linearProbing);
        }
        else {
            this->root = new OpenHash(size, c, 0, doubleHashing);
        }
    }
    else if (regex_match(tp, regex("QUADRATIC[\\s][0-9]+[\\s][0-9]+[\\s][0-9]+"))) {
        cmatch m;
        char* sub = tp+ 10;

    	regex_search(sub, m, regex("^[0-9]+"));
    	int size = stoi(m[0]);
    	sub = sub + m[0].length() + 1;
    	regex_search(sub, m, regex("^[0-9]+"));
    	int c1 = stoi(m[0]);
    	regex_search(sub, m, regex("[0-9]+$"));
    	int c2 = stoi(m[0]);
        this->root = new OpenHash(size, c1, c2, quadraticProbing);
    }
    else{
       // node that
       // string j(tp);
        throw InvalidInstruction(tp);
    }
    while (!file.eof()) {
        file.getline(tp, 255);
        if (regex_match(tp, regex("INSERT[\\s][a-z][a-zA-Z0-9_]*([\\s][0-9]+)?"))) {
            string name;
            int numPara = -1;
            cmatch m;
            regex_search(tp,m, regex("[a-z][a-zA-Z0-9_]*"));
            name = m[0];
            char* sub = tp + 7 + name.length();
            if (regex_search(sub, m, regex("([0-9]+)$"))) {
                numPara = stoi(m[0]);
            }
           
            int slot;
            slot= root->insert(name, unknow, root->begin, numPara);
            if(slot==-3){
                throw Overflow(tp);
            }
            else {
                //cout << "Insert success: " << " ";
                //cout << "Name: " << name << " " << "NumPara: " << numPara << ", Slot: ";
                cout << slot << endl;
            }
        }
        else if (regex_match(tp, regex("ASSIGN\\s[a-z][a-zA-Z0-9_]*\\s(([0-9]+)|('[a-zA-Z0-9\\s]*')|([a-z][a-zA-Z0-9_]*)|([a-z][a-zA-Z0-9_]*((\\((([0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*),)*([0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*){1}\\))|(\\(\\)))))"))){
            string name;
            cmatch m;
            regex_search(tp, m, regex("[a-z][a-zA-Z0-9_]*"));
            name = m[0];
            char* sub = tp + 7 + name.length()+1;
            if (regex_search(sub, m, regex("^[0-9]+$"))) {
              
                //assign vao mot bien kieu int
                int slot = root->assignInt(name, numtype);
                    //cout << "Assign success a Number: " << number << ", So slot: ";
                if (slot == -2) {
                    throw TypeMismatch(tp);
                }
                cout << slot << endl;
            }
            else if (regex_search(sub, m, regex("^('[a-zA-Z0-9\\s]*')"))) {
                string str = m[0];
                // assign vao mot bien kieu string
                int slot = root->assignInt(name, strtype);
                //cout << "Assign success a string: " << str << ", So slot: ";
                if (slot == -2) {
                    throw TypeMismatch(tp);
                }
                cout << slot << endl;
            }
            else {
                //string fun;
                string subname;
                regex_search(sub, m, regex("[a-z][a-zA-Z0-9_]*"));
                subname = m[0];
                if (regex_search(sub, m, regex("((\\((([0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*),)*([0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*){1}\\))|(\\(\\)))"))) {
                    string para[100];
                    sub += subname.length();
                    int numofpara = 0;
                    type* typepara = NULL;
                    if (m[0] != "()") {
                        int i = 0;
                        while (sub[0] != ')' && regex_search(sub, m, regex("[0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*"))) {
                            para[i] = m[0];
                            i++;
                            sub += m[0].length() + 1;
                        }
                        numofpara = i;
                        typepara = new type[i];
                        for (int i = 0; i < numofpara; i++) {
                            if (regex_match(para[i], regex("[0-9]+"))) {
                                typepara[i] = numtype;
                            }
                            else if (regex_match(para[i], regex("'[a-zA-Z0-9\\s]*'"))) {
                                typepara[i]=strtype;
                            }
                            else {
                                typepara[i]=oldtype;
                            }
                            //cout << para[i] << "   ";
                        }
                    }
                    // assign vao mot ham
                    string nameUndeclared = "";
                    int slot = root->assignFun(name, subname, typepara, numofpara, para,nameUndeclared);
                    if (typepara != NULL) {
                        delete[] typepara;
                        typepara = NULL;
                    }
                    if (slot == -1) {
                        throw Undeclared(nameUndeclared);
                    }
                    else if (slot == -2) {
                        throw TypeMismatch(tp);
                    }
                    else if (slot == -3) {
                        throw TypeCannotBeInferred(tp);
                    }
                    else {
                       // cout << "Assign success a function! " << "So slot: ";
                        cout << slot << endl;
                    }
                }
                else {
                    // assign old name

                    int slot = root->assignOld(name, subname);
                   // cout << "Assign success old name: " << subname << ", Slot: " ;
                    if (slot == -2) {
                        throw TypeMismatch(tp);
                    }
                    else if (slot == -3) {
                        throw TypeCannotBeInferred(tp);
                    }
                    else {
                        cout << slot << endl;
                    }
                }
            }


        }
        else if (regex_match(tp, regex("CALL\\s[a-z][a-zA-Z0-9_]*((\\((([0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*),)*([0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*){1}\\))|(\\(\\)))"))) {
        cmatch m;
        string subname;
        char* sub = tp + 5;
        regex_search(tp, m, regex("[a-z][a-zA-Z0-9_]*"));
        subname = m[0];
            string para[20];
            sub += subname.length();
            int numofpara = 0;
            type* typepara = NULL; //remember delete
           if( m.suffix() != "()"){
                int i = 0;
                while (sub[0] != ')' && regex_search(sub, m, regex("[0-9]+|'[a-zA-Z0-9\\s]*'|[a-z][a-zA-Z0-9_]*"))) {
                    para[i] = m[0];
                    i++;
                    sub += m[0].length() + 1;
                }
                numofpara = i;
                typepara = new type[i];
                for (int i = 0; i < numofpara; i++) {
                    if (regex_match(para[i], regex("[0-9]+"))) {
                        typepara[i] = numtype;
                    }
                    else if (regex_match(para[i], regex("'[a-zA-Z0-9\\s]*'"))) {
                        typepara[i] = strtype;
                    }
                    else {
                        typepara[i] = oldtype;
                    }
                   // cout << para[i] << "   ";
                }
            }
           string nameUndeclared = "";
            int slot = root->callFun(subname, typepara, numofpara, para,nameUndeclared);
            if (typepara != NULL) {
                
                delete[] typepara;
            }
            if (slot == -1) {
                throw Undeclared(nameUndeclared);
            }
            else if (slot == -2) {
                throw TypeMismatch(tp);
            }
            else if (slot == -3) {
                throw TypeCannotBeInferred(tp);
            }
            else {
                //cout << "Call success!" << " Slot: ";
                cout << slot << endl;
            }
           
        }
        else if (regex_match(tp, regex("BEGIN"))) {
        root->begin++;
        }
        else if (regex_match(tp, regex("END"))) {
            root->end++;
            root->begin--;
            if (root->begin < 0) {
                throw UnknownBlock();
            }
            for (int i = 0; i < root->size; i++) {
                if (root->data[i].scope == root->begin+1) {
                    root->status[i] = DELETED;
                }
            }
        }
        else if (regex_match(tp, regex("LOOKUP\\s[a-z][a-zA-Z0-9_]*"))) {
            string name;
            cmatch m;
            regex_search(tp, m, regex("[a-z][a-zA-Z0-9_]*"));
            name = m[0];
            int slot = 0;
            int addr = root->lookup(name, slot);
            if (addr == -1) {
                throw Undeclared(name);
            }
            else {
                cout << addr << endl;
            }
        }
        else if (regex_match(tp, regex("PRINT"))) {
            string answer;
            for (int i = 0; i < root->size; i++) {
                if (root->status[i] == NON_EMPTY) {
                    answer += to_string(i) + " " + root->data[i].name + "//" + to_string(root->data[i].scope) + ";";
                }
            }
            if (answer != "") {
                answer.erase(answer.length() - 1);
                cout << answer << endl;
            }
        }
        else {
            throw InvalidInstruction(tp);
        }
    }
    if (root->begin != 0) {
        throw UnclosedBlock(root->begin);
    }

    // root->print();
}
unsigned long long mahoa(int scope, string name) {
   int trans;
    string tmp;

    for (int i = 0; i < (int)name.size(); i++) {
        trans = (name[i] - 48);
        tmp = tmp + to_string(trans);
    }
    tmp = to_string(scope) + tmp;
    return stoll(tmp);
    }
int linearProbing(unsigned long long key, int i, int m, int c, int c0) {
    return ((key % m) + static_cast<unsigned long long>(c) * i) % m;
}
int quadraticProbing(unsigned long long key, int i, int m, int c1, int c2) {
    return ((key % m) + static_cast<unsigned long long>(c1) * i + c2 * i * i) % m;
}
int doubleHashing(unsigned long long key, int i, int m, int c, int c0) {
    int h1 = key % m;
    int h2 = 1 + (key % (static_cast<unsigned long long>(m) - 2));
    return (h1 + c * i * h2) % m;
}
int OpenHash::insert(string name, type nametype, int scope, int numofpara) {
    unsigned long long key = mahoa(scope, name);
    int i = 0;
    while (i < size) {
        int addr = hp(key, i, size, c, c0);
        if (status[addr] != NON_EMPTY) {
                if (numofpara == -1) {
                    data[addr].name = name;
                    data[addr].Nodetype = nametype;
                    data[addr].scope = scope;
                    //data[addr].key=
                }
                else {
                    if (scope != 0) {
                        throw InvalidDeclaration(name);
                    }
                    else {
                        data[addr].name = name;
                        data[addr].Nodetype = funtype;
                        data[addr].scope = scope;
                        data[addr].arr = new type[numofpara];
                        data[addr].arrsize = numofpara;
                        for (int i = 0; i < numofpara; i++) {
                            data[addr].arr[i] = unknow;
                        }
                        data[addr].funreturn = unknow;
                    }
                }
                status[addr] = NON_EMPTY;
                return i;
        }
        else if (data[addr].name == name && data[addr].scope == scope) {
            if (numofpara != -1 && scope > 0) {
                throw InvalidDeclaration(name);
            }
            throw Redeclared(name);
        }
        i++;
    }
    return -3;
    
}
            
int OpenHash::assignInt(string name, type nametype)
{
    int slot = 0;
    int addr = lookup(name, slot);
    if (addr == -1) {
        throw Undeclared(name); 
    }
    else if (status[addr] == NON_EMPTY && data[addr].Nodetype == unknow) {
        data[addr].Nodetype = nametype;
        return slot;
    }
    else if (status[addr] == NON_EMPTY && data[addr].Nodetype == nametype) {
        return slot;
    }
    else if (status[addr] == NON_EMPTY && data[addr].Nodetype != nametype) {
        return -2;
    }
    return slot;
}
int OpenHash::assignFun(string name, string funame, type* para, int parasize, string namepara[],string& nameUndeclared)
{
    int slot = 0;
    int addrfun = lookup(funame, slot);
    // check function and parameter
    if (addrfun==-1) {
        nameUndeclared = funame;
        return -1;
    }
    else if (data[addrfun].Nodetype != funtype) {
        return -2;
    }
    else {
        if (data[addrfun].arrsize != parasize) {
            return -2;
        }
        for (int i = 0; i < parasize; i++) {
            if (data[addrfun].arr[i] == unknow) {
                if (para[i] != oldtype) {
                    data[addrfun].arr[i] = para[i];
                }
                else {
                    int addr = lookup(namepara[i],slot);
                    if (addr == -1) {
                        nameUndeclared = namepara[i];
                        return -1;
                    }
                    else if (data[addr].Nodetype == unknow) {
                        return -3;
                    }
                    else if(data[addr].Nodetype!=funtype) {
                        data[addrfun].arr[i] = data[addr].Nodetype;
                    }
                    else {
                        return -2;
                    }
                }
            }
            else {
                if (para[i] != oldtype) {
                    if (data[addrfun].arr[i] != para[i]) {
                        return -2;
                    }
                }
                else {
                    int addr = lookup(namepara[i],slot);
                    if (addr == -1) {
                        nameUndeclared = namepara[i];
                        return -1;
                    }
                    else if (data[addr].Nodetype == unknow) {
                        data[addr].Nodetype = data[addrfun].arr[i];
                    }
                    else if(data[addr].Nodetype!=data[addrfun].arr[i]) {
                        return -2;
                    }
                }
            }
        }
    }
    //14-12
    if (data[addrfun].funreturn == oldtype) {
        return -2;
    }
    // check name
    int addrname = lookup(name, slot);
    if (addrname == -1) {
        nameUndeclared = name;
       return -1;
    }
    else {
        /*if (data[addrfun].funreturn == oldtype) {
            return -2;
        }*/
        if (data[addrfun].funreturn == unknow && data[addrname].Nodetype == unknow) {
            return -3;
        }
        else if (data[addrfun].funreturn == unknow && data[addrname].Nodetype != funtype && data[addrname].Nodetype!=unknow) {
            data[addrfun].funreturn = data[addrname].Nodetype;
        }
        else if (data[addrfun].funreturn != unknow && data[addrname].Nodetype != funtype && data[addrname].Nodetype == unknow) {
            data[addrname].Nodetype = data[addrfun].funreturn;
        }
        //not right, fixx
        else if(data[addrfun].funreturn!= data[addrname].Nodetype || data[addrname].Nodetype==funtype) {
            return -2;
        }
    }
    return slot;
}
int OpenHash::assignOld(string name, string oldname)
{
    int slot = 0;
    int addr_oldname = lookup(oldname, slot);
    int addr_name = lookup(name, slot);
    if (addr_oldname == -1) {
        throw Undeclared(oldname);
    }
    else if (data[addr_oldname].Nodetype == funtype ) {
        //14-12
        throw InvalidInstruction("ASSIGN "+ name + " " + oldname);
        //old name but is func
        return -2;
    }
    else {
        if (addr_name == -1) {
            throw Undeclared(name);
        }
        else if (data[addr_name].Nodetype == funtype) {
            return -2;
        }
        else {
            if (data[addr_name].Nodetype == unknow && data[addr_oldname].Nodetype == unknow) {
                return -3;
            }
            else if (data[addr_name].Nodetype != unknow && data[addr_oldname].Nodetype == unknow) {
                data[addr_oldname].Nodetype = data[addr_name].Nodetype;
            }
            else if (data[addr_name].Nodetype == unknow && data[addr_oldname].Nodetype != unknow) {
                data[addr_name].Nodetype = data[addr_oldname].Nodetype;
            }
            else if (data[addr_name].Nodetype != data[addr_oldname].Nodetype) {
                return -2;
            }
        }
    }
    return slot;
}
int OpenHash::callFun(string name, type* para, int sizepara, string namepara[], string& nameUndeclared)
{
    int slot = 0;
    int addrfun = lookup(name, slot);
    if (addrfun == -1) {
        nameUndeclared = name;
        return -1;
    }
    if (data[addrfun].arrsize != sizepara || data[addrfun].Nodetype!=funtype) {
        return -2;
    }
    for (int i = 0; i < sizepara; i++) {
        if (data[addrfun].arr[i] == unknow) {
            if (para[i] != oldtype) {
                data[addrfun].arr[i] = para[i];
            }
            else {
                int addr = lookup(namepara[i], slot);
                if (addr == -1) {
                    nameUndeclared = namepara[i];
                    return -1;
                }
                else if (data[addr].Nodetype == unknow) {
                    return -3;
                }
                else if (data[addr].Nodetype != funtype) {
                    data[addrfun].arr[i] = data[addr].Nodetype;
                }
                else {
                    // error
                    return -2;
                }
            }
        }
        else {
            if (para[i] != oldtype) {
                if (data[addrfun].arr[i] != para[i]) {
                    return -2;
                }
            }
            else {
                int addr = lookup(namepara[i], slot);
                if (addr == -1) {
                    nameUndeclared = namepara[i];
                    return -1;
                }
                else if (data[addr].Nodetype == unknow) {
                    data[addr].Nodetype = data[addrfun].arr[i];
                }
                else if (data[addr].Nodetype != data[addrfun].arr[i]) {
                    return -2;
                }
            }
        }
    }
    if (data[addrfun].funreturn == unknow) {
        data[addrfun].funreturn = oldtype;
    }
    else if (data[addrfun].funreturn != oldtype) {
        return -2;
    }
    return slot;
}
int OpenHash::lookup(string name,int& slot)
{
    for (int j = this->begin; j >= 0; j--) {
        unsigned long long key = mahoa(j, name);
        int i = 0;
        while (i < this->size) {
            int addr = this->hp(key, i, this->size, this->c, this->c0);
            if (status[addr] == NON_EMPTY && data[addr].name==name && data[addr].scope==j) {
                slot = i;
                return addr;
            }
            i++;
        }
    }
    return -1;
}
void OpenHash::clear()
{
    for (int i = 0; i < size; i++) {
        if (data[i].arr != NULL) {
            delete[] data[i].arr;
        }
    }
    if (status != NULL) {
        delete[] status;
        status = NULL;
      
    }
    if (data != NULL) {
        delete[]data;
        data = NULL;
    }
}
//void OpenHash::print() {
//    for (int i = 0; i < size; i++) {
//        cout << i << " " << data[i].name << " " << data[i].scope << " " <<data[i].Nodetype<<" "<< status[i] << endl;
//    }
//}
OpenHash::~OpenHash() {
    clear();
    
}