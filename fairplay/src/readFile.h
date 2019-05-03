//
// Created by fj on 11.03.19.
//

#ifndef FAIRPLAY_READFILE_H
#define FAIRPLAY_READFILE_H

#include <fstream>
#include <vector>
#include "utils.h"
#include "exceptions.h"

using namespace std;

Text readFile(const char* filename);

//returns 625 long array with frequencies of every bigram
//J is converted to I
//doubled letters are added half and half to LX and XL (with L the doubled letter)
//doubled letters are left 0
//position of frequency for AB is convertLetter(A)*25+convertLetter(B)
Frequencies* loadFrequencies(const char* filename);

void writeFile(const char* filename, Text solution, Keysquare& key);

#endif //FAIRPLAY_READFILE_H
