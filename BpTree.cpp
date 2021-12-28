//
// Created by leomo on 23/12/2021.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include "BpTree.h"





int BpTree::search(int index) {
    if (root == nullptr) {
        std::cout << "Tree is empty\n";
        return -1;
    } else {
        Node *cursor = root;
        //busca o id até chegar na folha
        while (!cursor->isLeaf) {
            if(cursor->leftId >= index){
                cursor = cursor->left;
            }else if(cursor->rightId <= index){
                cursor = cursor->right;
            }else{
                cursor = cursor->middle;
            }
        }
        //caso encontre na folha ele verifica se existe na esquerda ou direita imprime seu conteudo do arquivo e retorna seu ponteiro
        if(cursor->rightId == index||cursor->leftId == index ){
            //abre o arquivo em modo leitura e imprime os dados do arquivo
            std::ifstream namesR(nameFile, std::ios::in | std::ios::binary);
            namesR.seekg(ptr[index],std::ios::beg);
            char *name = (char *)(malloc(regSize * sizeof(char)));
            if(namesR.is_open()){
                namesR.read(name,(int)sizeof(char)*regSize);
                std::cout << name << std::endl;
            }else{
                std::cout << "File can't be open" << std::endl;
            }
            namesR.close();
            return ptr[index];
        }else{
            std::cout << "Not found" << std::endl;
            return -1;
        }
    }
}
void BpTree::print(Node *cursor) {
    if (cursor != nullptr) {
        if(!cursor->isLeaf) {
            std::cout << cursor->leftId << " " << cursor->rightId << " ";
            std::cout << " ( < ";
            print(cursor->left);
            std::cout << " = ";
            print(cursor->middle);
            std::cout << "  > ";
            print(cursor->right);
            std::cout << ") " ;
        }else{
            int lptr = cursor->leftId != 0?ptr[cursor->leftId]: 0;
            int rptr = cursor->rightId != 0?ptr[cursor->rightId]: 0;
            std::cout  << " key: " << cursor->leftId << "ptr: " << lptr  << " " << "key: " << cursor->rightId << "ptr: " << rptr  << "->";
        }
    }
}

