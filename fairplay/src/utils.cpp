//
// Created by fj on 02.05.19.
//

#include <string>
#include <iostream>
#include <cmath>
#include <cstring>
#include <exception>
#include <limits>
#include "utils.h"

void printKeysquare(const Keysquare& key){
    for(int i = 0; i < 5; ++i){
        for(char c : key.key[i]){
            cout << c << " ";
        }
        cout << "\n";
    }
}

//converts letter to integer ([A-Z] zonder J->[0-24]
int convertLetter(char c){
    if(int(c) > int('J')) { //skip J
        return int(c) - int('B'); //(int('A') + 1) = int('B')
    }
    return int(c) - int('A');
}

char getChar(const Keysquare& key, int row, int col){
    return key.key[row%5][col%5];
}

void findChar(const Keysquare& key, char a, char b, int& rowA, int& colA, int& rowB, int& colB){
    bool found_one = false;
    for(int i = 0; i < 5; ++i){
        for(int j =0; j < 5; ++j){
            if(key.key[i][j] == a){
                rowA = i;
                colA = j;
                if(found_one)
                    return;
                found_one = true;
            }
            if(key.key[i][j] == b){
                rowB = i;
                colB = j;
                if(found_one)
                    return;
                found_one = true;
            }
        }
    }
    cerr << "Didn't find char: " << a << " or " << b << "in:\n";
    printKeysquare(key);
    throw exception();
}

void decodeChar(const Keysquare& key, char& a, char& b){
    //convert positions a and b to row and column numbers
    int rowA,colA,rowB,colB;
    findChar(key,a,b,rowA,colA,rowB,colB);

    //replace a and be with correct letters
    if(rowA == rowB){
        //replace with letter on the left
        a = getChar(key, rowA, colA+4);  //+4 in stead of -1 because -1%5 = -1 not 4
        b = getChar(key, rowB, colB+4);  //+4 in stead of -1 because -1%5 = -1 not 4
    }
    else if(colA == colB){
        //replace with letter below
        a = getChar(key, rowA+4, colA);  //+4 in stead of -1 because -1%5 = -1 not 4
        b = getChar(key, rowB+4, colB);  //+4 in stead of -1 because -1%5 = -1 not 4
    }
    else {
        a = getChar(key, rowA, colB);
        b = getChar(key, rowB, colA);
    }
}

void decodeText(const Keysquare& key, char* text){
    //create lookup for key

    long size_t = strlen(text);
    for(long i = 0; i < size_t; i = i+2){    //for every 2 letters
        decodeChar(key, text[i], text[i+1]);
    }
};

//give the decoded text and frequencies table
//calculates a score for the decoded text
//lower is better
double score(Text text, const Frequencies frequencies){
    double score = 0;

    long size_t = strlen(text);

    double sum = 0;

    //count frequencies of bigrams in text
    Frequencies textFreq;

    int posA;
    int posB;

    for(long i = 0; i < size_t-1; ++i){
        posA = convertLetter(text[i]);
        posB = convertLetter(text[i+1]);
        textFreq.table[posA*25 + posB]++;
    }

    //add squares of differences
    for(int i = 0; i < 625; ++i){
        sum += pow((textFreq.table[i] - frequencies.table[i])/(1+frequencies.table[i]),2);
//        score += textFreq.table[i] * frequencies.table[i];    //veel gewicht leggen op te hoge waardes
    }
    //take square root of sum
    score = sqrt(sum);
    return score/size_t;
}

void swapRows(Keysquare& key, int rowA, int rowB){
    char tmp;
    for(int i = 0; i < 5; ++i){
        tmp = key.key[rowA][i];
        key.key[rowA][i] = key.key[rowB][i];
        key.key[rowB][i] = tmp;
    }
}

void swapCols(Keysquare& key, int colA, int colB){
    char tmp;
    for(int i = 0; i < 5; ++i){
        tmp = key.key[i][colA];
        key.key[i][colA] = key.key[i][colB];
        key.key[i][colB] = tmp;
    }
}

void swapRandomRows(Keysquare& key){
    int rowA;
    int rowB;
    rowA = rand() % 5;
    do {
        rowB = rand() % 5;

    }while(rowA == rowB);

    char tmp;
    for(int i = 0; i < 5; ++i){
        tmp = key.key[rowA][i];
        key.key[rowA][i] = key.key[rowB][i];
        key.key[rowB][i] = tmp;
    }
}

void swapRandomCols(Keysquare& key){
    int colA;
    int colB;
    colA = rand() % 5;
    do {
        colB = rand() % 5;

    }while(colA == colB);
    char tmp;
    for(int i = 0; i < 5; ++i){
        tmp = key.key[i][colA];
        key.key[i][colA] = key.key[i][colB];
        key.key[i][colB] = tmp;
    }
}

void swapLetters(Keysquare& key) {
    int a;
    int b;
    do {
        a = rand() % 25;
        b = rand() % 25;

    }while(a == b);

    char tmp = getChar(key, a%5, int(floor(a/5)));
    key.key[a%5][int(floor(a/5))] = key.key[b%5][int(floor(b/5))];
    key.key[b%5][int(floor(b/5))] = tmp;
}

void flipHorizontal(Keysquare& key){
    swapRows(key, 0, 4);
    swapRows(key, 1, 3);
}

