#include "utils.h"
#include "readFile.h"
#include <iostream>
#include <cstring>


int main(){
    string text_file = "data/playfair.txt";
    string bigram_file = "data/bigram_english_raw.txt";
    string solution_file = "playfair_solution.txt";
    cout << "Load code file\n";

    Text original_text = readFile(text_file.c_str());
    cout << "Load frequency file\n";
    Frequencies original_freq = *loadFrequencies(bigram_file.c_str());    //array of 625 bigram frequencies

    cout << "Generate key\n";
    //generate 5 different keys
    Keysquare best = {'P','A','L','M','E','R','S','T','O','N','B','C','D','F','G','H','I','K','Q','U','V','W','X','Y','Z'};
    generateKey(best);
    scoreKeysquare(best, original_text, original_freq);

    double diff;
    double T0 = 0.05;
    double T = T0;   //temperature
    int improvements;
    double r;       //holds random number
    Keysquare current;
    int nr_rounds = 0;
    int max_rounds = 10000;
    cout << "Decypher Key\n";
    while (nr_rounds < max_rounds) {
        ++nr_rounds;
        improvements = 0;
        current = best;
        //find good neighbour:
        for (int i = 0; i < 5000; ++i) {
            r = rand() % 101;
            if (r < 20)
                swapLetters(current);
            else if (r < 40)
                swapRandomCols(current);
            else if (r < 60)
                swapRandomRows(current);
            else if (r < 70)
                flipHorizontal(current);
            else if (r < 80)
                flipVertical(current);
            else if (r < 90)
                reverseRow(current);
            else if (r < 100)
                reverseCol(current);
            else
            {
                //crazy key
                for (int i = 0; i < 5; ++i)
                    swapLetters(current);
            }

            scoreKeysquare(current, original_text, original_freq);

            diff = current.score - best.score;
            if (diff > 0)
            {
                //score improved
                improvements++;
            }

            //use as new key:
            if (diff > 0)
            {
                //better key: accept current as best
                best = current;
            }
            else if (accept(diff, T))
            {
                //key is worse but accept anyway
                best = current;
            }
            else
            {
                current = best;
            }

            if (improvements > 100) {
                //improved enough
                //lower temperature
                break;
            }
        }
        //lower temperature
        T *= 0.9;
        if (improvements == 0) {
            //no more improvements found
            break;
        }
    }
    cout << "Key found: \n";
    printKeysquare(best);
    decodeText(best, original_text);
    cout << "Write solution to file\n";
    writeFile(solution_file.c_str(), original_text, best);
    delete[] original_text;
    cout << "Done\n";
    return 0;
}