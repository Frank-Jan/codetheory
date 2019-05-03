//
// Created by fj on 11.03.19.
//

#include "readFile.h"
#include <sstream>
#include <iostream>
#include "utils.h"
#include <cmath>

using namespace std;



Text readFile(const char* filename) {
    ifstream file;
    file.open(filename, ios::ate);  //open file and set position to end of file
    if(!file.is_open()) {
        throw ExceptionFileNotFound();
    }

    long size = file.tellg();

    char* buff = new char[size];

    file.seekg(0,ios::beg); //set position to begin of file
    file.read(buff, size);  //read file

    file.close();
    return buff;
}


//returns 600 long array with frequencies of every bigram (doubled letters omitted)
//J is converted to I
//double letters are added half and half to LX and XL (with L the doubled letter)
Frequencies* loadFrequencies(const char* filename){
    ifstream file;
    file.open(filename, ios::ate);  //open file and set position to end of file
    if(!file.is_open()) {
        throw ExceptionFileNotFound();
    }

    long size = file.tellg();

    long buff[625] = {0};

    file.seekg(0,ios::beg); //set position to begin of file

    string bigram;
    string number;  //tijdelijke opslag frequency
    long sum = 0;
    long n;
    while(!file.eof()){
        getline(file,bigram,' ');
        if(file.eof()){
            break;
        }
        getline(file, number, '\n');
        sum += stol(number);
        n = stol(number);
        if(bigram[0] == 'J')
            bigram[0] = 'I';
        if(bigram[1] == 'J')
            bigram[1] = 'I';
        if(bigram[0] == bigram[1]){ //if equal: split with X
            if(bigram[0] == 'X'){
                //XX?->ignore?
                break;
            }
            buff[convertLetter(bigram[0])*25 + convertLetter('X')] += floor(n/2);
            buff[convertLetter('X')*25 + convertLetter(bigram[0])] += floor(n/2);
        }
        else{
            buff[convertLetter(bigram[0])*25 + convertLetter(bigram[1])] += n;
        }
    }

    file.close();

    double rescale = (double)1./sum;
    double tmp = 0;
    Frequencies* ret = new Frequencies;
    for(int i = 0; i < 625; ++i){
        ret->table[i] = (double)buff[i]*rescale;
//        cout << convertDigit(floor(i/25)) << convertDigit(i) << "\t|" << ret[i] << "\t" << long(buff[i]) << "\n";
        tmp += ret->table[i];
    }

    return ret;
}

void writeFile(const char* filename, Text solution, Keysquare& key){
    ofstream file;
    file.open(filename, std::ios_base::ate);

    if(!file.is_open())
        throw ExceptionFileNotFound();

    file << solution << "\n";
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 5; ++j){
            file << key.key[j][i];
        }
        file << "\n";
    }
    file.close();
}