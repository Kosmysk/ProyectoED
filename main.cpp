#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <bitset>
#include "LZ.h"
#include "Huffman.h"

using namespace std;

// Funcion para leer un archivo
string leerArchivo(const string& filename) {
    ifstream file(filename);         //lee el archivo ingresado por parámetro

    // Leer todo el contenido del archivo en 'texto'
    string texto;
    char c;
    while (file.get(c)) {        //agrega carácter por carácter
        texto += c;
    }
    file.close();                //cierra el archivo
    return texto;                //retorna el texto del archivo
}

// Funcion para guardar los resultados
void logResults(ofstream &output, double total_time, double codecomp_time, double decodecomp_time){
    output << total_time << ";" << codecomp_time << ";" << decodecomp_time << endl;
}


// Funcion que mide el tiempo que toma en codificar y decodificar 20 veces
void TimeExperiment(const string &filename, const string &resultadosCodificacion, const string &resultadosCompresion){
    HuffmanCoding huffman; // Llamamos la class huffman para traer sus funciones
    ofstream file(resultadosCodificacion); //Abrimos el archivo donde guardaremos los tiempos de codificacion
    ofstream file2(resultadosCompresion); // Abrimos el archivo donde guardaremos los tiempos de compresion
    string original = leerArchivo(filename); // Abrimos el archivo que contiene los datos originales

    for (int i = 0; i < 20; ++i){ // En este ciclo for se especifica que se hace el experimento 20 veces, de 0 a 19
        
        auto start = chrono::high_resolution_clock::now(); //Tomamos el tiempo al principio del experimento
        string codified = huffman.encode(original); //codificacion
        auto tstep = chrono::high_resolution_clock::now(); //Tomamos el tiempo justo despues de la codificacion y antes de la decodificacion
        string decodified = huffman.decode(codified); // decodificacion
        auto end = chrono::high_resolution_clock::now(); // Tomamos el tiempo justo despues de decodificar
        chrono::duration<double> totaltime = end - start; // Tiempo total de codificar y decodificar
        chrono::duration<double> codetime = tstep - start; // tiempo que solo se demora en codificar
        chrono::duration<double> decodetime = end - tstep; // tiempo que se demora solo en decodificar
        logResults(file, totaltime.count(), codetime.count(), decodetime.count()); // guardamos los resultados

        start = chrono::high_resolution_clock::now(); // Exactamente lo mismo que el anterior, pero para la compresion y decompresion
        vector<pair<int, int>> comprimido = compresionLZ(original);
        tstep = chrono::high_resolution_clock::now();
        string descomprimido = descompresionLZ(comprimido);
        end = chrono::high_resolution_clock::now();
        totaltime = end - start;
        chrono::duration<double> comptime = tstep - start;
        chrono::duration<double> decomptime = end - tstep;
        logResults(file2, totaltime.count(), comptime.count(), decomptime.count());  
        
    }
    file.close(); // Cerramos ambos archivos
    file2.close();
}


int main(){
    //Cargamos los datos
    string data = leerArchivo("prueba.txt");
    
    // HUFFMAN ENCODING
    HuffmanCoding huffman;
    string codified = huffman.encode(data);
    huffman.saveEncoded(codified, "pruebacodified.bin");
    string codifiedbin = huffman.loadEncoded("pruebacodified.bin");
    string decodified = huffman.decode(codifiedbin);

    if(decodified == data){
        cout << "Decodificacion correcta." << endl;
    }
    else{
        cout << "Decodificacion incorrecta." << endl;
    }

    // LZ COMPRESSION
    vector<pair<int, int>> compressed = compresionLZ(data);
    saveComp(compressed, "pruebacompressed.bin");
    vector<pair<int, int>> compressedbin = loadComp("pruebacompressed.bin");
    string decompressed = descompresionLZ(compressedbin);

    if(decompressed == data){
        cout << "Descompresion correcta." << endl;
    }
    else{
        cout << "Descompresion incorrecta." << endl;
    }
    
    // Experimentos para medir tiempo, si solo se quiere medir el tiempo se puede borrar el resto del main.
    string txtexperimento = "prueba.txt"; // como en TimeExperiment() esta implementado el LeerArchivo() tenemos que ingresar filename como un string
    TimeExperiment(txtexperimento, "Resultados_Huffman.csv", "Resultados_LZ.csv");

    return 0;
}

// En la funcion main se prueban los metodos de los header Huffman.h y LZ.h, luego se compara el resultado final
// con el texto original para ver si funciona correctamente.
