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
            std::cout << " |( < ";
            print(cursor->left);
            std::cout << ") (= ";
            print(cursor->middle);
            std::cout << ") ( > ";
            print(cursor->right);
            std::cout << ")| " ;
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
            if(cursor->rightId < x){
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
                insertInternal(x, parent, newNode);
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
            cursor->rightId = x;
            cursor->size++;
        }

    }

        // For overflow, break the node
    else {
        Node *newNode = new Node;
        Node *split = new Node;
        newNode->isLeaf = false;
        newNode->size = 1;
        //se o maior nó da folha for menor que o valor a ser inserido
        if (x > cursor->rightId) {
            //meio == cursor->rightId
            newNode->rightId = cursor->rightId;
            newNode->middle = cursor;
            split->rightId = x;
            split->size = 1;
            newNode->right = split;
            newNode->left = new Node;
        } else if (x < cursor->leftId) {
            //meio == cursor->leftId
            newNode->rightId = cursor->leftId;
            newNode->middle = split;
            split->rightId = x;
            split->size = 1;
            newNode->right = cursor;
            newNode->left = new Node;
        } else {
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
            insertInternal(x, findParent(root,cursor), newNode);
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
