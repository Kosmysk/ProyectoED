#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include "Huffman.h"
#include "LZ.h"
using namespace std;

int main() {
    string data50MB = "english50MB.txt";
    string data100MB = "english100MB.txt";
    string data200MB = "english200MB.txt";

    ifstream file(data50MB.c_str());
    ifstream file2(data100MB.c_str());
    ifstream file3(data200MB.c_str());

    string linea;

    HuffmanCoding huffman;
  
    while(getline(file, linea)){
        auto txtComprimido = compresionLZ(linea);
        //auto txtDescomprimido = descompresionLZ(txtComprimido);
        
    }
    //auto txtComprimido = compresionLZ(data200MB);
    //auto txtDescomprimido = descompresionLZ(txtComprimido);

    return 0;
}
