#include "HashTable.h"
#include <algorithm> // função transform que deixa tudo minusculo
#include <iostream>
#include <math.h>

/* Entrada constructor
* no inputs
* initializes Entrada with occupied as false
*/
Entrada::Entrada(){
	occupied = false;
}


/* HashTable constructor
*  input s is the size of the array
*  set s to be size
*  initialize array of vectors Entrada
*/
HashTable::HashTable (int s) 
{
	Entrada entry;
	size=s;
	entry.occupied = false;
	for (int i=0; i<s; i++)
	{
		tabela.push_back(entry); // coloca s entradas na tabela hash
	}
}


/* computeHash
*  return an integer based on the input string
*  used for index into the array in hash table
*  be sure to use the size of the array to 
*  ensure array index doesn't go out of bounds
*
* Método de Horner: https://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
*/
int HashTable::computeHash(string s) {
	int horner = 19;
	int sum=0;
	vector <int> aux2;
	if(s.size() > aux.size()){
		for(int i=0; i<s.size();i++)
			aux2.push_back(pow(horner,i));
		aux=aux2;

	}
	for(int i=0; i<s.size();i++){
  		sum+= s[i]*aux[i]; // método de horner 
  	}
  	return abs((int)sum%size);
}



/* put
*  input: string word and int score to be inserted
*  First, look to see if word already exists in hash table
*   if so, addNewAppearence with the score to the WordEntry
*   if not, create a new Entry and push it on the list at the
*   appropriate array index
*/
void HashTable::put(string s, int score, int commentnum, int wordindex) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	
	int index = computeHash(s);
	bool p = contains(s); // flag que diz se a tabela ja contem a string
	if(!stopword(s)){
	if(!p){ // se não contem a palavra
		if(!tabela[index].occupied) // se não tá ocupada aquela entrada
		{
			tabela[index].word = s;
			tabela[index].occupied = true;
			tabela[index].scores.push_back(score);
			tabela[index].Cindex.push_back(commentnum);
			tabela[index].Windex.push_back(wordindex);
		}
		else // aquela entrada já esta ocupada (poem na lista encadeada)
		{
			Entrada nova;
			nova.word = s;
			nova.occupied = true;
			nova.scores.push_back(score);
			nova.Cindex.push_back(commentnum);
			nova.Windex.push_back(wordindex);
			tabela[index].prox.push_back(nova);
		}
	}
	else // a palavra ja tá na lista 
	{
		if(tabela[index].word == s) // no indice certo (não na lista encadeada)
		{
			tabela[index].scores.push_back(score);
			tabela[index].Cindex.push_back(commentnum);
			tabela[index].Windex.push_back(wordindex);
		}
		// ela ta no vetor de proximas
		else
		{
			for(int i=0;i<tabela[index].prox.size();i++)
			{
				if(tabela[index].prox[i].word == s)
				{
					tabela[index].prox[i].scores.push_back(score);
					tabela[index].prox[i].Cindex.push_back(commentnum);
					tabela[index].prox[i].Windex.push_back(wordindex);
					break;
				}
			}
		}
	}
 
}
}
/* getAverage
*  input: string word 
*  output: the result of a call to getAverage()
*          from the WordEntry
*  Must first find the WordEntry in the hash table
*  then return the average
*  If not found, return the value 2.0 (neutral result)
*/

double HashTable::getAverage(string s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	
	bool p= contains(s); // ve se a tabela tem a string
	double soma=0;
	double occ=0;
	double retorno;
	int index = computeHash(s);

	if (!p) // se deu falso, retorna 2
		retorno = 2;
	else
	{
		if(tabela[index].word == s) // no indice certo (não na lista encadeada)
		{
			for(int i=0; i<tabela[index].scores.size();i++)
			{

				soma+=tabela[index].scores[i];
				occ++;
			}
		}
		// ela ta no vetor de proximas
		else
		{
			for(int i=0;i<tabela[index].prox.size();i++)
			{
				if(tabela[index].prox[i].word == s)
				{
					
					for(int j=0; j<tabela[index].prox[i].scores.size();j++)
					{
						soma+=tabela[index].prox[i].scores[j];
						occ++;
					}	
				}
			}
		}
		retorno = soma/occ;

	}
	//cout << " palavra : " << s << " media : " << retorno << endl << endl;
	if(s == "love" or s=="loved" or s=="lovely" or s=="liked" or s == "good" or s=="nice")
		retorno = retorno+0.5;
	if(s == "bad" or s=="lousy" or s=="lame" or s=="horrible" or s == "awful" or s=="dead")
		retorno = retorno-0.5;
	return retorno;
}

/* Occurence
* input: string word
* output: number of occurrences of this word in the hash table
*/
int HashTable::occurrence(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	
	int index = computeHash(s);
	int oc=0;
	
	if(!contains(s)){ // ela nao ta na tabela = 0 ocorrencias
		return 0;
	}
	else
	{
		if(tabela[index].word == s) // no indice certo (não na lista encadeada)
		{
			for(int i=0; i<tabela[index].scores.size();i++) // pra cada score que tem aumenta uma ocorrencia
			{
				oc++;
			}
		}
		// ela ta no vetor de proximas
		else
		{
			for(int i=0;i<tabela[index].prox.size();i++) // por todo vetor de proximas
			{
				if(tabela[index].prox[i].word == s) // se ela ta la
				{
					for(int j=0; j<tabela[index].prox[i].scores.size();j++)
						oc++;
				}
			}

		}
	}
	return oc;
}

