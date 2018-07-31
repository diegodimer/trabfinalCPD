/* author: (your name here)
*  date: (today's date)
*  description:  provide a brief description of your program
*  proposed points (out of 10): a description of the number
*  of points you believe your assignment is worth
*/
#include "HashTable.h"
#include "trie.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <cstdlib>
#include <string>
using namespace std;

void AvaliaComentario(HashTable);
void AvaliaArquivo(HashTable, Trie);
double AvaliaComentario_arquivo(HashTable, string);
void AvaliaArqComent(HashTable, Trie);
bool menu(HashTable, Trie);

int main()
{
    //declare a few needed variables for inputing the data
    string line;    
    int score;
    int commentnum=1;
    int wordnum=0;

    ifstream myfile ("movieReviews.txt");
    if (myfile.fail() )
    {
        cout << "could not open file" <<endl;
        exit(0);
    }

    //create hash table
    HashTable table(10000);
    Trie trie;
    while (! myfile.eof() )
    {
        myfile >> score;  //get score
        myfile.get();    // get blank space
        getline(myfile,line);
        int len = line.size();
        while(len >0) {  //identify all individual strings
            string sub;
            len = line.find(" ");
            if (len > 0)
            {
                sub = line.substr(0,len);
                line = line.substr(len+1,line.size());
            }
            else {
                sub = line.substr(0,line.size()-1);
            }

            wordnum++; // proxima palavra
            if((int)sub.c_str()[0] > 64){ //tira sinais
                transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
                table.put(sub, score, commentnum, wordnum);
                trie.add(sub); 
            }
        }
        commentnum++; // proximo comentario
        wordnum=0;
    }
    myfile.close();
    bool continua=true;
    while(continua)
    {
        continua = menu(table, trie);
    }

    return 0;
}

bool menu(HashTable table, Trie trie)
{
    int menu=0;
    int k;
    vector<string> pal;
    string prefixo;
    string polaridade;
    bool inval = false;
    int floor=0, roof=4;
    cout<<" Escolha uma opcao: " << endl;
    cout<<"1. Escrever avaliacao" << endl;
    cout<<"2. Avaliar arquivo de reviews (saida no formato kagle)" << endl;
    cout<<"3. Avaliar arquivo de reviews (formato comentario por linha)" << endl;
    cout<<"4. Identificar k palavras com maior nivel de ocorrencias" <<endl;
    cout<<"5. Identificar k palavras com menor nivel de ocorrencias" <<endl;
    cout<<"6. Buscar indices de comentarios associados a uma palavra" <<endl;
    cout<<"7. Buscar indices de uma palavra associados a polaridade" <<endl;
    cout<<"8. Buscar palavras usando prefixo" <<endl;
    cout<<"9. Sair" <<endl;
    while(menu==0 or menu>9){
    cout<<endl<<endl<<"Digite sua opcao: ";
    cin>>menu;
}
    switch(menu){
        case 1:
            AvaliaComentario(table); // comentário no console
            break;

        case 2:
            AvaliaArquivo(table, trie); // arquivo no formato kaggle
            break;
        case 3:
        	AvaliaArqComent(table, trie);
        	break;
        case 4:
            cout<<"Digite quantas palavras quer buscar: ";
            cin>>k;
            table.kmais(k, true);
            std::getline (std::cin,prefixo);
            break;
        case 5:
            cout<<"Digite quantas palavras quer buscar: ";
            cin>>k;
            table.kmais(k, false);
            std::getline (std::cin,prefixo);
            break;

        case 6:
            std::getline (std::cin,prefixo);
            cout<<"Digite a palavra buscada: ";
            std::getline (std::cin,prefixo);
            cout<<"Listando indices de comentarios e palavras associados a palavra "<< prefixo<<":"<<endl;
            table.ListaIndices(prefixo, 0, 4);
            break;

        case 7:
            std::getline (std::cin,prefixo);
            cout<<"Digite a palavra buscada: ";
            std::getline (std::cin,prefixo);
            cout<<endl<<"Digite a polaridade (positivo, negativo ou neutro) :";
            getline(cin,polaridade);
            transform(polaridade.begin(), polaridade.end(), polaridade.begin(), ::tolower);
            
            if(polaridade == "negativo" or polaridade =="negativa"){
                floor = 0;
                roof = 1;
            }
            else if (polaridade == "neutro" or polaridade == "neutra"){
                    floor = 2;
                    roof = 2;
                    }
                    else if (polaridade == "positivo" or polaridade == "positiva"){
                        floor = 3;
                        roof = 4;
                        }
                        else{
                            cout<<"Polaridade invalida, listando todos."<<endl;
                            inval = true;
                        }
            if(!inval)
                cout<<"Listando indices de comentarios e palavras associados a palavra "<< prefixo<<" com polaridade "<< polaridade <<":"<<endl;
            table.ListaIndices(prefixo, floor, roof);
            break;

        case 8:
            std::getline (std::cin,prefixo);
            cout<<"Digite o prefixo buscado: ";
            std::getline (std::cin,prefixo);
            transform(prefixo.begin(), prefixo.end(), prefixo.begin(), ::tolower);
            trie.palavrascom(prefixo,pal);
            cout<<"Listando palavras com o prefixo "<< prefixo <<":"<<endl;
            for(int i=0;i<pal.size();i++)
            {
                cout<<pal[i]<<endl;
            }
            break;

        case 9:
        default:
            return false;
            break;
    
    }
    prefixo = "a";
    while( (prefixo != "s" and prefixo!="S") and (prefixo != "n" and prefixo!="N") ){
        cout<<endl<<"Voltar ao menu? (S/N)"<<endl;
        std::getline (std::cin,prefixo);
    }
    if(prefixo == "s" or prefixo == "S"){
            cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
            return true;
        }
    else
        return false;

}
void AvaliaComentario(HashTable table)
{
    string message = " ";
    bool bonus=false;
    char adv[][11] = {"most", "really", "best", "absolutely", "wondrously"};
    message = " ";
    
    std::getline (std::cin,message); // por algum motivo tava ficando no buffer e botando o ender como a string desejada
    message=" ";
    
    while(message.length() > 0)
    {
        cout << "enter a review -- Press return to exit: "<<endl;
       
        std::getline (std::cin,message);

        //used for calculating the average
        double sum = 0;
        double count = 0;
        double peso=0;

        int len = message.size();
        //get each individual word from the input
        while(len != std::string::npos) 
        {
            string sub;
            len = message.find(" ");
            if (len != string::npos) 
            {
                sub = message.substr(0,len);
                message = message.substr(len+1,message.size());
            }
            else 
            {
                sub = message;
            }
                for(int i=0; i <(sizeof(adv)/sizeof(*adv));i++)
                {
                    if(adv[i] == sub){
                    bonus = true;
                }
    }
                if(!table.stopword(sub)){ // se não for stopword
                    double media = table.getAverage(sub);
                    peso = table.occurrence(sub); // peso é o numero de ocorrencias
                    if(bonus){
                        if(media > 2)
                            media*=1.4; // ''bonus'' aumenta 40% ou diminui 40% da media (dependendo da media dela) com palavras quantificadoras
                        else
                            media*=0.6;
                        bonus = false;
                    }                        
                    sum += (media*peso); // media vezes o peso
                    count+=peso; // soma dos pesos 
                    //cout << " palavra: " << sub << " ocorrencias: "<< table.occurrence(sub) << " media: " << table.getAverage(sub) <<endl;
            }
        }

        if (message.size() > 0)
        {
            if(sum!= 0 and count != 0)
                cout << "The review has an average value of " << (double)sum/count<< endl << endl;
            else
                cout << "The review has an average value of 2"<<endl<<endl;

        }
    }
}

