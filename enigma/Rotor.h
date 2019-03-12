//
// Created by benji on 12/03/19.
//

#ifndef GROEPSWERK_ROTOR_H
#define GROEPSWERK_ROTOR_H

#include <string>

class Rotor {
    std::string m_combination;

    int m_position = 0;

public:
    Rotor(std::string combination, int startPosition = 0);

    /**
     * Adds 1 to the position, if the position overflows it returns true otherwise false
     * */
    bool updatePosition();

    char mapChar(char inp);

    char invMapChar(char inp);
};


#endif //GROEPSWERK_ROTOR_H
