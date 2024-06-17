#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

using namespace std;
struct TrieNode {                    //estructura de datos para manejar los caracteres del texto
    TrieNode* childNode[256];        //matriz de punteros para nodos secundarios de cada nodo
    bool wordEnd;                    //variable que se utiliza para indicar el final de un string
    int posN;                        //posicion actual

    TrieNode() {                                //constructor del nodo Trie
        wordEnd = false;
        posN = -1;
        for (int i = 0; i < 256; i++) {
            childNode[i] = nullptr;
        }
    }
};

//función que se encarga de agregar los caracteres "key" como un nuevo nodo
void insert_key(TrieNode* raiz, const string& key, int pos) {
    TrieNode* nodoActual = raiz;                //Inicializa el puntero "nodoActual"  con el nodo "raiz"

    for (auto c : key) {                        //Itera a lo largo del string
        if (nodoActual->childNode[static_cast<unsigned char>(c)] == nullptr) {            //Comprueba si el nodo existe en Trie para el carácter que se le pasó por parámetro "key" que ahora es "c"
            TrieNode* newNode = new TrieNode();                                            //Si no existe, lo crea
            nodoActual->childNode[static_cast<unsigned char>(c)] = newNode;                //Y mantiene la referencia para el nodo recién creado
        }
        nodoActual = nodoActual->childNode[static_cast<unsigned char>(c)];                //mueve el puntero del nodo actual al nuevo nodo creado
    }

    nodoActual->wordEnd = true;                                    //cambia el valor de "wordEnd" para el puntero "nodoActual" porque ahí terminaría el string
    nodoActual->posN = pos;                                        //actualiza la posición del "nodoActual"
}

//función de búsqueda de caracteres "key", que recorre el TrieNode 
bool search_key(TrieNode* raiz, const string& key, int& pos) {
    TrieNode* nodoActual = raiz;                //Inicializa el puntero "nodoActual"  con el nodo "raiz", tal como en insert_key

    for (auto c : key) {                        //recorre el string
        if (nodoActual->childNode[static_cast<unsigned char>(c)] == nullptr) {    //Comprueba si el nodo existe en el Trie para el caracter que se está revisando
            return false;            //el string o char no existe en Trie
        }
        nodoActual = nodoActual->childNode[static_cast<unsigned char>(c)];        //Mueve el puntero del Nodo actual al nodo del nuevo caracter agregado
    }

    if (nodoActual->wordEnd) {                            //revisa si el nodo actual apunta al final del string
        pos = nodoActual->posN;
        return true;
    }
    return false;
}

// Función para comprimir el texto usando LZ
vector<pair<int, int>> compresionLZ(const string& text) {
    TrieNode* raiz = new TrieNode();
    vector<pair<int, int>> comprimido;
    unordered_map<string, int> first;
    int pos;

    for (int i = 0; i < text.size(); ++i) {
        string substring;
        int longLength = 0;
        int longPos = -1;

        for (int j = i; j < text.size(); ++j) {
            substring += text[j];
            if (search_key(raiz, substring, pos)) {
                longLength = substring.size();
                longPos = pos;
            } else {
                break;
            }
        }

        if (longLength > 1) {
            comprimido.push_back(make_pair(longPos, longLength));
            i += longLength - 1; // Mueve i al final de la subcadena coincidente
        } else {
            string caracter = string(1, text[i]);
            if (first.find(caracter) != first.end()) {
                comprimido.push_back(make_pair(first[caracter], 1));
            } else {
                comprimido.push_back(make_pair(text[i], 0));
                first[caracter] = i;
            }
        }

        substring.clear();
        for (int j = i; j < text.size(); ++j) {
            substring += text[j];
            insert_key(raiz, substring, i);
        }
    }

    return comprimido;
}

// Función para descomprimir el texto usando LZ
string descompresionLZ(const vector<pair<int, int>>& comprimido) {
    string descomprimido;

    for (const auto& [caracter, largo] : comprimido) {
        if (largo == 0) {
            descomprimido += char(caracter);
        } else {
            int pos = caracter;
            descomprimido += descomprimido.substr(pos, largo);
        }
    }
    cout << endl;
    return descomprimido;
}

void imprimeC(vector<pair<int, int>> comprimido){
    cout << "Texto comprimido: ";
    for (const auto& p : comprimido) {
        if (p.second == 0) {
            cout << "(" << char(p.first) << ",0) ";
        } else {
            cout << "(" << p.first << "," << p.second << ") ";
        }
    }
    cout << endl;
}

//links referenciales de donde me ayudé con los strings
// https://cplusplus.com/reference/string/string/
// https://www.geeksforgeeks.org/trie-insert-and-search/
// https://www.geeksforgeeks.org/introduction-to-trie-data-structure-and-algorithm-tutorials/
