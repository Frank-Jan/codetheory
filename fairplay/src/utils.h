//
// Created by fj on 02.05.19.
//

#ifndef FAIRPLAY_UTILS_H
#define FAIRPLAY_UTILS_H

#include <vector>

using namespace std; //playfair key array of 25 char
typedef char* Text;                     //array of char

//struct Lookup{
//    int table[25][2]{0};     //5x5 matrix met positions [row,column]
//};

struct Frequencies{
    double table[625]{0};
};

struct Keysquare{
    char key[5][5];
    double score;
};

void printKeysquare(const Keysquare& key);

//converts letter to integer ([A-Z] zonder J->[0-24]
int convertLetter(char c);

char getChar(const Keysquare& key, int row, int col);

void decodeChar(const Keysquare& key, char& a, char& b);

void decodeText(const Keysquare& key, char* text);

void scoreKeysquare(Keysquare& key, const Text original, const Frequencies frequencies);

void swapRandomRows(Keysquare& key);

void swapRandomCols(Keysquare& key);

void swapLetters(Keysquare& key);

void flipHorizontal(Keysquare& key);

void flipVertical(Keysquare& key);

void reverseRow(Keysquare& key);

void reverseCol(Keysquare& key);

//give the decoded text and frequencies table
//calculates a score for the decoded text
//lower is better
//changes text
double score(Text text, Frequencies frequencies);

void createNeighbour(Keysquare& neighbour,double T,int& improved,const Text& original_text,const Frequencies& original_frequencies);

vector<Keysquare>* generateKeys(const Keysquare& seedKey, int number_of_keys);

void generateKey(Keysquare& key);

bool accept(double diff, double temperature);

void subProbing(double T, Keysquare& best_key, Text& original_text, Frequencies& original_freq);
#endif //FAIRPLAY_UTILS_H
