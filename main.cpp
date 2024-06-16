#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include "LZ.h"
#include "Huffman.h"

using namespace std;

// Funcion para leer un archivo
string leerArchivo(const string& filename){
    ifstream file(filename); //abre el archivo

    stringstream temp; //aca crea un espacio temporal donde se guarda el texto del archivo en temp
    temp << file.rdbuf();
    return temp.str(); // asi que lo que se obtiene seria algo asi como: linea 1\n linea 2\n ... y asi, de esa manera se guardan los saltos de linea tambien.
} 

// Funcion para guardar los resultados, no sé que más agregarle, solo muestra el tiempo del 1 al 20 como una columna, si alguno tiene alguna idea modifiquelo pls sdjds.
void logResults(ofstream &output, double time){
    output << time << endl;
}

// Funcion para guardar los archivos, para hacer pruebas de codificacion/compresion y decodificacion/decompresion por separado
void saveFile(ofstream  &output, string &codified){
    output << codified;
}

// Funcion que mide el tiempo que toma en codificar y decodificar 20 veces
void CodeAndDecodeExperiment(const string &filename, const string &resultados){
    HuffmanCoding huffman;
    ofstream file(resultados);
    for (int i = 0; i < 20; ++i){
        auto start = chrono::high_resolution_clock::now();
        string original = leerArchivo(filename);
        string codified = huffman.encode(original);
        string decodified = huffman.decode(codified);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> execution_time = end - start;
        logResults(file, execution_time.count());
    }
}

// Funcion que mide el tiempo solo de codificar, esto es para ver si es mas rapido codificando que decodificando.
void CodeExperiment(const string &filename, const string &resultados){
    HuffmanCoding huffman;
    ofstream file(resultados);
    for (int i = 0; i < 20; ++i){
        auto start = chrono::high_resolution_clock::now();
        string original = leerArchivo(filename);
        string codified = huffman.encode(original);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> execution_time = end - start;
        logResults(file, execution_time.count());
    }
}

int main(){
    HuffmanCoding huffman;

    string dataprueba = "prueba.txt";
    string textocod = "textocod.txt";
    string textodecod = "textodecod2.txt";
    //CodeAndDecodeExperiment(dataprueba, "resultados10245KB.csv");

    string original = leerArchivo(dataprueba);
    string codificadotxt = leerArchivo(textocod);

    string decodificado = huffman.decode(codificadotxt);
    
    ofstream file(textodecod);
    saveFile(file, decodificado);

    return 0;


}
