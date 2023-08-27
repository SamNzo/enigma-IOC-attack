#include "../include/findRotor.h"
#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"
#include "../include/progressbar.hpp"

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

int factorial(int n) {
    int f = 1;
    for (int i=n; i>0; i--) {
        f = f * i;
    }
    return f;
}

void findRotorOrder(std::string ciphertext, int rotorNumbers, std::string reflectorWiring) {
    // go through each rotor possible arrangements with ring setting 0
    // 17576 * 6 * (rotorNumbers! / (6 * (rotorNumbers - 3)!) ) arrangements

    std::cout << ComputeIndexOfCoincidence(ciphertext) << std::endl;

    /* Set up the progressbar*/
    progressbar bar(17576 * 6 * (factorial(rotorNumbers) / (6 * factorial(rotorNumbers - 3)) ));

    std::cout << ciphertext << std::endl;

    Reflector reflector = Reflector(reflectorWiring);
    reflectorList.push_back(reflector);

    std::cout << "[+] Looking for the best rotor order..." << std::endl;

    std::vector<std::string> wirings = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};
    std::map<std::string, float> resultDictionary;

    /* Deciphered text */
    std::string deciphered;

    /* Index of coincidence*/
    float IoC;

    std::vector<std::string> chosenWirings(wirings);

    // Each rotor arrangement
    for (size_t i = 0; i < rotorNumbers; ++i) {
        chosenWirings[0] = wirings[i]; // left rotor

        for (size_t j = 0; j < rotorNumbers; ++j) {
            if (i != j) {
                chosenWirings[1] = wirings[j]; // middle rotor

                for (size_t k = 0; k < rotorNumbers; ++k) {
                    if (i != k && j != k) {
                        chosenWirings[2] = wirings[k]; // right rotor

                        // Each rotor starting position
                        for (size_t l = 0; l < 26; ++l) {
                            
                            for (size_t m = 0; m < 26; ++m) {

                                for (size_t n = 0; n < 26; ++n) {
                                    bar.update();
                                    // Print or use the current combination of rotors
                                    // Create rotor configuration
                                    Rotor *leftRotor = new Rotor(chosenWirings[0], 0, l, 1);
                                    Rotor *middleRotor = new Rotor(chosenWirings[1], 0, m, 2);
                                    Rotor *rightRotor = new Rotor(chosenWirings[2], 0, n, 3);
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
            }
        }
    }

    std::cout << std::endl << "[+] Done" << std::endl;

    // Sort IoCs and print the top 10 configurations
    sort(resultDictionary);

}
