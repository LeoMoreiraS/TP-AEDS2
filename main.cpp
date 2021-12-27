#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <cstring>
#include "Node.h"
#include "BpTree.h"
#include <cstdio>

using namespace std;
#define REGISTER_SIZE 15

//arquivo para armazenar os indices da arvore
string TREE_FILE = R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\tree.txt)";
//arquivo para armazenar os nomes
string NAME_FILE = R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\names.bin)";

// para o trabalho resolvi utilizar uma arvore m-way de tamanho 3
// para implementação da arvore b+ onde cada no tem 3 filhos e 2 indices


void menu(){
    cout << "Inserir um novo registro (1)" << endl;
    cout << " Encontrar um registro (2)" << endl;
    cout << "  Remover um registro (3)" << endl;
    cout << "   Imprimir A arvore (4)" << endl;
    cout << "         Sair (0)       " << endl;
}
int main() {
    int op,address,x,id;
    char name[REGISTER_SIZE];
    cout << "Manter arquivos (0)" << endl << "Reiniciar arquivos (1)" << endl;
    cin >> op;
    //reiniciar e limpar os arquivos se não carrega os dados da memória
    bool reset = op == 1;
    BpTree tree = BpTree(NAME_FILE,TREE_FILE,REGISTER_SIZE,reset);
    id = tree.getID();
    cout << endl;

    system("pause");
    cout << string(50, '\n');
    while(true){
         menu();
         cin >> op;
        switch (op) {
            case 0:
                return 0;
            case 1:
                cout << "Digite um nome" << endl;
                cin.clear();
                fflush(stdin);
                cin.get(name,REGISTER_SIZE,'\n');
                tree.insert(id,name);
                tree.print(tree.root);
                cout << endl <<"Registro adicionado : ID(" << id << ") Nome: " << name;
                id+=10;
                break;
            case 2:
                cout << "Digite um id para buscar um registro" << endl;
                cin >> x;
                address = tree.search(x);
                if(address != -1){
                    cout << "Endereco do registro : " << address << endl;
                }
                break;
            case 3:
                cout << "Digite um id para remover um registro" << endl;
                cin >> x;
                tree.remove(x);
                tree.writeTree(TREE_FILE);
                break;
            case 4:
                cout << "root ";
                tree.print(tree.root);
                break;
            default:
                cout << "Opção invalida" << endl;
        }
        cout << endl;

        system("pause");
        cout << string(50, '\n');
    }
}