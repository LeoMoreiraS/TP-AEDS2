//
// Created by leomo on 23/12/2021.
//

#ifndef BMAISARVORE_BPTREE_H
#define BMAISARVORE_BPTREE_H


#include "Node.h"

class BpTree {
    void insertInternal(int x, Node *cursor, Node *child);
public:
    int search(int index);
    Node *root;
    void insert(int x);
    void print(Node *cursor);

    Node *findParent(Node *cursor, Node *child);
};


#endif //BMAISARVORE_BPTREE_H
