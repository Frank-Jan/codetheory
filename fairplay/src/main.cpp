#include <iostream>
#include "readFile.h"
#include <exception>
#include <string.h>

void printText(const char* text, unsigned blocksize = 0){
    if(blocksize <= 1){
        std::cout << text << "\n";
    }
    else{
        unsigned textsize = strlen(text);
        unsigned pos = 0;
        while(pos < textsize) {
            std::cout << text[pos];
            if (pos % blocksize == blocksize -1) {
                std::cout << "\n";
            }
            pos++;
        }
    }
}

void printTabs(unsigned tabs = 0){
    for(int t = 0; t < tabs; t++){
        std::cout << "\t";
    }
}

void printHelp(unsigned tabs = 0){
    printTabs(tabs);
    std::cout << "-h or --help for help\n";
}


int main(int argc, char* argv[]) {
    //read arguments:
    if(argc == 1){
        //no arguments given
        std::cout << "No arguments given:\n";
        printHelp(1);
        return 0;
    }
    bool error = false;
    char * filename;
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-h") == 0 or strcmp(argv[i],"--help") == 0){ //asking help
            printHelp();
        }
        else if(strcmp(argv[i], "-f") == 0){                            //giving filename
            if(argc > i+1){
                filename = argv[i+1];
            }
            else{
                std::cout << "Filename missing\n";
                error = true;
            }
            i++;
        }
        else if(strcmp(argv[i], "-d") == 0){                            //giving working directory
            if(argc > i+1){

            }
        }
        else{
            std::cout << "Unknown argument: " << argv[i] << ".\n";
            printHelp(1);
        }
    }

    if(error){
        return 1;
    }

    std::cout << "Filename: " << filename << "\n";

    char * text;
    try{
        text = readFile(filename);
    }
    catch(ExceptionFileNotFound &e){
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch(exception &e){
        std::cerr << e.what() << "\n";
        try{
            delete text;
        }
        catch(...){

        }
        return 1;
    }
    printText(text, 20);


    delete text;
    return 0;
}