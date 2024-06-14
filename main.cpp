#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "LZ.h"
#include "Huffman.h"

using namespace std;

string leerArchivo(const string& filename){
    ifstream file(filename);

    stringstream temp;
    temp << file.rdbuf();
    return temp.str();
}


int main(){
    HuffmanCoding huffman;

    string dataprueba = "data10245KB.txt";
    string originalFile = leerArchivo(dataprueba);
    string CodifiedFile = huffman.encode(originalFile);
    string DeCodifiedFile = huffman.decode(CodifiedFile);
    

}
