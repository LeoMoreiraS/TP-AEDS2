//
// Created by leomo on 23/12/2021.
//

#ifndef BMAISARVORE_BPTREE_H
#define BMAISARVORE_BPTREE_H


#include "Node.h"
#include <map>
#include <vector>
class BpTree {


    void insertInternal(int x, Node *cursor, Node *child);
    std::map <int,int> ptr;
    std::vector<int> emptyReg;
public:
    BpTree(std::string nameFile, std::string treeFile,std::string emptyFile, int regSize,bool reset);

private:
    int regSize;
    std::string nameFile;
    std::string emptyFile;
    std::string treeFile;
public:
    int getID() const;
    int search(int index);
    Node *root{};
    void insert(int x,char *name);
    void print(Node *cursor);
    int checkEmptyRegister();
    Node *findParent(Node *cursor, Node *child);
    Node *readTree();
    void remove(int x);

    void recWrite(Node *cursor,std::ostream &file,char direction,int fatherPos);

    void writeTree();

    void loadEmptyRegister();

    void writeEmptyRegister();
};


#endif //BMAISARVORE_BPTREE_H
