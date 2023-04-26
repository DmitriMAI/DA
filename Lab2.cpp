#include <iostream>
#include<sstream>
#include <fstream>

const int ONE = 1;
const int SEVEN = 7;
const int EIGHT = 8;

class Mynode{
private:
    std::string key;
    unsigned long long value;
    int index;
    Mynode* right_node;
    Mynode* left_node;
public:
    void Change_node(Mynode*& node){
        key = node->Get_key();
        value = node->Get_value();
    }
    Mynode(std::string& key, unsigned long long& value, int index): key(key), value(value), index(index), right_node(nullptr), left_node(nullptr){};
    void Set_right_node(Mynode* node){
        right_node = node;
    }
    void Set_left_node(Mynode* node){
        left_node = node;
    }
    void Set_value(unsigned long long new_val){
        value = new_val;
    }
    Mynode* Get_right_node(){
        return right_node;
    }
    Mynode* r(){
        return right_node;
    }
    Mynode* l(){
        return left_node;
    }
    Mynode* Get_left_node(){
        return left_node;
    }
    std::string& Get_key(){
        return key;
    };
    int Get_index(){
        return index;
    };
    void Set_index(int new_index){
        index = new_index;
    };
    unsigned long long Get_value(){
        return value;
    }
    void Set_key(std::string new_key){
        key = new_key;
    }
};

bool Get_bit_in_string(std::string& string, int index){
    unsigned int string_index = index/EIGHT;
    if (string_index >= string.size()){
        return false;
    }
    return string[string_index] & (ONE << (SEVEN - index % EIGHT) );
}

int First_diff_bit(std::string& first, std::string& second){
    int maxbits = std::max(first.size(), second.size()) * EIGHT;
    for (int i = 0; i < maxbits; ++i){
        if (Get_bit_in_string(first, i) != Get_bit_in_string(second, i)){
            return i;
        }
    }
    std::cout << "ERROR: in diff";
    exit(ONE);
}

