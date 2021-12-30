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
void BpTree::print(){
    Node *cursor = root;

    if (cursor != nullptr) {
        std::cout << "root ";
        if(!cursor->isLeaf) {
            if(cursor->leftId != 0 || cursor->rightId != 0){
                std::cout << cursor->leftId << " " << cursor->rightId << " " << std::endl;
            }
            std::cout << " ( < ";
            printRecursive(cursor->left);
            std::cout << " = ";
            printRecursive(cursor->middle);
            std::cout << "  > ";
            printRecursive(cursor->right);
            std::cout << ") " << std::endl;
        }else{
            int lptr = cursor->leftId != 0?ptr[cursor->leftId]: 0;
            int rptr = cursor->rightId != 0?ptr[cursor->rightId]: 0;
            if(cursor->leftId != 0 || cursor->rightId != 0){
                std::cout  << " {key: " << cursor->leftId << " ptr: " << lptr  << "} " << "{key: " << cursor->rightId << " ptr: " << rptr  << "}->";
            }
        }
    }else{
        std::cout << "Árvore vazia" << std::endl;
    }
}
void BpTree::printRecursive(Node *cursor) {
    if (cursor != nullptr) {
        if(!cursor->isLeaf) {
            if(cursor->leftId != 0 || cursor->rightId != 0){
                std::cout << cursor->leftId << " " << cursor->rightId << " " << std::endl;
            }
            std::cout << " ( < ";
            printRecursive(cursor->left);
            std::cout << " = ";
            printRecursive(cursor->middle);
            std::cout << "  > ";
            printRecursive(cursor->right);
            std::cout << ") " ;
        }else{
            int lptr = cursor->leftId != 0?ptr[cursor->leftId]: 0;
            int rptr = cursor->rightId != 0?ptr[cursor->rightId]: 0;
            if(cursor->leftId != 0 || cursor->rightId != 0){
                std::cout  << " {key: " << cursor->leftId << " ptr: " << lptr  << "} " << "{key: " << cursor->rightId << " ptr: " << rptr  << "}->";
            }
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
                //sobe o nó da direita e aloca um novo filho pra ele
                //        4
                //    /       \
                //  1  2 ->  4  0
                newNode->leftId = x;
                newNode->left = cursor;
                split->leftId = x;
                split->size = 1;
                newNode->middle = split;
                newNode->right = new Node;
            }else if(x < cursor->leftId){
                //meio == cursor->leftId
                newNode->leftId = cursor->leftId;
                newNode->middle = cursor;
                split->leftId = x;
                split->size = 1;
                newNode->left = split;
                newNode->right = new Node;
            }else{
                //meio == x
                newNode->leftId = x;
                split->leftId = cursor->leftId;
                split->size = 1;
                cursor->leftId = x;
                newNode->left = split;
                newNode->middle = cursor;
                newNode->right = new Node;
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
    //se não tiver mais q dois filhos empurra o novo nó pra direita e sobe o id dele
    if (cursor->size < 2) {
        if(x > cursor->leftId){
            cursor->rightId = x;
            cursor->size++;
            cursor->right = child->middle;
        }else if(x < cursor->left->leftId){
            cursor->rightId = cursor->leftId;
            cursor->leftId = x;
            cursor->right = cursor->middle;
            cursor->middle = cursor->left;
            cursor->left = child->left;
        }
    }else {//Caso o parente não tenha espaço divide os nós em 2 galhos
        Node *newNode = new Node;
        newNode->isLeaf = false;
        newNode->size = 1;
        Node *splitL = new Node;
        Node *splitR = new Node;
        splitL->size=1;
        splitR->size=1;
        splitR->isLeaf = false;
        splitL->isLeaf = false;
        if(x < cursor->left->leftId ){//caso realocar nó para esquerda
            //nó da esquerda
            splitL->leftId = cursor->left->leftId ;
            splitL->left = child->left;
            splitL->middle = cursor->left;
            splitL->right = new Node;
            //nó da direita
            splitR->leftId = cursor->rightId;
            splitR->left = cursor->middle;
            splitR->middle = cursor->right;
            splitR->right = new Node;
            //nó central
            newNode->leftId = cursor->leftId;
            newNode->left = splitL;
            newNode->middle = splitR;
            newNode->right = new Node;
        }else if((x > cursor->right->rightId && cursor->right->rightId != 0)||(x > cursor->right->leftId&& cursor->right->rightId != 0) ){//caso realocar nó pra esquerda
            //nó da esquerda
            splitL->leftId = cursor->leftId;
            splitL->left = cursor->left;
            splitL->middle = cursor->middle;
            splitL->right = new Node;
            //nó da direita
            splitR->leftId = x;
            splitR->left = cursor->right;
            splitR->middle = child->middle;
            splitR->right = new Node;
            //nó central
            newNode->leftId = cursor->rightId;
            newNode->left = splitL;
            newNode->middle = splitR;
            newNode->right = new Node;
        }else{
            //nó da direita
            splitR->leftId = x;
            splitR->left = cursor->right;
            splitR->middle = child->middle;
            splitR->right = new Node;
            //nó da esquerda
            splitL->leftId = cursor->leftId;
            splitL->left = cursor->left;
            splitL->middle = cursor->middle;
            splitL->right = new Node;
            //nó central
            newNode->leftId = cursor->rightId;
            newNode->left = splitL;
            newNode->middle = splitR;
            newNode->right = new Node;
        }


        if (cursor == root) {
            root = newNode;
            return;
        } else {
            insertInternal(newNode->leftId, findParent(root,cursor), newNode);
            return;
        }
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
    //verifica se o filho bate com algum dos filhos do cursor se sim retorna o cursor
    if(cursor->left == child || cursor->right == child || cursor->middle == child){
        parent = cursor;
        return parent;
    }else{//faz uma chamada recursiva com todos os filhos do cursor
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
        int pos=0;
        while (!cursor->isLeaf) {
            parent = cursor;
            if(x >= cursor->rightId && cursor->rightId!=0){
                pos=2;
                cursor = cursor->right;
            }else if(x < cursor->leftId ){
                pos=0;
                cursor = cursor->left;
            }else{
                pos=1;
                cursor = cursor->middle;
            }
        }
        if(x == cursor->leftId){
            cursor->leftId = cursor->rightId;
            cursor->rightId = 0;
        }else if(x == cursor->rightId){
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
            writeTree();
            return;
        }

        if (cursor->size != 0) {
            writeTree();
            return;
        }
        std::cout << "MERGE" <<std::endl;
        if(pos == 0){


            if(parent->size == 2 ){//puxa o filho da direita pro meio e o meio pra esquerda
                parent->left = parent->middle;
                parent->middle = parent->right;
                parent->leftId = parent->rightId;
                parent->right = cursor;
                parent->rightId = 0;
                parent->size--;

            }else if (parent == root){//se a raiz for o nó de cima apaga os filhos e puxa o filho remanescente pra raiz
                root = parent -> middle;
                delete parent->left;
                delete parent->right;
                delete parent;
            }else{//galho morreu sobe a folha
                removeInternal(parent->leftId, findParent(root,parent) , parent->middle);
            }
        }else if(pos == 1){
            //substitui o filho da esquerda do parente pelo da direita e puxa a direita pro meio
            if(parent->size == 2){
                parent->middle = parent->right;
                parent->right = cursor;
                parent-> leftId = parent->rightId;
                parent->rightId = 0;
                parent->size--;

            }else if(parent == root){//se a raiz for o nó de cima apaga os filhos e puxa o filho remanescente pra raiz
                root = parent->left;
                delete parent->middle;
                delete parent->right;
                delete parent;

            }else{// o galho morreu puxa a folha pra cima
                removeInternal(parent->leftId, findParent(root,parent) , parent->left);
            }
        }else{
            //remove o filho da direita e deixa o nó da direita vazio como a arvóre está pendendo pra
            //esquerda só remove o nó da direita porque a arvore é obrigada a ter um nó da esquerda
            parent->rightId = 0;
            parent->size--;
        }

        writeTree();
    }
}




void BpTree::removeInternal(int x, Node *cursor, Node *child) {

    if(cursor->size == 1){//apenas existe um id a esquerda
        if(x>=cursor->leftId){//verifica se o filho é o da esquerda ou meio que precisa ser realocado
            if(cursor->left->size == 1){//caso tenha apenas um nó desce o cursor para direita do filho da esquerda
                cursor->left->rightId = cursor->leftId;
                cursor->left->right = child;
                cursor->left->size++;
                *(cursor) = *(cursor-> left);
            }else{// divide o galho de baixo em 2 novos galhos
                cursor->middle->leftId = cursor->leftId;
                cursor->leftId = cursor->left->rightId;
                cursor->middle->left = cursor->left->right;
                cursor->left->right = new Node;
                cursor->middle->size = 1;
                cursor->left->size = 1;
                cursor->middle->middle = child;
            }
        }else{//esquerda vazia meio subir
            if(cursor->middle->size == 1){//caso tenha apenas um nó desce o cursor para esquerda do filho da direita
                cursor->middle->rightId = cursor->middle->leftId;
                cursor->middle->right = cursor->middle->middle;
                cursor->middle->middle = cursor->middle->left;
                cursor->middle->left = child;
                cursor->middle->leftId = cursor->leftId;
                cursor->middle->size++;
                *(cursor) = *(cursor-> middle);
            }else{// divide o galho de baixo em 2 novos galhos
                cursor->left->middle = cursor->middle->left;
                cursor->left->leftId = cursor->leftId;
                cursor->left->left = child;
                cursor->middle->left = cursor->middle->middle;
                cursor->middle->middle = cursor->middle->right;
                cursor->leftId = cursor->middle->leftId;
                cursor->middle->leftId =  cursor->middle->rightId;
                cursor->middle->rightId = 0;
                cursor->middle->right = new Node;
                cursor->middle->size = 1;
                cursor->left->size = 1;
            }
        }
    }else{
        removeInternal(x, findParent(root,cursor) , cursor);
    }
    if(cursor == root){//se a raiz for o cursor a arvore foi rebalanceada
        return;
    }else{// chama a função novamente passando o cursor como filho e pegando o pai dele como cursor
        removeInternal(x, findParent(root,cursor) , cursor);
    }
}


Node *BpTree::readTree() {
    std::string line;
    std::string data;
    Node *newRoot = new Node;
    Node *cursor = newRoot;
    std::map<int,Node*> ft;
    int fatherPos = 0;
    int a=0;
    char delimiter = '|';
    std::ifstream treeF(treeFile,std::ios::in);
    if(treeF.is_open()){
        while(!treeF.eof()){


            getline(treeF,line);
            int pos = treeF.tellg();
            a++;
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
                            ft[pos-10+a]=cursor;
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

        isLeaf = cursor->isLeaf?"true":"false";
        lptr = cursor->isLeaf?ptr[cursor->leftId]:0;
        rptr = cursor->isLeaf?ptr[cursor->rightId]:0;
        file << isLeaf << '|' <<cursor->leftId << '|' << cursor->rightId << '|' << cursor->size << '|' << fatherPos << '|'<<  lptr << '|'<< rptr << '|' << direction << std::endl;
        int pos = (int)file.tellp();
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