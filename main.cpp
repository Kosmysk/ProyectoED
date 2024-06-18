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

// Funcion para leer el archivo .bin que se genera con saveBinaryFile
string leerArchivoBin(const string &filename){
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
void saveBinaryFile(const string &bitString, const string binaryfile){
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

// Funcion para guardar los archivos, para hacer pruebas de codificacion/compresion y decodificacion/decompresion por separado
void saveFile(ofstream  &output, string &codified){
    output << codified;
}

// Funcion para guardar los resultados, no sé que más agregarle, solo muestra el tiempo del 1 al 20 como una columna, si alguno tiene alguna idea modifiquelo pls sdjds.
void logResults(ofstream &output, double time){
    output << time << endl;
}

// Funcion que mide el tiempo que toma en codificar y decodificar 20 veces
void TimeExperiment(const string &filename, const string &resultadosCodificacion, const string &resultadosCompresion){
    HuffmanCoding huffman;
    ofstream file(resultadosCodificacion);
    ofstream file2(resultadosCompresion);
    string original = leerArchivo(filename);

    for (int i = 0; i < 20; ++i){
        
        auto start = chrono::high_resolution_clock::now();
        string codified = huffman.encode(original);
        string decodified = huffman.decode(codified);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> execution_time = end - start;
        logResults(file, execution_time.count());

        start = chrono::high_resolution_clock::now();
        vector<pair<int, int>> comprimido = compresionLZ(original);
        string descomprimido = descompresionLZ(comprimido);
        end = chrono::high_resolution_clock::now();
        execution_time = end - start;
        logResults(file2, execution_time.count());  

        
    }
    file.close();
    file2.close();
}

int main(){
    HuffmanCoding huffman;

    string dataprueba = "prueba.txt";
    string textobin = "textoprueba.bin";
    //TimeExperiment(dataprueba, "resultadoshuffman.csv", "resultadosLZ.csv");

    string original = leerArchivo(dataprueba);
    
    string codified = huffman.encode(original);
    string bitstring = leerArchivoBin(textobin);

    string decodificado = huffman.decode(bitstring);
    cout << decodificado;

    // Para probar si el texto original es igual al decodificado, considerando que la decodificacion se hizo desde el archivo .bin
    if(original == decodificado){
        cout << "TA BUENO" << endl;
    }
    else{
        cout << "TA MALO" << endl; 
    }


}