class Patricia{
private:
    Mynode* main_node = nullptr;
public:
    bool Empty(){
        if (main_node == nullptr){
            return true;
        };
        return false;
    };
    Mynode* Find(std::string& key){
        if (Empty()){
            return nullptr;
        }
        if (main_node->Get_key() == key){
            return main_node;
        }
        Mynode* prev_node = main_node;
        Mynode* now_node = main_node->Get_left_node();
        while (now_node->Get_index() > prev_node->Get_index()){
            prev_node = now_node;
            if (Get_bit_in_string(key, now_node->Get_index())){
                now_node = now_node->Get_right_node();
            } else {
                now_node = now_node->Get_left_node();
            }
        }
        return now_node;
    }
    std::pair<Mynode*, Mynode*> Find_to_insert(std::string& key, int index){
        Mynode* prev_node = main_node;
        Mynode* now_node = main_node->Get_left_node();
        while ((now_node->Get_index() > prev_node->Get_index()) && (now_node->Get_index() < index)) {
            prev_node = now_node;
            if (Get_bit_in_string(key, now_node->Get_index())){
                now_node = now_node->Get_right_node();
            } else {
                now_node = now_node->Get_left_node();
            }
        }
        return std::pair<Mynode*, Mynode*>(prev_node,now_node);
    }
    bool First_case_delete(Mynode* p, Mynode* x){
        if (x == p->Get_right_node()){
            if (x == x->Get_right_node()){
                p->Set_right_node(x->Get_left_node());
            }else{
                p->Set_right_node(x->Get_right_node());
            }
        } else{
            if (x == x->Get_left_node()){
                p->Set_left_node(x->Get_right_node());
            }else{
                p->Set_left_node(x->Get_left_node());
            }
        }
        delete x;
        x = nullptr;
        return true;
    }
    bool Second_case_delete(Mynode* x, Mynode* p, Mynode* m){
        Mynode* prev_node = main_node;
        Mynode* now_node = main_node->Get_left_node();
        while (now_node->Get_index() > prev_node->Get_index()){
            prev_node = now_node;
            if (Get_bit_in_string(p->Get_key(), now_node->Get_index())){
                now_node = now_node->Get_right_node();
            } else {
                now_node = now_node->Get_left_node();
            }
        }
        Mynode* q = prev_node;
        Mynode* n;
        if (p->Get_right_node() == x){
            n = p->Get_left_node();
        }else{
            n = p->Get_right_node();
        }
        if (n == p){
            n = x;
        }
        if (m->Get_right_node() == p){
            m->Set_right_node(n);
        }else{
            m->Set_left_node(n);
        }
        if (q->Get_right_node() == p){
            q->Set_right_node(x);
        }else{
            q->Set_left_node(x);
        }
        x->Set_value(p->Get_value());
        x->Set_key(p->Get_key());
        delete p;
        p = nullptr;
        return true;
    }
    bool Delete(std::string& key){
        if (Empty()){
            return false;
        }
        if ((main_node->Get_left_node() == main_node) && (main_node->Get_key() == key)){
            delete main_node;
            main_node = nullptr;
            return true;
        }
        Mynode* point_to_parent_of_deleted;
        Mynode* tmp_prev = main_node;
        Mynode* deleted = main_node->Get_left_node();
        while (deleted->Get_index() > tmp_prev->Get_index()){
            point_to_parent_of_deleted = tmp_prev;
            tmp_prev = deleted;
            if (Get_bit_in_string(key, deleted->Get_index())){
                deleted = deleted->Get_right_node();
            } else {
                deleted = deleted->Get_left_node();
            }
        }
        if (deleted->Get_key() != key){
            return false;
        }
        if (tmp_prev == deleted){
            First_case_delete(point_to_parent_of_deleted, deleted);
            return true;
        }
        Second_case_delete(deleted, tmp_prev,point_to_parent_of_deleted);
        return true;
    }
    bool Add(std::string& key, unsigned long long& value){
        if (Empty()){
            Mynode* node = new Mynode(key, value, 0);
            node->Set_right_node(nullptr);
            node->Set_left_node(node);
            main_node = node;
        } else{
            Mynode* node = Find(key);
            if (node->Get_key() == key){
                return false;
            }
            int new_index = First_diff_bit(node->Get_key(), key);
            Mynode* new_node = new Mynode(key, value, new_index);
            std::pair<Mynode*, Mynode*> place_to_instert = Find_to_insert(key, new_index);
            if (Get_bit_in_string(key, new_index)) {
                new_node->Set_right_node(new_node);
                new_node->Set_left_node(place_to_instert.second);
            } else{
                new_node->Set_left_node(new_node);
                new_node->Set_right_node(place_to_instert.second);
            }
            Mynode* prev_node = place_to_instert.first;
            if (prev_node->Get_left_node() == place_to_instert.second){
                prev_node->Set_left_node(new_node);
            }
            else{
                prev_node->Set_right_node(new_node);
            }
        }
        return true;
    };
    void Remove_all_tree(){
        if (main_node == nullptr){
            return;
        }
        if (main_node != main_node->Get_left_node()){
            Mynode* tmp_prev = main_node;
            Mynode* deleted = main_node->Get_left_node();
            while (deleted->Get_index() > tmp_prev->Get_index()){
                tmp_prev = deleted;
                if (Get_bit_in_string(main_node->Get_key(), deleted->Get_index())){
                    deleted = deleted->Get_right_node();
                } else {
                    deleted = deleted->Get_left_node();
                }
            }
            tmp_prev->Set_left_node(main_node->Get_left_node());
            tmp_prev->Set_right_node(nullptr);
            delete main_node;
            main_node = tmp_prev;
            main_node->Set_index(-ONE);
        }
        delete main_node;
        main_node = nullptr;
    }
    ~Patricia(){
        Remove_all_tree();
    }
    Mynode* Get_main_node(){
        return main_node;
    }
    bool Save(Mynode* now_node, std::ofstream &oStream){
        if (now_node == nullptr){
            return true;
        }
        int node_index = now_node->Get_index();
        int key_size = now_node->Get_key().size();
        unsigned long long node_val = now_node->Get_value();
        oStream.write((char *) &key_size, sizeof(int));
        oStream.write(now_node->Get_key().c_str(), sizeof(char) * now_node->Get_key().size());
        oStream.write((char *) &node_index, sizeof(int));
        oStream.write((char *) &node_val, sizeof(unsigned long long));
        if (now_node->Get_left_node() && now_node->Get_left_node()->Get_index() > node_index) {
            Save(now_node->Get_left_node(), oStream);
        }
        if (now_node->Get_right_node() && now_node->Get_right_node()->Get_index() > node_index) {
            Save(now_node->Get_right_node(), oStream);
        }
        return true;
    }
    bool Load(std::istream &iStream){
        int keySize;
        Patricia loadTree;
        while (iStream.read((char *) &keySize, sizeof(keySize))) {
            std::string new_key;
            unsigned long long value;
            int index;
            new_key.resize(keySize);
            iStream.read((char *) new_key.data(), keySize);
            iStream.read((char *) &index, sizeof(index));
            iStream.read((char *) &value, sizeof(value));
            loadTree.Add(new_key, value);
        }
        std::swap(*this, loadTree);
        return true;
    }
};

