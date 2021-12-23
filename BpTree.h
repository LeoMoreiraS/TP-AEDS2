//
// Created by leomo on 23/12/2021.
//

#ifndef BMAISARVORE_BPTREE_H
#define BMAISARVORE_BPTREE_H


#include "Node.h"

class BpTree {
public:
    int search(int index);
    Node *root;
    void insert(int x);
    void print(Node *cursor);
};


#endif //BMAISARVORE_BPTREE_H