double AvaliaComentario_arquivo(HashTable table, string message)
{
    bool bonus=false;
    char adv[][10] = {"most","mostly" "lot", "really", "best"};
     
    while(message.length() > 0)
    {


        //used for calculating the average
        double sum = 0;
        double count = 0;
        double peso=0;

        int len = message.size();
        //get each individual word from the input
        while(len != std::string::npos) 
        {
            string sub;
            len = message.find(" ");
            if (len != string::npos) 
            {
                sub = message.substr(0,len);
                message = message.substr(len+1,message.size());
            }
            else 
            {
                sub = message;
            }
                for(int i=0; i <(sizeof(adv)/sizeof(*adv));i++)
                {
                    if(adv[i] == sub){
                    bonus = true;
                }
    }
                if(!table.stopword(sub)){ // se não for stopword
                    double media = table.getAverage(sub);
                    peso = table.occurrence(sub); // peso é o numero de ocorrencias
                    if(bonus){
                        if(media > 2)
                            media*=1.4; // ''bonus'' aumenta 40% ou diminui 40% da media (dependendo da media dela) com palavras quantificadoras
                        else
                            media*=0.6;
                        bonus = false;
                    }                        
                    sum += (media*peso); // media vezes o peso
                    count+=peso; // soma dos pesos 
                    //cout << " palavra: " << sub << " ocorrencias: "<< table.occurrence(sub) << " media: " << table.getAverage(sub) <<endl;
            }
        }

        if (message.size() > 0)
        {
            if(sum!= 0 and count != 0)
                return (double)sum/count;
            else
                return 2;
        }
    }
}

void AvaliaArquivo(HashTable table, Trie trie)
{ 
    string line;    
    int id;
    int sentenceid;
    double score;
    string nome;
    getline (std::cin,nome);
    cout<<"\nInsira o nome do arquivo com sufixo: ";
    getline (std::cin,nome);
    int ant=0;
    ofstream output;
    output.open ("output.csv");
    output<<"PhraseId,Sentiment"<<endl;

    ifstream myfile (nome.c_str());
    if (myfile.fail() )
    {
        cout << "could not open file" <<endl;
        exit(0);
    }
    getline(myfile, line); // ignore the first line
    
    while (! myfile.eof() )
    {
        myfile >> id;  //get score
        myfile.get();    // get blank space
        myfile.get();

        myfile >> sentenceid;
        
        getline(myfile,line);
        
        score = AvaliaComentario_arquivo(table, line);

        //cout<<id<<endl;
        if(id!=ant) // tava colocando 2 vezes o mesmo id
        {
            output<<id<<","<<round(score)<<endl;
        };
        ant = id;
        
    }

    myfile.close();
    output.close();
    cout<<"Fim da avaliação de arquivo.";
}
void AvaliaArqComent(HashTable table, Trie trie)
{
	 string line;    
    double score;
    string nome;

    getline (std::cin,nome);
    cout<<"\nInsira o nome do arquivo com sufixo: ";
    getline (std::cin,nome);
    ofstream output;
    output.open ("SAIDA.txt");

    ifstream myfile (nome.c_str());
    if (myfile.fail() )
    {
        cout << "could not open file" <<endl;
        exit(0);
    }
    
    do
    {
        getline(myfile,line);      
        score = AvaliaComentario_arquivo(table, line);
        output<<line<<" score: "<<score<<endl;
    }while(! myfile.eof() );

    myfile.close();
    output.close();
    cout<<"Arquivo SAIDA.txt gerado com sucesso!"<<endl;

}