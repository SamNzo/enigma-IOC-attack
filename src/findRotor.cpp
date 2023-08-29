#include "../include/findRotor.h"
#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"
#include "../include/progressbar.hpp"

/* Enigma parameters */
std::vector<Rotor> rotorList;
std::vector<Reflector> reflectorList;
std::vector<Plugboard> plugboardList;

std::vector<std::string> config;

// Function to sort the map according
// to value in a (key-value) pairs
// https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/
void sort(std::map<std::string, float>& M, bool print)
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
        if (print) {
            std::cout << "[" << count+1 << "] " << it->first << ": " << it->second << "\n";
        }
        ++count;
        config.push_back(it->second);
    }
}

int factorial(int n) {
    int f = 1;
    for (int i=n; i>0; i--) {
        f = f * i;
    }
    return f;
}

std::vector<std::string> findRotorConfig(std::string ciphertext, int rotorNumbers, std::string reflectorWiring, bool batch) {
    // go through each rotor possible arrangements with ring setting 0
    // 17576 * 6 * (rotorNumbers! / (6 * (rotorNumbers - 3)!) ) arrangements

    /* Set up the progressbar*/
    progressbar bar(17576 * 6 * (factorial(rotorNumbers) / (6 * factorial(rotorNumbers - 3)) ));

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
                                    resultDictionary[reflectorWiring + " " + chosenWirings[0] + " " + chosenWirings[1] + " " + chosenWirings[2] + " " + std::to_string(l) + " " + std::to_string(m) + " " + std::to_string(n)] = IoC;

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

    std::cout << std::endl << "[+] Done" << std::endl << std::endl;

    // Sort IoCs and print the top 10 configurations
    sort(resultDictionary, true);

    // RING SETTINGS
    int x;

    do {
        std::cout << "\nWhich one do you want to test?\n"; // Type a number and press enter
        std::cin >> x; // Get user input from the keyboard
    } while (x<1 || x>10);

    std::istringstream iss(config[x-1]); // split by spaces
    std::vector<std::string> rotorConfig{
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}
    };

    float savedIoC = resultDictionary[config[x-1]];

    resultDictionary.clear();
    for (int i=0; i<26; i++) { // find best ring setting / starting position for right rotor
        Rotor *leftRotor = new Rotor(rotorConfig[1], 0, stoi(rotorConfig[4]), 1);
        Rotor *middleRotor = new Rotor(rotorConfig[2], 0, stoi(rotorConfig[5]), 2);
        Rotor *rightRotor = new Rotor(rotorConfig[3], i, (stoi(rotorConfig[6])+i)%26, 3);
        rotorList.push_back(*leftRotor);
        rotorList.push_back(*middleRotor);
        rotorList.push_back(*rightRotor);

        // Create enigma machine
        Enigma *enigma = new Enigma(rotorList, reflectorList, plugboardList);

        // Try those settings
        deciphered = enigma->encode(ciphertext);
        IoC = ComputeIndexOfCoincidence(deciphered);

        // Store the IoC with its configuration
        resultDictionary[std::to_string(i) + " " + std::to_string((stoi(rotorConfig[6])+i)%26)] = IoC;

        // Clear list & free memory
        rotorList.clear(); // note that clear() leaves pointer management to user
        delete leftRotor;
        delete middleRotor;
        delete rightRotor;
        delete enigma;
    }

    // get the best ring setting / starting position
    // and fix it for the next step
    config.clear();
    sort(resultDictionary, false);

    int rightRingSetting;
    int rightStartingPosition;

    if (savedIoC != resultDictionary[config[0]]) {
        std::cout << "[+] A better configuration was found for the right rotor: " << config[0] << std::endl;

        std::string userInput;
        std::cout << "Do you want to use it? (yes/no) [yes]: ";
        std::cin >> userInput;

        // Convert user input to lowercase for case-insensitive comparison
        for (char& c : userInput) {
            c = std::tolower(c);
        }

        if (userInput == "yes" || userInput == "y") {
            std::cout << "Proceeding...\n";
            savedIoC = resultDictionary[config[0]];

            std::istringstream iss2(config[0]); // split by spaces
            std::vector<std::string> rightRotorSettings{
                std::istream_iterator<std::string>{iss2},
                std::istream_iterator<std::string>{}
            };

            rightRingSetting = stoi(rightRotorSettings[0]);
            rightStartingPosition = stoi(rightRotorSettings[1]); 

        } else if (userInput == "no" || userInput == "n") {
            rightRingSetting = 0;
            rightStartingPosition = stoi(rotorConfig[6]);
        } else {
            std::cout << "Invalid input.\n";
        }

    }
    else {
        std::cout << "[-] No better configuration found for the right rotor" << std::endl;
        rightRingSetting = 0;
        rightStartingPosition = stoi(rotorConfig[6]);
    }

   
    resultDictionary.clear();
    for (int i=0; i<26; i++) { // find best ring setting / starting position for middle rotor
        Rotor *leftRotor = new Rotor(rotorConfig[1], 0, stoi(rotorConfig[4]), 1);
        Rotor *middleRotor = new Rotor(rotorConfig[2], i, (stoi(rotorConfig[5])+i)%26, 2);
        Rotor *rightRotor = new Rotor(rotorConfig[3], rightRingSetting, rightStartingPosition, 3);
        rotorList.push_back(*leftRotor);
        rotorList.push_back(*middleRotor);
        rotorList.push_back(*rightRotor);

        // Create enigma machine
        Enigma *enigma = new Enigma(rotorList, reflectorList, plugboardList);

        // Try those settings
        deciphered = enigma->encode(ciphertext);
        IoC = ComputeIndexOfCoincidence(deciphered);

        // Store the IoC with its configuration
        resultDictionary[std::to_string(i) + " " + std::to_string((stoi(rotorConfig[6])+i)%26)] = IoC;

        // Clear list & free memory
        rotorList.clear(); // note that clear() leaves pointer management to user
        delete leftRotor;
        delete middleRotor;
        delete rightRotor;
        delete enigma;
    }
    
    // get the best ring setting / starting position
    config.clear();
    sort(resultDictionary, false);

    int middleRingSetting;
    int middleStartingPosition;
    if (savedIoC != resultDictionary[config[0]]) {
        std::cout << "[+] A better configuration was found for the middle rotor: " << config[0] << std::endl;

        std::string userInput2;
        std::cout << "Do you want to use it? (yes/no) [yes]: ";
        std::cin >> userInput2;

        // Set the default value to "yes" if the user input is empty
        if (userInput2.empty()) {
            userInput2 = "yes";
        }

        // Convert user input to lowercase for case-insensitive comparison
        for (char& c : userInput2) {
            c = std::tolower(c);
        }

        if (userInput2 == "yes" || userInput2 == "y") {
            std::cout << "Proceeding...\n";
            savedIoC = resultDictionary[config[0]];

            std::istringstream iss3(config[0]); // split by spaces
            std::vector<std::string> middleRotorSettings{
                std::istream_iterator<std::string>{iss3},
                std::istream_iterator<std::string>{}
            };

            middleRingSetting = stoi(middleRotorSettings[0]);
            middleStartingPosition = stoi(middleRotorSettings[1]); 

        } else if (userInput2 == "no" || userInput2 == "n") {
            middleRingSetting = 0;
            middleStartingPosition = stoi(rotorConfig[5]);
        } else {
            std::cout << "Invalid input.\n";
        }

    }
    else {
        std::cout << "[-] No better configuration found for the middle rotor" << std::endl;
        middleRingSetting = 0;
        middleStartingPosition = stoi(rotorConfig[5]);
    }

    std::cout << "The best rotor configuration found is:\n"
            << "Reflector: " << reflectorWiring << "\n" 
            << "Rotors: " << rotorConfig[1] << " " << rotorConfig[2] << " " << rotorConfig[3] << "\n" 
            << "Ring settings: " << 0 << " " << std::to_string(middleRingSetting) << " " << std::to_string(rightRingSetting) << "\n" 
            << "Starting positions: " << rotorConfig[4] << " " << std::to_string(middleStartingPosition) << " " << std::to_string(rightStartingPosition) << "\n";

    rotorConfig[5] = std::to_string(middleStartingPosition);
    rotorConfig[6] = std::to_string(rightStartingPosition);
    rotorConfig.push_back(std::to_string(middleRingSetting));
    rotorConfig.push_back(std::to_string(rightRingSetting));

    return rotorConfig;
}
