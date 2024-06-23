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
vector<pair<int, int>> compresionLZ(const string& texto) {
    TrieNode* raiz = new TrieNode();                        // Crea el nodoTrie
    vector<pair<int, int>> comprimido;                      //Vector para almacenar la compresión
    unordered_map<string, int> first;                       //unordered_map para almacenar la primera ocurrencia de cada cadena
    int pos;                                                //Variable para almacenar la posición

    for (int i = 0; i < texto.size(); ++i) {                    //recorre todo el texto
        string substring;                                        //Variable que guarda el substring actual
        int longLength = 0;                                      //Longitud de la coincidencia más larga encontrada
        int longPos = -1;                                        //Posición de la coincidencia más larga encontrada

        for (int j = i; j < texto.size(); ++j) {                //Busca todas los substrings que coinciden desde la posición i
            substring += texto[j];                                //Agrega el carácter al final del substring
            if (search_key(raiz, substring, pos)) {                //Busca el substring en el Trie
                longLength = substring.size();                    //Actualiza el valor de "longLenght" para guardar la longitud de la coincidencia más larga
                longPos = pos;                                    //Actualiza el valor de "longPos" para guardar la posición de inicio de la coincidencia más larga
            } else {
                break;
            }
        }

        if (longLength > 1) {
            comprimido.push_back(make_pair(longPos, longLength));        //Agrega la posición y longitud de la coincidencia al vector comprimido
            i += longLength - 1;                                        //Mueve i al final del substring coincidente
        } else {
            string caracter = string(1, texto[i]);                        //Obtiene el carácter actual como string
            if (first.find(caracter) != first.end()) {                    //Verifica si el carácter ya está en unordered_map de primeras ocurrencias "first"
                comprimido.push_back(make_pair(first[caracter], 1));        //Agrega la posición de la primera aparición al vector comprimido
            } else {
                comprimido.push_back(make_pair(texto[i], 0));            //Agrega el carácter al vector comprimido
                first[caracter] = i;                                    //Registra la posición de la primera ocurrencia del carácter en unordered_map
            }
        }

        substring.clear();                                                //vacía el substring para insertar todos los substrings en el Trie
        for (int j = i; j < texto.size(); ++j) {                          //Inserta todos los substrings desde la posición i en el Trie
            substring += texto[j];                                        //Agrega el carácter al final de la subcadena actual
            insert_key(raiz, substring, i);                               //Inserta el substring en el Trie
        }
    }

    return comprimido;                                                //Retorna el texto comprimido
}

// Función para descomprimir el texto usando LZ
string descompresionLZ(const vector<pair<int, int>>& comprimido) {
    string descomprimido;                //Variable para almacenar el texto descomprimido

    for (const auto& [caracter, largo] : comprimido) {                        //recorre todo el texto comprimido
        if (largo == 0) {                                                    //Si la longitud es cero, es un carácter que aparece por primera vez en el texto comprimido y tiene forma (a,0) por ejemplo
            descomprimido += char(caracter);                                //Agrega el carácter al texto descomprimido
        } else {
            int pos = caracter;                                            //asigna el valor de la posición inicial del substring en el texto descomprimido
            descomprimido += descomprimido.substr(pos, largo);            //Agrega el substring al texto descomprimido
        }
    }
    return descomprimido;                //retorna el texto descomprimido
}    

void imprimeC(vector<pair<int, int>> comprimido){                //Función que imprime el texto comprimido
    cout << "Texto comprimido: ";    
    for (const auto& p : comprimido) {                                    //Recorre el texto comprimido y va imprimiendo cada uno de los pairs
        if (p.second == 0) {
            cout << "(" << char(p.first) << ",0) ";
        } else {
            cout << "(" << p.first << "," << p.second << ") ";
        }
    }
    cout << endl;
}

// Funcion para guardar el archivo comprimido
void saveComp(const vector<pair<int, int>>& comprimido, const string& filename) {
    ofstream file(filename, ios::binary); // Abrimos el archivo donde vamos a guardar el output en modo binario
    for (const auto& p : comprimido) { // En este ciclo for se itera sobre cada elemento p (par) del vector
        // Aca debemos hacer reinterpret_cast para transformar a un punto const char el entero correspondiente del par, esto porque write espera ese puntero
        // luego se usa sizeof para saber la cantidad de bytes que tiene el par, y write escribe esta cantidad de bytes en el archivo
        file.write(reinterpret_cast<const char*>(&p.first), sizeof(p.first)); // En esta parte se escribe el primer numero del par
        file.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second)); // En esta parte se escribe el segundo numero del par
    }
    file.close(); // Se cierra el archivo
}

vector<pair<int, int>> loadComp(const string& filename) {
    ifstream file(filename, ios::binary); // Abrimos el archivo del que cargaremos los datos en modo binario y los convertiremos
    vector<pair<int, int>> comprimido; // Inicializamos un vector que contiene los pares del archivo comprimido
    pair<int, int> p; // Inicializamos p (par) que contiene los pares (a;b)
    // Hacemos un ciclo while para trabajar con todos los datos del archivo
    while (file.read(reinterpret_cast<char*>(&p.first), sizeof(p.first))) { //in.read necesita un puntero a char para funcionar, por eso se hace el reinterpret_cast
        file.read(reinterpret_cast<char*>(&p.second), sizeof(p.second));// con &p almacenamos los bytes leidos del archivo en p.first y con sizeof obtenemos el numero de bytes que deben leerse del archivo
        comprimido.push_back(p); // En esta parte añadimos los pares leidos del archivo al vector, p.first representa el primer numero del par y p.second el segundo
    }
    file.close(); // cerramos el archivo
    return comprimido; //devuelve los 
}

//links referenciales de donde me ayudé con los strings y el uso de la estructura Trie
// https://cplusplus.com/reference/string/string/
// https://www.geeksforgeeks.org/trie-insert-and-search/
// https://www.geeksforgeeks.org/introduction-to-trie-data-structure-and-algorithm-tutorials/
