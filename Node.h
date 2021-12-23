//
// Created by leomo on 23/12/2021.
//

#ifndef BMAISARVORE_NODE_H
#define BMAISARVORE_NODE_H


class Node {
public:
        bool isLeaf;
        Node* left;
        int leftId;
        Node* middle;
        int rightId;
        Node* right;
        int size;
};


#endif //BMAISARVORE_NODE_H