/* contains
* input: string word
* output: true if word is in the hash table
*         false if word is not in the hash table
*/
bool HashTable::contains(string s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	
	int index = computeHash(s);
	if(!tabela[index].occupied)
		return false;
	else{
		if (tabela[index].word == s)
			return true;
		else{
			for(unsigned int i=0; i < tabela[index].prox.size(); i++){
				if(tabela[index].prox[i].word == s){
					return true;
			}
		}
	}
	return false;
 }
}

bool HashTable::stopword(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	
	char stopwords[][30] = {"santa","one", "two", "three", "clause", "about", "consider", "`", "n't", "that","for", "film", "but","with", "in", "i","single", "ever", "seen", "so", "its", "think", "minute", "don't", "really", "lot", "do", "have" ,"had","as", "movie", "every", "script", "ends", "little", "many", "much", "often" , "rarely", "always", "never","...","..",".",",",":",";","'","1","2","3","4","5","6","7","8","9","~", "^", "[","]","(",")","*", "at", "the", "on", "of", "an", "and", "whose", "even", "then", "because", "why", "where", "what", "by", "a", "all", "you", "your", "it", "this", "their", "them", "he", "she", "us", "yours", "is", "was", "to", "END"};
	bool retorno = false;
	
	for(int i=0; i <(sizeof(stopwords)/sizeof(*stopwords));i++)
	{
		if(s.size() == 1)
			return true;
		if(stopwords[i] == s){
			retorno = true;
			break;
		}
	}
	return retorno;


}

void HashTable::ListaIndices(string s, int floor, int roof)
{
	int index = computeHash(s);

	if(!contains(s)){ // ela nao ta na tabela = 0 ocorrencias
		cout<<"A palavra "<<s<<" nao esta na tabela hash :("<<endl;
	}
	else
	{
		if(tabela[index].word == s) // no indice certo (não na lista encadeada)
		{
			for(int i=0; i<tabela[index].scores.size();i++) // pra cada score que tem aumenta uma ocorrencia
			{
				if(tabela[index].scores[i] >= floor and tabela[index].scores[i] <= roof)
					cout<<"Comentario numero: "<<tabela[index].Cindex[i]<<"    Palavra numero: "<< tabela[index].Windex[i]<< "    Score associado: " << tabela[index].scores[i]<<endl;
			}
		}
		// ela ta no vetor de proximas
		else
		{
			for(int i=0;i<tabela[index].prox.size();i++) // por todo vetor de proximas
			{
				if(tabela[index].prox[i].word == s) // se ela ta la
				{
					for(int j=0; j<tabela[index].prox[i].scores.size();j++)
						if(tabela[index].prox[i].scores[j] >= floor and tabela[index].prox[i].scores[j] <= roof)
							cout<<"Comentario numero: "<<tabela[index].prox[i].Cindex[j]<<"    Palavra numero: "<< tabela[index].prox[i].Windex[j]<< "    Score associado: " << tabela[index].prox[i].scores[j]<<endl;
				}
			}

		}
	}
	
}

void HashTable::swap(Quick* a, Quick* b)
{
    Quick t = *a;
    *a = *b;
    *b = t;
}
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int HashTable::partition (Quick arr[], int low, int high)
{
    int pivot = arr[high].chave;    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j].chave <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void HashTable::quickSort(Quick arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void HashTable::kmais(int k, bool kmais)
{
	vector<Quick> aux;

	for(int i=0;i<tabela.size();i++)
	{
		if(tabela[i].occupied)
		{
			Quick novo;
			novo.chave = occurrence(tabela[i].word);
			novo.palavra = tabela[i].word;
			aux.push_back(novo);
			for(int j=0; j<tabela[i].prox.size();j++){
				Quick filho;
				filho.chave = occurrence(tabela[i].prox[j].word);
				filho.palavra = tabela[i].prox[j].word;
				aux.push_back(filho);

			}
		}
	}
	int n=aux.size()-1;
	Quick* a = &aux[0];
	quickSort(a,0, n);
	if(kmais)
	{
		cout<< "Listando as "<<k<<" palavras com maior ocorrencia: "<<endl;
	
		for(int i=0;i<k;i++)
		{
			cout<<"Ocorrencias: " << a[aux.size()-i-1].chave <<" Palavra: "<< a[aux.size()-i-1].palavra<<endl;
		
		}
	}
	else
	{
		cout<< "Listando as "<<k<<" palavras com menor ocorrencia: "<<endl;
	
		for(int i=0;i<k;i++)
		{
			cout<<"Ocorrencias: " << a[i].chave <<" Palavra: "<< a[i].palavra<<endl;
		
		}

	}
}

int HashTable::conta()
{
	int o=0;

	for (int i=0;i<tabela.size();i++){
		if(tabela[i].occupied){
			o++;
		}
	}
	cout<<"posicoes ocupadas na tabela: "<< o<<endl;
	
	int maior;
	int aux=0;

	for (int i=0;i<tabela.size();i++){
		if(tabela[i].occupied){
			for(int j=0;j<tabela[i].prox.size();j++)
				aux++;
			if(aux>maior){
				maior=aux;
				aux=0;
			}
		}
	}

	cout<<"mais vizinhos: "<<maior;
	return 0;
}
