#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <cstring>
#include "Node.h"
#include "BpTree.h"
#include <stdio.h>

using namespace std;

// para o trabalho resolvi utilizar uma arvore m-way de tamanho 3
// para implementação da arvore b+ onde cada no tem 3 filhos e 2 indices

int main() {

    BpTree tree = BpTree();
    Node* root =tree.readTree(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\tree.txt)");
    tree.root = root;
    ofstream namesW(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\names.bin)", ios::out | ios::binary);

    char name[15];
    int a = 0;


//    for (int i = 10; i <= 50; i+=10) {
//        cout << "Digite um nome" << endl;
//        cin.clear();
//        fflush(stdin);
//        cin.get(name,15,'\n');
//        namesW.seekp(0,ios::end);
//        a = (int)namesW.tellp();
//        namesW.write(name, 15*sizeof(char));
//        tree.insert(i,a);
//        tree.writeTree(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\test.txt)");
//        tree.print(tree.root);
//        cout<<endl;
//    }

    namesW.close();
    tree.remove(30);
    int pos = tree.search(50);
    ifstream namesR(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\names.bin)", ios::in | ios::binary);
    namesR.seekg(pos,ios::beg);
    namesR.read(name,15*sizeof(char));
    cout << name << endl;
    tree.writeTree(R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\test.txt)");

    tree.print(tree.root);
    cout<<endl;


    return 0;
}