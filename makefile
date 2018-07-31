############################# Makefile ##########################
main: HashTable.cpp main.cpp HashTable.h
	g++ HashTable.cpp main.cpp HashTable.h trie.h trie.cpp -o main
