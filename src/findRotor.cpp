#include "../include/findRotor.h"
#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"

/* Enigma parameters */
std::vector<Rotor> rotorList;
std::vector<Reflector> reflectorList;
std::vector<Plugboard> plugboardList;

void findRotorOrder(std::string ciphertext, int rotorNumbers, std::string reflectorWiring) {
    // go through each rotor possible arrangements with ring setting 0
    // 17576 * 6 * (rotorNumbers! / (6 * (rotorNumbers - 3)!) ) arrangements

    Reflector reflector = Reflector(reflectorWiring);
    reflectorList.push_back(reflector);

    std::cout << "Looking for best rotor order.." << std::endl;

    std::vector<std::string> wirings = {"I", "II", "III"};
    std::vector<std::string> results;

    /* Deciphered text */
    std::string deciphered;

    /* Index of coincidence*/
    int IoC;

    std::vector<std::string> chosenWirings(wirings);

    // Each rotor arrangement
    for (size_t i = 0; i < wirings.size(); ++i) {
        chosenWirings[0] = wirings[i];

        for (size_t j = 0; j < wirings.size(); ++j) {
            if (i != j) {
                chosenWirings[1] = wirings[j];

                for (size_t k = 0; k < wirings.size(); ++k) {
                    if (i != k && j != k) {
                        chosenWirings[2] = wirings[k];

                        // Print or use the current combination of rotors
                        std::cout << chosenWirings[0] << " - " << chosenWirings[1] << " - " << chosenWirings[2] << std::endl;
                        // Create rotor configuration
                        Rotor *leftRotor = new Rotor(chosenWirings[0], 0, 0, 1);
                        Rotor *middleRotor = new Rotor(chosenWirings[1], 0, 0, 2);
                        Rotor *rightRotor = new Rotor(chosenWirings[2], 0, 0, 3);
                        rotorList.push_back(*leftRotor);
                        rotorList.push_back(*middleRotor);
                        rotorList.push_back(*rightRotor);

                        // Create enigma machine
                        Enigma *enigma = new Enigma(rotorList, reflectorList, plugboardList);

                        // Try those settings
                        deciphered = enigma->encode(ciphertext);
                        IoC = ComputeIndexOfCoincidence(deciphered);

                        // Clear list & free memory
                        rotorList.clear(); // note that clear() leaves pointer management to user
                        delete leftRotor;
                        delete middleRotor;
                        delete rightRotor;
                        delete enigma;

                    }
                }
            }
        }
    }
}
