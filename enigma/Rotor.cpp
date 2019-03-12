//
// Created by benji on 12/03/19.
//

#include "Rotor.h"

Rotor::Rotor(std::string combination, int startPosition) : m_combination(combination), m_position{startPosition}{

}

bool Rotor::updatePosition() {
    if (m_position == 25){
        m_position = 0;
        return true;
    }
    else{
        m_position += 1;
        return false;
    }
}

char Rotor::mapChar(char inp) {
    int index = (inp - 'A' + m_position) % 26;
    char temp =  m_combination[index];
    char result = (temp - 'A' + 26 - m_position) % 26 + 'A';
    return result;
}

char Rotor::invMapChar(char inp) {
    int index = (inp - 'A' + m_position) % 26;
    char temp = 'A' + m_combination.find(char(index+'A'));
    char result = (temp - 'A' + 26 - m_position) % 26 + 'A';
    return result;
}
