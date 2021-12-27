//
// Created by leomo on 23/12/2021.
//

#ifndef BMAISARVORE_BPTREE_H
#define BMAISARVORE_BPTREE_H


#include "Node.h"
#include <map>
class BpTree {


    void insertInternal(int x, Node *cursor, Node *child);
    std::map <int,int> ptr;
public:
    BpTree(std::string nameFile, std::string treeFile, int regSize,bool reset);

private:
    int regSize;
    std::string nameFile;
    std::string treeFile;
public:
    int getID();
    int search(int index);
    Node *root{};
    void insert(int x,char *name);
    void print(Node *cursor);

    Node *findParent(Node *cursor, Node *child);
    Node *readTree(std::string file);
    void remove(int x);

    void recWrite(Node *cursor,std::ostream &file,char direction,int fatherPos);

    void *writeTree(std::string file);
};


#endif //BMAISARVORE_BPTREE_H
