#include <iostream>
#include <list>
#include "Node.h"
#include "BpTree.h"

using namespace std;

// para o trabalho resolvi utilizar uma arvore m-way de tamanho 3
// para implementação da arvore b+ onde cada no tem 3 filhos e 2 indices

int main() {

    BpTree tree = BpTree();
    tree.insert(1);
    tree.print(tree.root);
    cout << endl;
    tree.insert(2);
    cout << endl;
    tree.print(tree.root);
    cout << endl;
    tree.insert(3);
    tree.print(tree.root);
    cout << endl;
    tree.insert(4);
    tree.print(tree.root);

    cout << endl;
    tree.insert(5);
    tree.print(tree.root);
    cout << endl;
    tree.insert(6);
    tree.print(tree.root);
    cout << endl;
    tree.insert(7);
    tree.print(tree.root);
    cout << endl;
    tree.insert(8);
    tree.print(tree.root);
    cout << endl;
    tree.insert(-1);
    tree.print(tree.root);
    cout << endl;
    tree.insert(-2);
    tree.print(tree.root);
    cout << endl;



    return 0;
}