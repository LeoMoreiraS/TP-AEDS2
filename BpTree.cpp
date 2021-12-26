//
// Created by leomo on 23/12/2021.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "BpTree.h"
#include "Node.h"


void insert(int x);

int BpTree::search(int index) {
    if (root == nullptr) {
        std::cout << "Tree is empty\n";
        return -1;
    } else {
        Node *cursor = root;
        while (!cursor->isLeaf) {
            if(cursor->leftId >= index){
                cursor = cursor->left;
            }else if(cursor->rightId <= index){
                cursor = cursor->right;
            }else{
                cursor = cursor->middle;
            }
        }
        if(cursor->rightId == index){
            return index;
        }else if(cursor->leftId == index ){
            return index;
        }else{
            std::cout << "Not found\n";
            return -1;
        }
    }
}
void BpTree::print(Node *cursor) {
    if (cursor != nullptr) {
        std::cout << cursor->leftId << " " << cursor->rightId << " ";
        if(!cursor->isLeaf) {
            std::cout << " ( < ";
            print(cursor->left);
            std::cout << ") (= ";
            print(cursor->middle);
            std::cout << ") ( > ";
            print(cursor->right);
            std::cout << ") " ;
        }else{
            std::cout << "|" ;
        }
    }
}

void BpTree::insert(int x) {
    if (root == nullptr) {
        root = new Node;

        root->rightId = x;
        root->size = 1;
    } else {
        Node *cursor = root;
        Node *parent;
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
        // inserção nos nós da folha
        if (cursor->size < 2) {
            if(cursor->leftId == 0 && cursor->rightId == 0 ){
                cursor->rightId = x;
            }else if(cursor->rightId < x){
                cursor->leftId = cursor->rightId;
                cursor->rightId = x;
            }else{
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
                newNode->rightId = cursor->rightId;
                newNode->middle = cursor;
                split->rightId = x;
                split->size = 1;
                newNode->right = split;
                newNode->left = new Node;
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
                insertInternal(newNode->rightId, parent, newNode);
            }
        }
    }
}

void BpTree::insertInternal(int x, Node *cursor, Node *child) {
    // If we doesn't have overflow
    if (cursor->size < 2) {

        if(cursor->leftId == 0 && cursor->rightId >x){
            cursor->leftId = x;
            cursor->size++;
            cursor->left = child;
        }else if(cursor->rightId <x){
            cursor->leftId = cursor->rightId;
            cursor->left = cursor->middle;
            cursor->middle = child->middle;
            cursor->right = child->right;
            cursor->rightId = child->rightId;
            cursor->size++;
        }

    }

        // For overflow, break the node
    else {
        Node *newNode = new Node;
        newNode->isLeaf = false;
        newNode->size = 1;
        //se o maior nó da folha for menor que o valor a ser inserido
        if (x > cursor->rightId) {
            //meio == cursor->rightId
            newNode->rightId = cursor->rightId;
            newNode->middle = cursor;
            newNode->right = child;
            newNode->left = new Node;
            cursor->right = new Node;
        } else if (x < cursor->leftId) {
            //meio == cursor->leftId
            newNode->rightId = cursor->leftId;
            newNode->middle = child;
            newNode->right = cursor;
            newNode->left = new Node;
        } else {
            //meio == x
            newNode->rightId = x;
            cursor->leftId = x;
            newNode->middle = child;
            newNode->right = cursor;
            newNode->left = new Node;
        }

        if (cursor == root) {
            root = newNode;
        } else {
            insertInternal(newNode->rightId, findParent(root,cursor), newNode);
        }
    }
}
Node* BpTree::findParent(Node* cursor,
                         Node* child)
{
    Node* parent;

    // If cursor reaches the end of Tree
    if (cursor->isLeaf){
        return nullptr;
    }

    if(cursor->left == child){
        parent = cursor->left;
        return parent;
    }else if(cursor->right == child){
        parent = cursor->right;
        return parent;
    }else if(cursor->middle == child){
        parent = cursor->middle;
        return parent;
    }else{
        parent = findParent(cursor->right,child);
        parent = findParent(cursor->middle,child);
        parent = findParent(cursor->left,child);
        return parent;
    }

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
        if (cursor == root) {

            if (cursor->size == 0) {
                std::cout << "Tree died\n";
                delete cursor;
                root = nullptr;
            }
            return;
        }
//        cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
//        cursor->ptr[cursor->size + 1] = NULL;
//        if (cursor->size >= (MAX + 1) / 2) {
//            return;
//        }
//        if (leftSibling >= 0) {
//            Node *leftNode = parent->ptr[leftSibling];
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
//        }
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

Node *BpTree::readTree(std::string file) {
    std::string line;
    std::string data;
    Node *root = new Node;
    std::vector<int>::iterator itr;
    Node *cursor = root;
    std::vector<Node*> fathers;
    std::vector<int> fatherPosition;
    int fatherPos = 0;
    int a=0;
    char delimiter = '|';
    std::ifstream treeFile;
    treeFile.open(file);
    if(treeFile.is_open()){
        while(!treeFile.eof()){
            int pos = treeFile.tellg();
            std::cout << "pos: "<<pos << std::endl;
            getline(treeFile,line);
            std::cout << "line size " << line.size()<<std::endl;
            std::cout << "line: "+line << std::endl;

            for(int i = 0;i<6;i++) {
                data = line.substr(0, line.find(delimiter));
                line = line.substr(line.find(delimiter) + 1);
                switch (i) {
                    case 0:
                        cursor->isLeaf = data == "true";
                        if(!cursor->isLeaf){
                            fathers.insert(fathers.begin()+fathers.size(),cursor);
                            fatherPosition.insert(fatherPosition.begin()+fatherPosition.size(),pos+a);
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
                        //percorre o vetor de positions procurando o index do father
                        itr = std::find(fatherPosition.begin(), fatherPosition.end(), fatherPos);
                        if (itr != fatherPosition.cend()) {
                            fatherPos = std::distance(fatherPosition.begin(), itr);
                        }
                        else {
                            std::cout << "Element not found";
                        }
                        if(data == "<"){
                            fathers[fatherPos]->left = cursor;
                        }else if(data == ">"){
                            fathers[fatherPos]->right = cursor;
                        }else if(data == "="){
                            fathers[fatherPos]->middle = cursor;
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
    treeFile.close();
    return root;

}

void BpTree::recWrite(Node *cursor,std::ostream &file,char direction,int fatherPos){
    std::string isLeaf;
    std::cout << (file.tellp()) << std::endl;
    if (cursor != nullptr) {
        int pos = file.tellp();
        isLeaf = cursor->isLeaf?"true":"false";
        file << isLeaf << '|' <<cursor->leftId << '|' << cursor->rightId << '|' << cursor->size << '|' << fatherPos << '|' << direction << std::endl;
        if(!cursor->isLeaf) {
            recWrite(cursor->left,file,'<',pos);
            recWrite(cursor->middle,file,'=',pos);
            recWrite(cursor->right,file,'>',pos);
        }
    }

}
void *BpTree::writeTree(std::string file) {
    std::ofstream treeFile(file);
    if(treeFile.is_open()){
        recWrite(root, treeFile ,'.',0);
    }else{
        std::cout << "erro ao abrir o arquivo de arvore" << std::endl;
        return nullptr;
    }

    treeFile.close();
}