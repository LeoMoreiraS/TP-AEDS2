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
    //tamanho da word
    int regSize;
    //raiz da árvore
    Node *root;
    //paths dos arquivos
    std::string nameFile;
    std::string emptyFile;
    std::string treeFile;
    //estruturas auxiliares para guardar ponteiros e registros vazios
    std::map <int,int> ptr;
    std::vector<int> emptyReg;
    //funções para atualizar os arquivos da arvore
    Node *readTree();
    void writeTree();
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
    //função pra pegar o maior id da árvore
    int getID() const;

public:
    //construtor
    BpTree(std::string nameFile, std::string treeFile,std::string emptyFile, int regSize,bool reset);
    //impressão da arvore
    void print();
    //busca de um nó/registro
    int search(int index);
    //remoção e inserção de nós
    void remove(int x);
    void insert(int x,char *name);
};


#endif //BMAISARVORE_BPTREE_H