void flipVertical(Keysquare& key){
    swapCols(key, 0, 4);
    swapCols(key, 1, 3);
}

void reverseRow(Keysquare& key){
    int row = rand()%5; //get random row number
    char tmp = key.key[row][0];
    for(int i = 0; i < 4; ++i){
        key.key[row][i] = key.key[row][i+1];
    }
    key.key[row][4] = tmp;
}

void reverseCol(Keysquare& key){
    int col = rand()%5; //get random row number
    char tmp = key.key[0][col];
    for(int i = 0; i < 4; ++i){
        key.key[i][col] = key.key[i+1][col];
    }
    key.key[4][col] = tmp;
}

//give an array of
void createChildren(const Keysquare& key, vector<Keysquare>& c){
    int childNr = 0;
    char tmp;
    //swap letters
    for(int i = 0; i < 25; ++i){
        tmp = key.key[i%5][int(floor(i/5))];
        for(int j = i+1; j < 25; ++j){
            c[childNr] = key;
            c[childNr].key[i%5][int(floor(i/5))] = c[childNr].key[j%5][int(floor(j/5))];
            c[childNr].key[j%5][int(floor(j/5))] = tmp;
            ++childNr;  //next child
        }
    }
    //swap rows
    for(int i = 0; i < 4; ++i){
        for(int j = i+1; j < 5; ++j){
            c[childNr] = key;
            swapRows(c[childNr], i,j);
            ++childNr;
        }
    }
    //swap columns
    for(int i = 0; i < 4; ++i){
        for(int j = i+1; j < 5; ++j){
            c[childNr] = key;
            swapCols(c[childNr], i,j);
            ++childNr;
        }
    }
//    cout << childNr << "\n";
}

void scoreKeysquare(Keysquare& key, const Text original, const Frequencies frequencies){
    char decoded_text[strlen(original)];
    strcpy(decoded_text, original);
    decodeText(key, decoded_text);
    key.score = score(decoded_text, frequencies);
}

//returns true if no more improvements could be made
//replaces champion and champion_score to best found
void calculateScores(vector<Keysquare>& c, const Text original,
        Frequencies frequencies, Keysquare& champion){
    int champNr = 0;    //number of champion in c
    scoreKeysquare(c.at(0), original, frequencies);
    for(int i = 1; i < c.size(); ++i){
        scoreKeysquare(c[i], original, frequencies);
        if(c[i].score < c[champNr].score)
            champNr = i;
    }
}

void createNeighbour(Keysquare& neighbour, double T, int& improved,const Text& original_text,const Frequencies& original_frequencies){
    //create a random neighbour:
    Keysquare tmp = neighbour;
    improved = 0;
    int N;
    for(int i = 0; i < 1000; i++){
        N = rand()%100;
        //verander iets willekeurig aan de neighbour:
        if( i < 20){
            //swap rows
            swapRandomRows(tmp);
        }
        else if(i < 40){
            //swap columns
            swapRandomCols(tmp);
        }
        else if(i < 95){
            //swap 2 letter
            swapLetters(tmp);
        }
        else{
            //swap multiple letters
            int r = rand() % 4 + 2;
            for(int i = 0; i < r; ++i)
                swapLetters(tmp);
        }

        scoreKeysquare(tmp, original_text, original_frequencies);

        if(tmp.score < neighbour.score){
            improved++;
        }

        if(tmp.score > neighbour.score || accept(tmp.score-neighbour.score, T)){
            //accepted tmp als a new neightbour
            neighbour = tmp;
        }

        if(improved > 100)
            break;
    }

}

bool accept(double diff, double temperature){
    double r = (double)rand()/RAND_MAX;
    double chance = exp(diff/temperature);
    return r < chance;
}

vector<Keysquare>* generateKeys(const Keysquare& seed, int number_of_keys){
    vector<Keysquare>* keys = new vector<Keysquare>(number_of_keys, seed);
    char alphabet[25] = {'A','B','C','D','E','F','G','H','I','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    for(Keysquare& key : *keys){
        for(int i = 0; i < 20; ++i){
            swapLetters(key);
        }
    }
    return keys;
}

void subProbing(double T, Keysquare& best_key, Text& original_text, Frequencies& original_freq){
    scoreKeysquare(best_key, original_text, original_freq);
    Keysquare neighbour = best_key;
    printKeysquare(best_key);
    double diff;
    int improved;
    int nr_rounds = 0;
    int max_rounds = 4000;
    do{
        //create neightbour
        improved = 0;
        ++nr_rounds;
        T *= 0.9;

        createNeighbour(neighbour, T, improved, original_text, original_freq);


        if(improved == 0) {
            //can no longer improve key: stop
            std::cout << "can no longer improve\n";
            break;
        }

        if(accept(diff,T)){
            best_key = neighbour;
        }
        else{
            //try again
            neighbour = best_key;
        }
        if(nr_rounds%10 == 0) {
            cout << ".";
            cout.flush();
        }
        if(nr_rounds%100 == 0){
            cout << "\n";
        }
        if(nr_rounds%500 == 0) {
            printKeysquare(best_key);
        }

    }while(nr_rounds < max_rounds); //stop if maximum iterations have been met
}

void generateKey(Keysquare& key){
    for(int i = 0; i < 20; ++i){
        swapLetters(key);
    }
}