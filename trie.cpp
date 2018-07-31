#include "trie.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Função construtora da raiz (palavras não começam com 0, então usa ele como "nulo")
Trie::Trie(void){
	nodo = 0;
	final = false;
}
/* add
* input: string w
*  poem a string na arvore procurando o nodo mais proximo
*  colocando o resto da palavra como filhos desse nodo
*/
void Trie::add(const string& w){
	Trie* current = this; // current = raiz da arvore
	for(int i = 0; i < w.size(); i++){ // por todo o tamanho da palavra
		bool achou = false; 
		int j = 0;
		while(!achou && j < current->filho.size()){		//Procura o nodo que tem como caractere o próximo caractere da palavra
			if(current->filho[j].nodo == w[i]){ // se o filho j é aquele da palavra
				achou = true;						//Encontrou, ele agora é o current
				current = &(current->filho[j]);	
			} 
			else{
				j++; // se não passa a analisar o proximo filho
			}
		}
		if(!achou){						//passou por todos os filhos e não achou, cria o nodo
			Trie newNode;
			newNode.nodo = w[i]; // nodo é o char 
			newNode.prefixo = current->prefixo; // prefixo é o que vem antes do current
			newNode.prefixo += w[i]; // mais o char em questao
			newNode.final = false; // não é final pq tem mais palavra pra analisar
			
			current->filho.push_back(newNode); // ele é filho do current (compartilham o prefixo só o filho tendo um a mais)
			current = &(current->filho[current->filho.size()-1]); // agora o current é o que acabou de criar 
		}
	}
	current->final = true;	//no final da palavra current se tornou a ultima letra (e palavra)
	current = NULL; // desaloca a memoria do ponteiro
	delete current; // deleta o ponteiro
}

//Dado um prefixo, insere em words todas as palavras que tem este prefixo
void Trie::palavrascom(const string& Procurado, vector<string>& palavras){
	if(Procurado == prefixo){ // se o que eu to procurando é igual o prefixo da arvore "atual"
		listapalavras(palavras); // só listar as palavras a partir dele
	} else{
		for(int i = 0; i < filho.size(); i ++) // se nao, chama recursao para cada filho
			filho[i].palavrascom(Procurado, palavras);
	}
}

void Trie::listapalavras(vector<string> &palavras)
{
	if(this->final == true){ // se é um nodo final
		palavras.push_back(this->prefixo);
	}
	for(int i=0;i<filho.size();i++) // faz isso para todos os filhos de todos os nodos (ja sei que tem ele como prefixo)
		filho[i].listapalavras(palavras);
}
