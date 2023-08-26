#include "../include/findRotor.h"
#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"

/* Enigma parameters */
std::vector<Rotor> rotorList;
std::vector<Reflector> reflectorList;
std::vector<Plugboard> plugboardList;

// Function to sort the map according
// to value in a (key-value) pairs
// https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/
void sort(std::map<std::string, float>& M)
{
 
    // Declare a multimap
    std::multimap<float, std::string> MM;
 
    // Insert every (key-value) pairs from
    // map M to multimap MM as (value-key)
    // pairs
    for (auto& it : M) {
        MM.insert({ it.second, it.first });
    }
 
    // Print the multimap
    /*
    for (auto& it : MM) {
        std::cout << it.second << ' ' << it.first << std::endl;
    }*/

    // Print the last 10 elements in reverse
    int count = 0;
    for (auto it = MM.rbegin(); it != MM.rend() && count < 10; ++it) {
        std::cout << it->first << ": " << it->second << "\n";
        ++count;
    }
}

void findRotorOrder(std::string ciphertext, int rotorNumbers, std::string reflectorWiring) {
    // go through each rotor possible arrangements with ring setting 0
    // 17576 * 6 * (rotorNumbers! / (6 * (rotorNumbers - 3)!) ) arrangements

    std::cout << ciphertext << std::endl;

    Reflector reflector = Reflector(reflectorWiring);
    reflectorList.push_back(reflector);

    std::cout << "[0%] Looking for the best rotor order..." << std::endl;

    std::vector<std::string> wirings = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};
    std::map<std::string, float> resultDictionary;

    /* Deciphered text */
    std::string deciphered;

    /* Index of coincidence*/
    float IoC;

    std::vector<std::string> chosenWirings(wirings);

    // Each rotor arrangement
    for (size_t i = 0; i < rotorNumbers; ++i) {
        chosenWirings[0] = wirings[i];

        for (size_t j = 0; j < rotorNumbers; ++j) {
            if (i != j) {
                chosenWirings[1] = wirings[j];

                for (size_t k = 0; k < rotorNumbers; ++k) {
                    if (i != k && j != k) {
                        chosenWirings[2] = wirings[k];

                        // Print or use the current combination of rotors
                        //std::cout << reflectorWiring << " - " << chosenWirings[0] << " - " << chosenWirings[1] << " - " << chosenWirings[2] << std::endl;
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

                        // Store the IoC with its configuration
                        resultDictionary[reflectorWiring + " " + chosenWirings[0] + " " + chosenWirings[1] + " " + chosenWirings[2]] = IoC;

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

    // Sort IoCs and print the top 10 configurations
    sort(resultDictionary);
}
