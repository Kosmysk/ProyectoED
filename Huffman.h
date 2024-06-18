#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>
#include <memory>

using namespace std;

class HuffmanCoding {
private: // Estructura de Nodo con uso de shared_ptr para gestión automática de memoria
    struct Node {
        char data; // Carácter representado por el nodo
        unsigned freq; // Frecuencia del carácter
        shared_ptr<Node> left; // Hijo izquierdo del nodo en el árbol
        shared_ptr<Node> right; // Hijo derecho del nodo en el árbol

        Node(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
        ~Node() {} // Destructor para liberar nodos recursivamente
    };
    // Estructura de comparación para el heap de mínimos 
    struct Compare {
        bool operator()(const shared_ptr<Node>& left, const shared_ptr<Node>& right) {
            return left->freq > right->freq;
        }
    };

    shared_ptr<Node> root; // Raíz del árbol Huffman

    // Método privado para construir el árbol Huffman
    void buildTree(const unordered_map<char, unsigned>& freqMap) {
        priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, Compare> minHeap; // Heap de mínimos para el árbol Huffman
        for (const auto& pair : freqMap) {
            minHeap.push(make_shared<Node>(pair.first, pair.second));
        }

        while (minHeap.size() != 1) {
            auto left = minHeap.top();
            minHeap.pop();

            auto right = minHeap.top();
            minHeap.pop();

            auto merged = make_shared<Node>('$', left->freq + right->freq);
            merged->left = left;
            merged->right = right;

            minHeap.push(merged);
        }

        root = minHeap.top();
        minHeap.pop();
    }

    // Método privado para construir códigos Huffman recursivamente
    void buildCodesHelper(shared_ptr<Node> node, const string& code, unordered_map<char, string>& codes) {
        if (node == nullptr) {
            return;
        }

        if (node->data != '$') {
            codes[node->data] = code;
        }

        buildCodesHelper(node->left, code + "0", codes);
        buildCodesHelper(node->right, code + "1", codes);
    }

public:
    // Constructor
    HuffmanCoding() : root(nullptr) {}

    // Metodo público para CODIFICAR texto
    string encode(const string& text) {
        // Calcular frecuencias de los caracteres
        unordered_map<char, unsigned> freqMap;
        for (char c : text) {
            freqMap[c]++;
        }

        // Construir el arbol Huffman
        buildTree(freqMap);

        // Crear mapa de códigos Huffman
        unordered_map<char, string> codes;
        string code;
        buildCodesHelper(root, code, codes);

        // Codificar el texto
        string encoded;
        for (char c : text) {
            encoded += codes[c];
        }

        return encoded;
    }

    // Metodo publico para DECODIFICAR texto
    string decode(const string& encodedText) {
        string decoded;
        shared_ptr<Node> current = root;

        for (char bit : encodedText) {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }

            if (current->left == nullptr && current->right == nullptr) {
                decoded += current->data;
                current = root;
            }
        }

        return decoded;
    }

