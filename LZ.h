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
vector<pair<string, int>> compresionLZ(const string& texto) {
    vector<pair<string, int>> comprimido;
    int pos = 0;

    while (pos < texto.size()) {                 // recorre el texto de entrada          // y esto no hace que sea O(n^3)????? por complejidad combinada, digo yo owo
        auto [findPos, findLargo] = findStrings(texto, pos);        //busca substrings repetidos y los reemplaza con pairs (pos,largo)

        if (findLargo >= 1) {
            comprimido.push_back({to_string(findPos), findLargo});              // Almacena la posición y el largo del substring en formato de string
            pos += findLargo;
        } else {
            comprimido.push_back({string(1, texto[pos]), 0});               // Si no encuentra substrings repetidos, lo agrega como uno nuevo con el 0 en el segundo elemento del pair
            ++pos;
        }
    } 

    return comprimido;
}

// Función para descomprimir el texto usando LZ
string descompresionLZ(const vector<pair<string, int>>& comprimido) {
    string descomprimido;

    for (const auto& [caracter, largo] : comprimido) {
        if (largo == 0) {           // Si el segundo valor (el largo) es cero, añade el caracter directamente a "descomprimido"
            descomprimido += caracter;
        } else {
            int pos = stoi(caracter);                      // convierte un string a un int
            descomprimido += descomprimido.substr(pos, largo);     // Añade el substring desde la posición y largo especificado al string "descomprimido"
        }
    }

    return descomprimido;
}

// voy a ver si puedo modificar eso de las complejidades en otro archivo, att: Fernando  


//links referenciales de donde me ayudé con los strings
// https://cplusplus.com/reference/string/string/
// 
