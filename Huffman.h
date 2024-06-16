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
