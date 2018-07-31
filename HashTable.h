#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <list>
#include <iostream>
//#include "WordEntry.h"
#include <vector>
#include <cmath>
#include <string>


using namespace std;

class Quick {

	private:
		int chave;
		string palavra;

	friend class HashTable;
};

class Entrada{
	
	private:
		string word;
		vector<int> scores; // vetor com os scores nas diferentes aparições da palavra
		vector<int> Windex; // indices da palavra
		vector<int> Cindex; // indice do comentario
		bool occupied; // se a entrada ta ocupada ou não
		vector<Entrada> prox; // proximas entradas (lista encadeada)
	
	public:
		Entrada(void); // entrada nova (para criar tabela)
		
	friend class HashTable; // friend class = hashtable pode usar a entrada
};

class HashTable {

    private:
	vector<Entrada> tabela;
    int computeHash(string);
    int size;
    void swap(Quick*, Quick*);
	int partition (Quick[], int, int);
	void quickSort(Quick[], int, int);
	vector<int> aux;


    public:
        HashTable(int);
        bool contains(string);
        double getAverage(string);
        void put(string, int, int, int);
        int occurrence(string);
        bool stopword(string s);
		void ListaIndices(string, int, int);
		void kmais(int, bool);
		int conta();
};


#endif
