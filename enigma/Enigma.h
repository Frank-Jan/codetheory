//
// Created by benji on 12/03/19.
//

#ifndef GROEPSWERK_ENIGMA_H
#define GROEPSWERK_ENIGMA_H

#include <string>
#include <vector>
#include <map>
#include "Rotor.h"

class Enigma {
private:
    std::vector<std::string> ROTORS = {"AJDKSIRUXBLHWTMCQGZNPYFVOE", "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "BDFHJLCPRTXVZNYEIWGAKMUSQO", "THEQUICKBROWNFXJMPSVLAZYDG", "XANTIPESOKRWUDVBCFGHJLMQYZ"};

    std::string reflector ="YRUHQSLDPXNGOKMIEBFZCWVJAT";

    std::vector<Rotor> m_rotors = {};

    std::string plugboard = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char reflect(char inp);

    char plug(char inp);
public:
    Enigma();

    Enigma(int rotor0, int rotor1, int rotor2, std::string startPosition);

    /**
     * Encrypt/Decrypt 1 character
     *
     * OPGELET: verplaatst de rotors NIET!!!
     * */
    char encrypt(char inp);

    /**
     * Encrypt/Decrypt een string
     *
     * verplaatst de rotors WEL
     * */
    std::string encrypt(std::string inp);


    /**
     * Zet het plugboard in de string vorm
     * */
    void setPlugBoard(std::string plug);

    /**
     * Verplaats de eerste rotor i stappen, indien nodig update de rest van de rotoren ook
     * */
    void advancePosition(int i);
};


/**
 *  Vind de correcte rotors en posities voor een encrypted string en een crib en geeft een schatting van een correct plugboard
 *
 *  OPGELET: het plugboard kan incompleet zijn bij een crib die niet lang genoeg is, daarom wordt de string niet decrypted door deze functie
 * */
void solveEnigma(std::string encryptedText, std::string crib);

/**
 * Map elke char van de crib op de overeenkomstige char in de encrypted string
 * */
std::vector<std::pair<char,char>> makeGraph(std::string text, std::string crib);


/**
 * Bevat de main functionality voor het schatten van de rotoren en posities
 * */
class Grid{
    std::vector<std::vector<int> > m_grid{26};

    /**
     * Mapt een paar van 2 rijen op de enigma machine met de correcte instelling
     * */
    std::map<std::pair<char, char>, Enigma> positionToEnigma;

    /**
     * Mapt een rij op alle andere rijen die verbonden zijn met een enigma machine
     * */
    std::map<char, std::vector<char> > links;

public:
    Grid();

    /**
     * Voeg enigmamachines toe voor de crib
     *
     * positie i in de lijst @param positions komt overeen met de Enigma i in de lijst @param enigmas
     * */
    void addEnigmas(std::vector<std::pair<char, char>> positions, std::vector<Enigma> enigmas);

    /**
     * Activeer een plaats in het grid, met @param c1 de rij en @param c2 de kolom
     * */
    void activate(char c1, char c2);

    /**
     * Print het volledige grid en alle plug regels die eruit afgeleid kunnen worden
     * */
    void print();

    /**
     * Reset het grid
     * */
    void reset();

    /**
     * Controleert of de rotors / rotorposities juist zijn
     *
     * Gebeurt door te zien of er geen rijen zijn met 26 mogelijke mappings
     * */
    bool correctPosition();

    /**
     * Verplaatst de eerste rotor van alle enigma's in het grid met 1 verder
     * */
    void advanceEnigmas();

    /**
     * Controleert of de juiste plug is ingevuld in de grid
     *
     * Gebeurt door te zien of in de gegeven rij er kolommen zijn met 0
     *
     * OPGELET: werkt alleen correct als de rotoren correct zijn
     * */
    char correctPlug(char c1, char c2);
};

#endif //GROEPSWERK_ENIGMA_H