void Make_lower_string (std::string &key) {
    for (char &i: key) {
        i = std::tolower(i);
    }
}




class Parser {
private:
    Patricia* tree;
    std::stringstream& ss;
public:
    Parser(Patricia*& tree,std::stringstream& ss):tree(tree), ss(ss){}

    void ParseLine(std::string& line){
        ss << line;
        if (line.empty()) {
            return;
        }
        std::string command;
        ss >> command;
        if (command == "+"){
            Adding();
        }
        else if (command == "-"){
            Deletion();
        }
        else if (command == "!"){
            SaveOrLoad();
        }
        else {
            Finding(command);
        }
        ss.clear();
    }

    void Adding(){
        std::string key;
        unsigned long long value;
        ss >> key >> value;
        Make_lower_string(key);
        if (tree->Add(key,value)){
            std::cout << "OK\n";
        } else{
            std::cout << "Exist\n";
        }
    }
    void Deletion(){
        std::string key;
        ss >> key;
        Make_lower_string(key);
        if (tree->Delete(key)){
            std::cout << "OK\n";
        }else {
            std::cout << "NoSuchWord\n";
        }
    }
    void SaveOrLoad(){
        std::string command;
        ss >> command;
        std::string path;
        ss >> path;
        if (command == "Save"){
            std::ofstream stream(path, std::ios::binary);
            if (tree->Save(tree->Get_main_node(), stream)){
                std::cout << "OK\n";
            }else {
                std::cout << "ERROR: in save\n";
            }
            stream.close();
        }
        else {
            std::ifstream stream(path, std::ios::binary);
            tree->Remove_all_tree();
            if (tree->Load(stream)){
                std::cout << "OK\n";
            }else {
                std::cout << "ERROR: in load\n";
            }
            stream.close();
        }
    }
    void Finding(std::string key){
        Make_lower_string(key);
        Mynode* node = tree->Find(key);
        if (node!= nullptr && node->Get_key() == key){
            std::cout << "OK: " << node->Get_value() << '\n';
        }else {
            std::cout << "NoSuchWord\n";
        }
    }
};

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Patricia* treePtr = new Patricia();
    std::string line;
    std::stringstream ss;
    Parser parser(treePtr, ss);
    while(std::getline(std::cin, line)){
        parser.ParseLine(line);
    }
    delete treePtr;
    return 0;
}
