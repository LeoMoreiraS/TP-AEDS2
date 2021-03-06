#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include "Node.h"
#include "BpTree.h"
#include <cstdio>

using namespace std;
#define REGISTER_SIZE 16

//arquivo para armazenar os indices da arvore
string TREE_FILE = R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\tree.txt)";
//arquivo para armazenar os nomes
string NAME_FILE = R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\names.bin)";
//arquivo para armazenar os espaços de registros vazios
string EMPT_FILE = R"(C:\Users\leomo\Desktop\Trabalho01\bmaisarvore\removedPosition.txt)";
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
    //reiniciar e limpar os arquivos se não carrega os dados da memória secundaria
    bool reset = op == 1;
    BpTree tree = BpTree(NAME_FILE,TREE_FILE,EMPT_FILE,REGISTER_SIZE,reset);
    cout << endl;

    system("pause");
    cout << string(50, '\n');
    while(true){
         menu();
         cin >> op;
        switch (op) {
            case 0:
                cout << "fim do programa" <<endl;
                return 0;

            case 1:
                cout << "Digite um nome" << endl;
                cin.clear();
                fflush(stdin);
                cin.get(name,REGISTER_SIZE,'\n');
                cout << "Digite um id" << endl;
                cin>>id;
                tree.insert(id,name);
                tree.print();
                cout << endl <<"Registro adicionado : ID(" << id << ") Nome: " << name;
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
                break;

            case 4:
                tree.print();
                break;

            default:
                cout << "Opção invalida" << endl;
                break;

        }
        cout << endl;
        system("pause");
        cout << string(50, '\n');
    }
}