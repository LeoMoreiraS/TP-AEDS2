//
// Created by leomo on 23/12/2021.
//

#ifndef BMAISARVORE_BPTREE_H
#define BMAISARVORE_BPTREE_H


#include "Node.h"
#include <map>
#include <vector>
class BpTree {
private:
    int regSize;
    Node *root{};
    std::string nameFile;
    std::string emptyFile;
    std::string treeFile;
    std::map <int,int> ptr;
    std::vector<int> emptyReg;


    Node *readTree();
    void recWrite(Node *cursor,std::ostream &file,char direction,int fatherPos);
    //funções para reutilização dos registros vazios
    int checkEmptyRegister();
    void loadEmptyRegister();
    void writeEmptyRegister();
    //funções para inserção e remoção recursiva de nós
    Node *findParent(Node *cursor, Node *child);
    void removeInternal(int x, Node *cursor, Node *child);
    void insertInternal(int x, Node *cursor, Node *child);
    //função recursiva da impressão da árvore
    void printRecursive(Node *cursor);

public:
    //construtor
    BpTree(std::string nameFile, std::string treeFile,std::string emptyFile, int regSize,bool reset);
    //busca de um nó/registro
    void print();
    int search(int index);
    void remove(int x);
    void insert(int x,char *name);


    void writeTree();

    int getID() const;
};


#endif //BMAISARVORE_BPTREE_H
