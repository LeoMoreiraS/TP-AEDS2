#include <iostream>
#include <list>
#include "Node.h"
#include "BpTree.h"

using namespace std;

// para o trabalho resolvi utilizar uma arvore m-way de tamanho 3
// para implementação da arvore b+ onde cada no tem 3 filhos e 2 indices

int main() {

    BpTree tree = BpTree();
//    BpTree Test = BpTree();
//    Test.insert(3);
//    Test.insert(2);
//    Test.print(Test.root);
    cout << endl;
    Node novoNo = Node();
    novoNo.leftId = 5;
    novoNo.rightId = 10;




    Node leftNo = Node();
    leftNo.leftId = 1;
    leftNo.rightId = 5;
    leftNo.isLeaf = true;
    novoNo.left = &leftNo;

    Node rightNo = Node();
    rightNo.isLeaf = true;
    rightNo.rightId=10;
    rightNo.leftId=8;
    novoNo.right = &rightNo;

    Node middleNo = Node();
//    middleNo.leftId = 6;

    middleNo.rightId = 7;

    middleNo.isLeaf = true;
    novoNo.middle = &middleNo;

    tree.root = &novoNo;
    tree.insert(6);
    cout << novoNo.leftId << "\n" <<(novoNo.left)->leftId << endl;
    cout << tree.search(3)<< endl;
    tree.print(tree.root);
    return 0;
}
