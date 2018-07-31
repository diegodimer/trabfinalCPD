#ifndef TRIE
#include <vector>
#include <string>

#define TRIE
using namespace std;
// cada nodo é uma letra e contem um identificador se é final de palavra ou não
class Trie{
	
	private:
		char nodo;
		string prefixo; // o que tem antes daquele nodo (palavra)
		bool final; // indica se é o final da palavra ou nao
		vector<Trie> filho; // vetor de filhos (vai ser no maximo 26 filhos mas n tem pq alocar 26 e nao usar)
		void listapalavras(vector<string>&);
		
	public:
		Trie(void);
		void add(const string&);
		void palavrascom(const string&, vector<string>&);
		
};

#endif