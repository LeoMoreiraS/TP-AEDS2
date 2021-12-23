//
// Created by leomo on 23/12/2021.
//

#include <iostream>
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
            std::cout << "( < ";
            print(cursor->left);
            std::cout << ") (- ";
            print(cursor->middle);
            std::cout << ") ( > ";
            print(cursor->right);
            std::cout << ")"<< std::endl;
        }
    }
}

void BpTree::insert(int x) {
    if (root == nullptr) {
        root = new Node;
        root->isLeaf = true;
        root->rightId = x;
        root->size = 1;
    } else {
        Node *cursor = root;
        Node *parent;
        while (!cursor->isLeaf) {
            parent = cursor;
            if(cursor->leftId > x){
                cursor = cursor->left;
            }else if(cursor->rightId < x){
                cursor = cursor->right;
            }else{
                cursor = cursor->middle;
            }
        }
        if (cursor->size < 2) {
            if(cursor->leftId == 0 && cursor->rightId == 0 ){
                cursor = cursor->left;
            }else if(cursor->rightId < x){
                cursor = cursor->right;
            }
            cursor->leftId = x;
            cursor->size++;
        }
//        else {
//            Node *newLeaf = new Node;
//            int virtualNode[MAX + 1];
//            for (int i = 0; i < MAX; i++) {
//                virtualNode[i] = cursor->key[i];
//            }
//            int i = 0, j;
//            while (x > virtualNode[i] && i < MAX)
//                i++;
//            for (int j = MAX + 1; j > i; j--) {
//                virtualNode[j] = virtualNode[j - 1];
//            }
//            virtualNode[i] = x;
//            newLeaf->IS_LEAF = true;
//            cursor->size = (MAX + 1) / 2;
//            newLeaf->size = MAX + 1 - (MAX + 1) / 2;
//            cursor->ptr[cursor->size] = newLeaf;
//            newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
//            cursor->ptr[MAX] = NULL;
//            for (i = 0; i < cursor->size; i++) {
//                cursor->key[i] = virtualNode[i];
//            }
//            for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
//                newLeaf->key[i] = virtualNode[j];
//            }
//            if (cursor == root) {
//                Node *newRoot = new Node;
//                newRoot->key[0] = newLeaf->key[0];
//                newRoot->ptr[0] = cursor;
//                newRoot->ptr[1] = newLeaf;
//                newRoot->IS_LEAF = false;
//                newRoot->size = 1;
//                root = newRoot;
//            } else {
//                insertInternal(newLeaf->key[0], parent, newLeaf);
//            }
//        }
    }
}