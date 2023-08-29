#ifndef FINDPLUGBOARD_H
#define FINDPLUGBOARD_H
#include <iostream>
#include <string.h>
#include <sstream>
#include <iterator>
#include "../enigma/include/Rotor.h"
#include "../enigma/include/Reflector.h"
#include <map>

int findPlugboardConnections(std::string ciphertext, std::vector<Rotor> rotorList, std::vector<Reflector> reflectorList);

#endif