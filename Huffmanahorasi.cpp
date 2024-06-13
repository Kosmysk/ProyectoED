#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

// Estructura para representar un nodo en el árbol Huffman
struct Node {
    char data;  // Carácter representado por el nodo
    unsigned freq;  // Frecuencia del carácter
    Node* left;  // Hijo izquierdo del nodo en el árbol (tomas ijo mio)
    Node* right;  // Hijo derecho del nodo en el árbol

    Node(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
    ~Node() {
        delete left;
        delete right;
    }
};

// Estructura de comparación para el heap de mínimos
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// Clase principal para la codificación y decodificación
class HuffmanCoding {
private:
    priority_queue<Node*, vector<Node*>, Compare> minHeap;  // Heap de mínimos para el árbol Huffman
    unordered_map<char, string> codes;  // Diccionario para almacenar codigos huffman
    unordered_map<string, char> reverseMapping;  // Diccionario inverso para decodificación

    // Método para construir el árbol Huffman a partir de las frecuencias
    Node* buildTree(const unordered_map<char, unsigned>& freqMap) {
        for (const auto& pair : freqMap) {
            minHeap.push(new Node(pair.first, pair.second));
        }

        while (minHeap.size() != 1) {
            Node* left = minHeap.top();
            minHeap.pop();

            Node* right = minHeap.top();
            minHeap.pop();

            Node* merged = new Node('$', left->freq + right->freq);
            merged->left = left;
            merged->right = right;

            minHeap.push(merged);
        }

        return minHeap.top();
    }

    // Método auxiliar para construir códigos Huffman recursivamente
    void buildCodesHelper(Node* root, const string& code) {
        if (root == nullptr) {
            return;
        }

        if (root->data != '$') {
            codes[root->data] = code;
            reverseMapping[code] = root->data;
        }

        buildCodesHelper(root->left, code + "0");
        buildCodesHelper(root->right, code + "1");
    }

public:
    // Método para codificar 
    string encode(const string& text) {
        unordered_map<char, unsigned> freqMap;
        for (char c : text) {
            freqMap[c]++;
        }

        Node* root = buildTree(freqMap);
        buildCodesHelper(root, "");

        stringstream encoded;
        for (char c : text) {
            encoded << codes[c];
        }

        return encoded.str();
    }

    // Método para decodificar 
    string decode(const string& encodedText) {
        stringstream decoded;
        string code;
        for (char bit : encodedText) {
            code += bit;
            if (reverseMapping.find(code) != reverseMapping.end()) {
                decoded << reverseMapping[code];
                code = "";
            }
        }

        return decoded.str();
    }
};

int main() {
    HuffmanCoding huffman;
    string texto = "╠Ã┐╠Ã┐╠Ã Ã┐╠Ã┐";
    string textoCodificado = huffman.encode(texto);
    cout << "Texto codificado: " << textoCodificado << endl;
    string textoDecodificado = huffman.decode(textoCodificado);
    cout << "Texto decodificado: " << textoDecodificado << endl;

    return 0;
}

//#saqué algunas weas de https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// https://docs.python.org/3/library/heapq.html
//https://www.programiz.com/dsa/huffman-coding