    // Funcion para leer el archivo .bin que se genera con saveBinaryFile
    string loadEncoded(const string &filename){
    ifstream file(filename, ios::binary); // se abre el archivo con ifstream en modo binario
    // En esta parte se declara el vector de lo bytes del archivo y se guardan estos bytes en el vector
    vector<unsigned char> byteVector((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); //estos istreambuf_iterator sirven para iterar sobre el archivo, el primero
    // itera sobre todos los char en file, que en este caso son los bytes del archivo, el que está vacio es para finalizar esta especie de range 

    string bitString; //se declara un bitString que contiene los bits del archivo pero como un string
    for (unsigned char byte : byteVector){ // El ciclo for trabaja sobre cada byte, que convenientemente es un unsigned char ya que solo almacena 1 byte, en byteVector
        bitString += bitset<8>(byte).to_string(); //usando bitset podemos representar de maneria binaria este byte en 8 bits, y con to_string queda como string.
    }

    return bitString; //Devuelve este string que contiene los bits
    }

    // Funcion para transformas los bits de string que devuelve nuestro huffman a un archivo .bin como corresponde
    void saveEncoded(const string &bitString, const string binaryfile){
        // abrimos el archivo donde vamos a escribir el string como bin, esto se hace con std::ios::binary, asi nos aseguramos que el resultado sea binario si o si
        ofstream file(binaryfile, ios::binary);

        vector<unsigned char> byteVector; //Como no podemos escribir bit por bit, hacemos un vector de bytes, que guarda los bits del bitstring
        int bitcount = 0; //Se utiliza bitcount para contar los bits que entran al vector
        unsigned char byte = 0; // Se inicializa un byte para acumular los bits con unsigned char, lo que es perfecto porque solo almacena 8 bits

        for(char bit : bitString){ // En este for se itera sobre cada bit en el bitstring
            if (bit == '1'){ // Sabemos que en un byte hay 8 bits, y que estos van de 0 a 7, en esta parte movemos el bit de valor 1 hacia la izquierda, es decir 10000000
                byte |= (1<<(7 - bitcount)); //  Ahora que tenemos el bit como 10000000 con el operador OR este se añade al byte y nos quedaría el byte como 10000000
            }     
            bitcount ++; //Suma 1 a bitcount, si el bit es 0 suma uno directamente, es decir que si el string de bits es '101' el byte nos quedaria como 10100000, ya que para 
            // el tercer bit, el bit '1' la bitcount seria 2 y esto dejaria 7-2 dejandolo en la quinta posicion

            if (bitcount == 8){ // Como los bytes solo almacenan 8 bits, cuando esta cuenta llega a 8, este byte completo se añade al byteVector
                byteVector.push_back(byte);
                bitcount = 0; // Y se reinician el bitcount y el byte para ir contando bits de nuevo e ir agregando bytes al bytevector
                byte = 0;
            }
        }

        if(bitcount > 0){ // si es que sobran bits que no completaron un byte, esto se encarga de meterlo al vector
            byteVector.push_back(byte);
        }

        // aca se escriben los resultados en el archivo, write espera un const char asi que se usa reintrepret_cast para transformar la data de byteVector en un const char
        file.write(reinterpret_cast<const char *>(byteVector.data()), byteVector.size()); //byteVector.size() es el tamaño del vector array, es decir los bytes que se deben escribir.
        file.close(); //Se cierra el archivo
    }

};


//Update: Añadí un constructor explicito en la clase HuffmanCoding para inicializar la raíz del arbol Huffman (root)
// como nullptr. Esto proporciona una inicialización limpia y coherente del objeto HuffmanCoding al crear instancias de la clase.


//  Antes había riesgos de fugas de mmoria y liberaciones incorrectas.  Cambié de punteros crudos a punteros inteligentes, usando shared_ptr 
//para los nodos del arbol Huffman. Esto mejora la gestión de la memoria y evita problemas con la liberacion incorecta de memoria ya que liberan memoria
//automaticamente cuando los objetos ya no son necesarios.

//Eliminación del Destructor Personalizado: Ya no necesitamos un destructor personalizado para los nodos, gracias al uso de shared_ptr,
// que se encarga automáticamente de liberar la memoria.

//Mejora en la Construcción del Árbol Huffman: Ajusté los métodos buildTree y buildcodeshelper para trabajar con shared_ptr, lo cual 
//hace que la construcción del árbol sea másb segura.

//en conclusión: te queremos shared_ptr <3 (esto de los punteros inteligentes los saqué de estas paginas // https://www.geeksforgeeks.org/shared_ptr-in-cpp/
// https://learn.microsoft.com/es-es/cpp/cpp/how-to-create-and-use-shared-ptr-instances?view=msvc-170  y https://cplusplus.com/reference/memory/shared_ptr/)
