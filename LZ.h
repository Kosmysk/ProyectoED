#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Función para encontrar el substring más largo que ya apareció antes, y retorna la posición de inicio de dicho substring y su largo.
pair<int, int> findStrings(const string& texto, int pos) {                  
    int maxLargo = 0;
    int maxPos = -1;

    for (int inicio = 0; inicio < pos; ++inicio) {              // recorre todas las posiciones anteriores a la posición actual
        int largo = 0;
        while (pos + largo < texto.size() && texto[inicio + largo] == texto[pos + largo]) {             // compara los caracteres en la posición 'inicio' y 'pos'
            ++largo;
        }
        if (largo > maxLargo) {             // si se encuentra un substring más largo modifica el valor del largo máximo y la posición máxima 
            maxLargo = largo;
            maxPos = inicio;
        }
    }

    return {maxPos, maxLargo};
}

// Función para comprimir el texto usando LZ
vector<pair<string, int>> compresionLZ(const string& texto) {
    vector<pair<string, int>> comprimido;
    int pos = 0;

    while (pos < texto.size()) {                 // recorre el texto de entrada
        auto [findPos, findLargo] = findStrings(texto, pos);        //busca substrings repetidos y los reemplaza con pairs (pos,largo)

        if (findLargo > 1) {
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


//links referenciales de donde me ayudé con los strings
// https://cplusplus.com/reference/string/string/
// 
