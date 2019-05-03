#include "utils.h"
#include "readFile.h"
#include <iostream>
#include <cstring>


int main(){
    string filename = "data/playfair.txt";
    Text original_text = readFile(filename.c_str());
    Frequencies original_freq = *loadFrequencies("data/bigram_english_raw.txt");    //array of 625 bigram frequencies

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
    double lowest_Score = 10000000;
    while (nr_rounds < max_rounds) {
        ++nr_rounds;
        improvements = 0;
        current = best;
        //find good neighbour:
//            printKeysquare(current);
//            cout << "\tnr_rounds: " << nr_rounds << "\n";
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
            else{
//                    cout << "crazy key\n";
                for (int i = 0; i < 5; ++i)
                    swapLetters(current);
            }

            scoreKeysquare(current, original_text, original_freq);
            diff = current.score - best.score;
            if (diff > 0) {
                //score improved
                improvements++;
            }

            if (diff > 0) {
                //accept key
                best = current;
            } else if (accept(diff, T)) {
                //accept anyway
                best = current;
            } else {
//                    cerr << "Denied\n";
                current = best;
            }

            if (improvements > 100) {
                //improved enough
                //lower temperature
//                    cout << "\tLower Temp after:" << i << " " << improved <<" \n";
                break;
            }
        }
        //lower temperature
        T *= 0.9;
        if (improvements == 0) {
            //no more improvements found
            cout << "\tNo more improvements: "<< best.score << "\n";
            break;
        }
        if(nr_rounds%500 == 0){
            cout << "score:  " << best.score << "\n";
            cout << "rounds: " << nr_rounds << "\n";
        }
        //repeat search
        }
        cout << "score: " << best.score << "\n";
//        printKeysquare(best);
//    }

//    Keysquare best = keys[0];
//    for(Keysquare& key : keys){
//        if(key.score > best.score)
//            best = key;
//    }
//    cout << "best_score: " << lowest_Score << "\n";
    cout << "key: ("<< best.score << ")\n";
    printKeysquare(best);
    decodeText(best, original_text);
    cout << original_text << "\n";
    delete[] original_text;
    return 0;
}