void BpTree::insert(int x,char *name) {
    if (root == nullptr) {
        root = new Node;

        root->rightId = x;
        root->size = 1;
    } else {
        Node *cursor = root;
        Node *parent;
        while (!cursor->isLeaf) {
            parent = cursor;

            if( x < cursor->leftId ){
                cursor = cursor->left;
            }else if(cursor->rightId == 0 || x < cursor->rightId){
                cursor = cursor->middle;
            }else{
                cursor = cursor->right;
            }
        }
        // inserção nos nós da folha
        if (cursor->size < 2) {
            if(cursor->size == 0){
                cursor->leftId = x;
            }else if( x > cursor->leftId){
                cursor->rightId = x;
            }else{
                cursor->rightId = cursor->leftId;
                cursor->leftId = x;
            }
            cursor->size++;
        }else { // caso a folha esteja cheia inserir na folha e rebalancear a arvore SPLIT
            std::cout << "SPLIT" <<std::endl;
            Node *newNode = new Node;
            Node *split = new Node;
            newNode->isLeaf = false;
            newNode->size = 1;
            //se o maior nó da folha for menor que o valor a ser inserido
            if(x > cursor->rightId){
                //meio == cursor->rightId
                //   2      4
                //1     2      4
                newNode->leftId = x;
                newNode->left = cursor;
                split->leftId = x;
                split->size = 1;
                newNode->middle = split;
                newNode->right = new Node;
            }else if(x < cursor->leftId){
                //meio == cursor->leftId
                newNode->rightId = cursor->leftId;
                newNode->middle = split;
                split->rightId = x;
                split->size = 1;
                newNode->right = cursor;
                newNode->left = new Node;
            }else{
                //meio == x
                newNode->rightId = x;

                split->rightId = cursor->leftId;
                split->size = 1;
                cursor->leftId = x;
                newNode->middle = split;
                newNode->right = cursor;
                newNode->left = new Node;
            }

            if (cursor == root) {
                root = newNode;
            } else {
                insertInternal(newNode->leftId, parent, newNode);
            }
        }
    }
    //abre o arquivo de nomes
    int regAddress = checkEmptyRegister();
    if(regAddress == -1){
        std::ofstream namesW(nameFile, std::ios::app | std::ios::out | std::ios::binary);
        //se aberto busca o final do arquivo
        namesW.seekp(0,std::ios::end);
        //salva o endereço onde vai ser inserido o novo registro
        int address = (int)namesW.tellp();
        //insere o novo registro no arquivo
        namesW.write(name, (int)sizeof(char)*regSize);
        //salva o endereço do arquivo no ponteiro relativo ao id da arvore
        ptr[x]=address;
        namesW.close();
    }else{
        //abre o arquivo em função leitura escirta para posicionar e escrever o registro caso exista um registro vazio
        std::fstream namesWR(nameFile, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        //busca o endereço a ser removido
        if(namesWR.is_open()){
            namesWR.seekp(regAddress,std::ios_base::beg);
            int add = (int)namesWR.tellp();
            namesWR.write(name, (int)sizeof(char)*regSize);
            namesWR.close();
            ptr[x]=add;
        }
    }

    //reescreve a arvore no arquivo
    writeTree();
}

void BpTree::insertInternal(int x, Node *cursor, Node *child) {
    // If we doesn't have overflow
    if (cursor->size < 2) {


        cursor->rightId = x;
        cursor->size++;
        cursor->right = child->middle;


    }else {// For overflow, break the node
        Node *newNode = new Node;
        newNode->isLeaf = false;
        newNode->size = 1;
        Node *splitL = new Node;
        Node *splitR = new Node;
        splitL->size=1;
        splitL->isLeaf = false;
        splitL->leftId = cursor->leftId;
        splitL->left = cursor->left;
        splitL->middle = cursor->middle;
        splitL->right = new Node;
        splitR->size=1;
        splitR->isLeaf = false;
        splitR->leftId = x;
        splitR->left = cursor->right;
        splitR->middle = child->middle;
        splitR->right = new Node;
        newNode->leftId = cursor->rightId;
        newNode->left = splitL;
        newNode->middle = splitR;
        newNode->right = new Node;
        if (cursor == root) {
            root = newNode;
        } else {
            insertInternal(newNode->leftId, findParent(root,cursor), newNode);
        }
//        //se o maior nó da folha for menor que o valor a ser inserido
//        if (x > cursor->rightId) {
//            //meio == cursor->rightId
//            newNode->rightId = cursor->rightId;
//            newNode->middle = cursor;
//            newNode->right = child;
//            newNode->left = new Node;
//            cursor->right = new Node;
//        } else if (x < cursor->leftId) {
//            //meio == cursor->leftId
//            newNode->rightId = cursor->leftId;
//            newNode->middle = child;
//            newNode->right = cursor;
//            newNode->left = new Node;
//        } else {
//            //meio == x
//            newNode->rightId = x;
//            newNode->middle = child;
//            newNode->right = cursor;
//            newNode->left = new Node;
//        }
//
//        if (cursor == root) {
//            root = newNode;
//        } else {
//            insertInternal(newNode->rightId, findParent(root,cursor), newNode);
//        }
    }
}
Node* BpTree::findParent(Node* cursor,
                         Node* child)
{
    Node *parent = nullptr;

    // If cursor reaches the end of Tree
    if (cursor->isLeaf){
        return nullptr;
    }

    if(cursor->left == child){
        parent = cursor;
        return parent;
    }else if(cursor->right == child){
        parent = cursor;
        return parent;
    }else if(cursor->middle == child){
        parent = cursor;
        return parent;
    }else{
        findParent(cursor->right,child);
        findParent(cursor->middle,child);
        findParent(cursor->left,child);
    }
    return parent;
}

void BpTree::remove(int x) {
    if (root == nullptr) {
        std::cout << "Tree empty\n";
    } else {
        Node *cursor = root;
        Node *parent;
        int leftSibling, rightSibling;
        while (!cursor->isLeaf) {
            parent = cursor;
            if(cursor->rightId <= x){
                cursor = cursor->right;
            }else if(cursor->leftId > x){
                cursor = cursor->left;
            }else{
                cursor = cursor->middle;
            }
        }
        int pos;
        if(x == cursor->leftId){
            pos = 0;
            cursor->leftId = 0;
        }else if(x == cursor->rightId){
            pos = 1;
            cursor->rightId = 0;
        }else{
            std::cout << "Not found\n";
            return;
        }
        cursor->size--;
        int address = ptr[x];
        //abre o arquivo em função leitura escirta para posicionar e apagar o registro removido
        std::fstream namesW(nameFile, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        //busca o endereço a ser removido
        if(namesW.is_open()){
            namesW.seekp(address,std::ios_base::beg);
            int add = (int)namesW.tellp();
            namesW.write("                ", (int)sizeof(char)*regSize);
            namesW.close();
            //adiciona o endereço nos arquivos de registro vazio
            emptyReg.push_back(add);
            writeEmptyRegister();
        }
        ptr[x] = -1;

        if (cursor == root) {

            if (cursor->size == 0) {
                std::cout << "Tree died\n";
                delete cursor;
                root = nullptr;
            }
            return;
        }

        if (cursor->size != 0) {
            return;
        }

        if (pos==0) {
//            Node *leftNode = parent->left;
//            if (leftNode->size >= (MAX + 1) / 2 + 1) {
//                for (int i = cursor->size; i > 0; i--) {
//                    cursor->key[i] = cursor->key[i - 1];
//                }
//                cursor->size++;
//                cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
//                cursor->ptr[cursor->size - 1] = NULL;
//                cursor->key[0] = leftNode->key[leftNode->size - 1];
//                leftNode->size--;
//                leftNode->ptr[leftNode->size] = cursor;
//                leftNode->ptr[leftNode->size + 1] = NULL;
//                parent->key[leftSibling] = cursor->key[0];
//                return;
//            }
        }
//        if (rightSibling <= parent->size) {
//            Node *rightNode = parent->ptr[rightSibling];
//            if (rightNode->size >= (MAX + 1) / 2 + 1) {
//                cursor->size++;
//                cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
//                cursor->ptr[cursor->size - 1] = NULL;
//                cursor->key[cursor->size - 1] = rightNode->key[0];
//                rightNode->size--;
//                rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
//                rightNode->ptr[rightNode->size + 1] = NULL;
//                for (int i = 0; i < rightNode->size; i++) {
//                    rightNode->key[i] = rightNode->key[i + 1];
//                }
//                parent->key[rightSibling - 1] = rightNode->key[0];
//                return;
//            }
//        }
//        if (leftSibling >= 0) {
//            Node *leftNode = parent->ptr[leftSibling];
//            for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) {
//                leftNode->key[i] = cursor->key[j];
//            }
//            leftNode->ptr[leftNode->size] = NULL;
//            leftNode->size += cursor->size;
//            leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
//            removeInternal(parent->key[leftSibling], parent, cursor);
//            delete[] cursor->key;
//            delete[] cursor->ptr;
//            delete cursor;
//        } else if (rightSibling <= parent->size) {
//            Node *rightNode = parent->ptr[rightSibling];
//            for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
//                cursor->key[i] = rightNode->key[j];
//            }
//            cursor->ptr[cursor->size] = NULL;
//            cursor->size += rightNode->size;
//            cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
//            cout << "Merging two leaf nodes\n";
//            removeInternal(parent->key[rightSibling - 1], parent, rightNode);
//            delete[] rightNode->key;
//            delete[] rightNode->ptr;
//            delete rightNode;
//        }
    }
}



//void BPTree::removeInternal(int x, Node *cursor, Node *child) {
//    if (cursor == root) {
//        if (cursor->size == 1) {
//            if (cursor->ptr[1] == child) {
//                delete[] child->key;
//                delete[] child->ptr;
//                delete child;
//                root = cursor->ptr[0];
//                delete[] cursor->key;
//                delete[] cursor->ptr;
//                delete cursor;
//                cout << "Changed root node\n";
//                return;
//            } else if (cursor->ptr[0] == child) {
//                delete[] child->key;
//                delete[] child->ptr;
//                delete child;
//                root = cursor->ptr[1];
//                delete[] cursor->key;
//                delete[] cursor->ptr;
//                delete cursor;
//                cout << "Changed root node\n";
//                return;
//            }
//        }
//    }
//    int pos;
//    for (pos = 0; pos < cursor->size; pos++) {
//        if (cursor->key[pos] == x) {
//            break;
//        }
//    }
//    for (int i = pos; i < cursor->size; i++) {
//        cursor->key[i] = cursor->key[i + 1];
//    }
//    for (pos = 0; pos < cursor->size + 1; pos++) {
//        if (cursor->ptr[pos] == child) {
//            break;
//        }
//    }
//    for (int i = pos; i < cursor->size + 1; i++) {
//        cursor->ptr[i] = cursor->ptr[i + 1];
//    }
//    cursor->size--;
//    if (cursor->size >= (MAX + 1) / 2 - 1) {
//        return;
//    }
//    if (cursor == root)
//        return;
//    Node *parent = findParent(root, cursor);
//    int leftSibling, rightSibling;
//    for (pos = 0; pos < parent->size + 1; pos++) {
//        if (parent->ptr[pos] == cursor) {
//            leftSibling = pos - 1;
//            rightSibling = pos + 1;
//            break;
//        }
//    }
//    if (leftSibling >= 0) {
//        Node *leftNode = parent->ptr[leftSibling];
//        if (leftNode->size >= (MAX + 1) / 2) {
//            for (int i = cursor->size; i > 0; i--) {
//                cursor->key[i] = cursor->key[i - 1];
//            }
//            cursor->key[0] = parent->key[leftSibling];
//            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
//            for (int i = cursor->size + 1; i > 0; i--) {
//                cursor->ptr[i] = cursor->ptr[i - 1];
//            }
//            cursor->ptr[0] = leftNode->ptr[leftNode->size];
//            cursor->size++;
//            leftNode->size--;
//            return;
//        }
//    }
//    if (rightSibling <= parent->size) {
//        Node *rightNode = parent->ptr[rightSibling];
//        if (rightNode->size >= (MAX + 1) / 2) {
//            cursor->key[cursor->size] = parent->key[pos];
//            parent->key[pos] = rightNode->key[0];
//            for (int i = 0; i < rightNode->size - 1; i++) {
//                rightNode->key[i] = rightNode->key[i + 1];
//            }
//            cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
//            for (int i = 0; i < rightNode->size; ++i) {
//                rightNode->ptr[i] = rightNode->ptr[i + 1];
//            }
//            cursor->size++;
//            rightNode->size--;
//            return;
//        }
//    }
//    if (leftSibling >= 0) {
//        Node *leftNode = parent->ptr[leftSibling];
//        leftNode->key[leftNode->size] = parent->key[leftSibling];
//        for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++) {
//            leftNode->key[i] = cursor->key[j];
//        }
//        for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++) {
//            leftNode->ptr[i] = cursor->ptr[j];
//            cursor->ptr[j] = NULL;
//        }
//        leftNode->size += cursor->size + 1;
//        cursor->size = 0;
//        removeInternal(parent->key[leftSibling], parent, cursor);
//    } else if (rightSibling <= parent->size) {
//        Node *rightNode = parent->ptr[rightSibling];
//        cursor->key[cursor->size] = parent->key[rightSibling - 1];
//        for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++) {
//            cursor->key[i] = rightNode->key[j];
//        }
//        for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++) {
//            cursor->ptr[i] = rightNode->ptr[j];
//            rightNode->ptr[j] = NULL;
//        }
//        cursor->size += rightNode->size + 1;
//        rightNode->size = 0;
//        removeInternal(parent->key[rightSibling - 1], parent, rightNode);
//    }
//}

Node *BpTree::readTree() {
    std::string line;
    std::string data;
    Node *newRoot = new Node;
    Node *cursor = newRoot;
    std::map<int,Node*> ft;
    int fatherPos = 0;
    int a=0;
    char delimiter = '|';
    std::ifstream treeF(treeFile);
    if(treeF.is_open()){
        while(!treeF.eof()){
            int pos = (int)treeF.tellg();

            getline(treeF,line);
            if(line.size()<5){
                break;
            }
            for(int i = 0;i<=7;i++) {
                data = line.substr(0, line.find(delimiter));
                line = line.substr(line.find(delimiter) + 1);
                switch (i) {
                    case 0:
                        cursor->isLeaf = data == "true";
                        if(!cursor->isLeaf){
                            ft[pos+a]=cursor;
                        }
                        break;
                    case 1:
                        cursor->leftId = stoi(data);
                        break;
                    case 2:
                        cursor->rightId = stoi(data);
                        break;
                    case 3:
                        cursor->size = stoi(data);
                        break;
                    case 4:
                        fatherPos = stoi(data);
                        break;
                    case 5:
                        if(cursor->isLeaf && cursor->leftId != 0){
                            ptr[cursor->leftId] = stoi(data);
                        }
                        break;
                    case 6:
                        if(cursor->isLeaf && cursor->rightId != 0){
                            ptr[cursor->rightId] = stoi(data);
                        }
                        break;
                    case 7:
                        if(data == "<"){
                            ft[fatherPos]->left = cursor;
                        }else if(data == ">"){
                            ft[fatherPos]->right = cursor;
                        }else if(data == "="){
                            ft[fatherPos]->middle = cursor;
                        }
                        break;
                    default:
                        std::cout << "Erro na leitura dos dados da arvore" << std::endl;
                        return nullptr;
                }

            }
            cursor = new Node;
            a++;
        }
    }else{
        std::cout << "erro ao abrir o arquivo de arvore" << std::endl;
        return nullptr;
    }
    std::cout<< std::endl;
    treeF.close();
    return newRoot;

}

void BpTree::recWrite(Node *cursor,std::ostream &file,char direction,int fatherPos){
    std::string isLeaf;
    int lptr;
    int rptr;
    if (cursor != nullptr) {
        int pos = (int)file.tellp();
        isLeaf = cursor->isLeaf?"true":"false";
        lptr = cursor->isLeaf?ptr[cursor->leftId]:0;
        rptr = cursor->isLeaf?ptr[cursor->rightId]:0;
        file << isLeaf << '|' <<cursor->leftId << '|' << cursor->rightId << '|' << cursor->size << '|' << fatherPos << '|'<<  lptr << '|'<< rptr << '|' << direction << std::endl;
        if(!cursor->isLeaf) {
            recWrite(cursor->left,file,'<',pos);
            recWrite(cursor->middle,file,'=',pos);
            recWrite(cursor->right,file,'>',pos);
        }
    }

}
void BpTree::writeTree() {
    std::ofstream treeF(treeFile);
    if(treeF.is_open()){
        recWrite(root, treeF , '.', 0);
        treeF.close();
    }else{
        std::cout << "erro ao abrir o arquivo de arvore" << std::endl;
    }
}

BpTree::BpTree(std::string nameFile, std::string treeFile, std::string emptyFile, int regSize,bool reset) : nameFile(std::move(nameFile)), treeFile(std::move(treeFile)), emptyFile(std::move(emptyFile)),  regSize(regSize){
    if(reset){
        //caso o argumento reset seja passado como true a arvore é reiniciada e os arquivos são limpos
        std::ofstream clearTree(this->treeFile);
        if(clearTree.is_open()){
            std::cout<< "A arvore foi resetada" << std::endl;
            clearTree.close();
        }else{
            std::cout<< "Erro ao abrir o arquivo da arvore" << std::endl;
        }
        std::ofstream clearNames(this->nameFile);
        if(clearNames.is_open()){
            std::cout<< "Os nomes foram resetados" << std::endl;
            clearNames.close();
        }else{
            std::cout<< "Erro ao abrir o arquivo de nomes" << std::endl;
        }
        std::ofstream clearEmpty(this->emptyFile);
        if(clearEmpty.is_open()){
            clearEmpty << 0 << std::endl;
            std::cout<< "Os registros vazios foram resetados" << std::endl;

            clearEmpty.close();
        }else{
            std::cout<< "Erro ao abrir o arquivo de registros vazios" << std::endl;
        }
        root = new Node;
    }else{
        //se não a arvore é carregada para memória principal
        this->root = readTree();
        loadEmptyRegister();
    }
}

int BpTree::getID() const {
    Node *cursor = root;
    while(!cursor->isLeaf){
        cursor = cursor->right;
    }
    return cursor->rightId +10;
}

int BpTree::checkEmptyRegister() {
    if(!emptyReg.empty()){
        std::ofstream emptyF(emptyFile);
        if(emptyF.is_open()) {
            emptyF << emptyReg.size() - 1 << std::endl;
            for (int i = 1; i < emptyReg.size(); i++) {
                emptyF << emptyReg[i] << std::endl;
            }
            emptyF.close();
            int regAddres = emptyReg[0];
            emptyReg.erase(emptyReg.begin());
            return regAddres;
        }else{
            std::cout << "Erro ao abrir arquivo de registros" << std::endl;
            return -1;
        }
    }else{
        return -1;
    }
}

void BpTree::loadEmptyRegister() {
    std::ifstream emptyF(emptyFile);
    std::string line;
    if(emptyF.is_open()) {
        getline(emptyF,line);
        int size = std::stoi(line);
        for (int i = 0; i< size; i++){
            getline(emptyF,line);
            emptyReg.push_back(std::stoi(line));
        }
        emptyF.close();
    }
}
void BpTree::writeEmptyRegister() {
    std::ofstream emptyF(emptyFile);
    if(emptyF.is_open()) {
        emptyF << emptyReg.size() << std::endl;
        for (int i : emptyReg) {
            emptyF << i << std::endl;
        }
        emptyF.close();
    }
}