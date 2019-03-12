//
// Created by fj on 11.03.19.
//

#ifndef FAIRPLAY_EXCEPTIONS_H
#define FAIRPLAY_EXCEPTIONS_H
#include <exception>

struct ExceptionFileNotFound : public std::exception {
    const char * what() const throw () {
        return "File not found";
    }
};

#endif //FAIRPLAY_EXCEPTIONS_H
