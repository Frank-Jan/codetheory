//
// Created by fj on 11.03.19.
//

#include "readFile.h"
#include <sstream>

char* readFile(const char* filename) {
    ifstream file;
    file.open(filename, ios::ate);  //open file and set position to end of file
    if(!file.is_open()) {
        throw ExceptionFileNotFound();
    }

    unsigned int size = file.tellg();

    char* buff = new char[size];

    file.seekg(0,ios::beg); //set position to begin of file
    file.read(buff, size);  //read file

    file.close();
    return buff;

}