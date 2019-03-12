//
// Created by benji on 12/03/19.
//

#include <iostream>
#include "Enigma.h"

Enigma::Enigma(int rotor0, int rotor1, int rotor2, std::string startPosition) {
    int start0 = startPosition[0] - 'A';
    int start1 = startPosition[1] - 'A';
    int start2 = startPosition[2] - 'A';
    m_rotors.emplace_back(Rotor{ROTORS[rotor0], start0});
    m_rotors.emplace_back(Rotor{ROTORS[rotor1], start1});
    m_rotors.emplace_back(Rotor{ROTORS[rotor2], start2});
}

char Enigma::encrypt(char inp) {
    char temp = plug(inp);

    temp = m_rotors[0].mapChar(temp);
    temp = m_rotors[1].mapChar(temp);
    temp = m_rotors[2].mapChar(temp);

    temp = reflect(temp);

    temp = m_rotors[2].invMapChar(temp);
    temp = m_rotors[1].invMapChar(temp);
    temp = m_rotors[0].invMapChar(temp);

    return plug(temp);
}

char Enigma::reflect(char inp) {
    int index = inp - 'A';
    return reflector[index];
}

std::string Enigma::encrypt(std::string inp) {
    std::string result;
    for(char c : inp){
        result.push_back(encrypt(c));
        advancePosition(1);
    }
    return result;
}

void Enigma::setPlugBoard(std::string plug) {
    plugboard = plug;
}

char Enigma::plug(char inp) {
    int index = inp - 'A';
    return plugboard[index];
}

void Enigma::advancePosition(int i) {
    for(auto j = 0; j < i; ++j){
        if(m_rotors[0].updatePosition() && m_rotors[1].updatePosition())
            m_rotors[2].updatePosition();
    }
}

Enigma::Enigma() {

}

//int Enigma::setPosition(int pos) {
//    return 0;
//}

void solveEnigma(std::string encryptedText, std::string crib) {
    auto positions = makeGraph(encryptedText.substr(0,crib.size()), crib);
    for(int pos1 = 0; pos1 < 5; ++pos1) {
        for (int pos2 = 0; pos2 < 5; ++pos2) {
            for (int pos3 = 0; pos3 < 5; ++pos3) {
                if (pos1 == pos2 || pos1 == pos3 || pos2 == pos3)
                    continue;
                std::vector<Enigma> enigmaMachines;
                for (int i = 0; i < positions.size(); ++i) {
                    enigmaMachines.emplace_back(Enigma{pos1, pos2, pos3, "AAA"});
                    enigmaMachines.back().advancePosition(i);
                }
                Grid grid;
                grid.addEnigmas(positions, enigmaMachines);
                std::cout << "TESTING ROTORS " << pos1 << ' ' << pos2 << ' ' << pos3 << std::endl;
                for (int i = 0; i < 26 * 26 * 26; ++i) {
                    char plug1 = 'A';
                    char plug2 = 'B';
                    grid.activate(plug1, plug2);
                    if (!grid.correctPosition()) {
                        grid.reset();
                        grid.advanceEnigmas();
                    } else {
                        int p1 = i%26;
                        int p2 = (i - p1)/26 % 26;
                        int p3 = (i-26*p2-p1)/(26*26);
                        std::cout << "CORRECTE ROTOR POSITIE GEVONDEN: " << char('A'+p1) << char('A'+p2) << char('A'+p3) << std::endl;
                        if(grid.correctPlug(plug1, plug2) != plug2){
                            auto correctPlug = grid.correctPlug(plug1, plug2);
                            std::cout << "CORRECTED PLUG FROM " << plug1 << " = " << plug2 << " TO " << plug1 << " = " <<  correctPlug << std::endl;
                            grid.reset();
                            grid.activate(plug1, correctPlug);
                        }
                        grid.print();
                        return;
                    }
                }

            }
        }
    }
}

std::vector<std::pair<char,char>> makeGraph(std::string text, std::string crib) {
    std::vector<std::pair<char, char> > enigmaMachines;
    for(int i = 0; i < crib.size(); ++i){
        enigmaMachines.emplace_back(std::pair<char,char>{text[i], crib[i]});
    }
    return enigmaMachines;
}

Grid::Grid() {
    for(int i = 0; i < 26; ++i){
        m_grid[i] = std::vector<int>(26,0);
    }
}

void Grid::addEnigmas(std::vector<std::pair<char, char>> positions, std::vector<Enigma> enigmas) {

    for(int i = 0; i < positions.size(); ++i){
        positionToEnigma[positions[i]] = enigmas[i];
        positionToEnigma[{positions[i].second, positions[i].first}] = enigmas[i];

        if(links.find(positions[i].first) == links.end()){
            links[positions[i].first] = {positions[i].second};
        }
        else{
            links[positions[i].first].push_back(positions[i].second);
        }

        if(links.find(positions[i].second) == links.end()){
            links[positions[i].second] = {positions[i].first};
        }
        else{
            links[positions[i].second].push_back(positions[i].first);
        }
    }
}

void Grid::activate(char c1, char c2) {
    int pos1 = c1 - 'A';
    int pos2 = c2 - 'A';

    if (m_grid[pos1][pos2] == 0){
        m_grid[pos1][pos2] = 1;
        if(links.find(c1) != links.end()){
            for(auto c : links[c1]){
                auto enigmaResult = positionToEnigma[{c1, c}].encrypt(c2);
                activate(c, enigmaResult);
            }
        }
    }
    if (m_grid[pos2][pos1] == 0){
        m_grid[pos2][pos1] = 1;
        if(links.find(c2) != links.end()){
            for(auto c : links[c2]){
                auto enigmaResult = positionToEnigma[{c2, c}].encrypt(c1);
                activate(c, enigmaResult);
            }
        }
    }
}

void Grid::print() {
    std::cout << "  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z" << std::endl;
    for(int i = 0; i < 26; ++i){
        std::cout << char('A'+i) << ' ';
        for(int j = 0; j < 26; ++j){
            std::cout << m_grid[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    for(int i = 0; i < 26; ++i){
        for(int j = 0; j < 26; ++j){
            if(m_grid[i][j]){
                std::cout << char('A'+i) << " = " << char('A'+j) << std::endl;
            }
        }
    }
}

void Grid::reset() {
    for(int i = 0; i < 26; ++i){
        m_grid[i] = std::vector<int>(26,0);
    }
}

bool Grid::correctPosition() {
    for(int i = 0; i < 26; ++i){
        int acc = 0;
        for(int j = 0; j < 26; ++ j){
            acc += m_grid[i][j];
        }
        if(acc == 26)
            return false;
    }
    return true;
}

void Grid::advanceEnigmas() {
    for(auto& pair : positionToEnigma){
        pair.second.advancePosition(1);
    }
}

char Grid::correctPlug(char c1, char c2) {
    int pos1 = 'A'-c1;
    auto acc = 0;
    char lastChar = c2;
    for(int j = 0; j < 26; ++j){
        acc += m_grid[pos1][j];
        if(m_grid[pos1][j] == 0){
            lastChar = 'A' + j;
        }
    }
    return lastChar;
}
