#include <iostream>
#include <list>

#include "Node.h"
#include "BpTree.h"

using namespace std;

// para o trabalho resolvi utilizar uma arvore m-way de tamanho 3
// para implementação da arvore b+ onde cada no tem 3 filhos e 2 indices

int main() {

    BpTree tree = BpTree();
    //Node* root =tree.readTree(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\tree.txt)");
    //tree.root = root;

    for (int i = 10; i <= 100; i+=10) {
        tree.insert(i);
        tree.print(tree.root);
        cout<<endl;
    }
    tree.writeTree(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\test.txt)");
    tree.remove(30);
    tree.print(tree.root);
    cout<<endl;

    return 0;
}