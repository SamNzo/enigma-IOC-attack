#include "../include/findPlugboard.h"
#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"

std::vector<std::string> connections;

// Function to sort the map according
// to value in a (key-value) pairs
// https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/
void sort2(std::map<std::string, float>& M, bool print)
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
        connections.push_back(it->second);
    }
}

int findPlugboardConnections(std::string ciphertext, std::vector<Rotor> rotorList, std::vector<Reflector> reflectorList) {

    std::cout << "[+] Looking for the best plugboard connections..." << std::endl;

    /* Enigma parameters */
    std::vector<Plugboard> plugboardList;
    std::vector<std::string> savedCouples;
    std::string savedLetters;
    std::vector<std::string> couples;
    std::map<float, std::string> decipheredVector;
    std::string combination;

    std::map<std::string, float> resultDictionary;
    std::string deciphered;
    float IoC;

    for (char first = 'A'; first <= 'Z'; ++first) {
        for (char second = first; second <= 'Z'; ++second) {
            if (first != second) {
                couples.clear();
                combination.clear();
                plugboardList.clear();
                combination += first;
                combination += second;
                couples.push_back(combination);
                plugboardList.push_back(couples);

                // Create enigma machine
                Enigma *enigma = new Enigma(rotorList, reflectorList, plugboardList);

                // Try those settings
                deciphered = enigma->encode(ciphertext);
                decipheredVector[IoC] = deciphered;
                IoC = ComputeIndexOfCoincidence(deciphered);

                resultDictionary[combination] = IoC;

                delete enigma;
            }
        }
    }

    // get max value
    sort2(resultDictionary, true);

    savedCouples.push_back(connections[0]);
    savedLetters += connections[0][0]; // add letter already plugged
    savedLetters += connections[0][1]; // so they cant be plugged again

    std::cout << "Deciphered text: \n"
                << "+----------------------------------------------------+\n"
                << decipheredVector[resultDictionary[connections[0]]]
                << "\n+----------------------------------------------------+\n"
                << std::endl;

    // ask if the user wants to look for other plugboard connections
    // maximum is 10 couples (= 20 letters)
    std::string userInput;
    size_t pos;
    while (savedLetters.size() < 20) {

        std::cout << "Connections found: ";

        for (int i=0; i<savedCouples.size(); i++) {
            std::cout << savedCouples[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Do you want to keep working out the plugboard connections? (yes/no) [yes]:\n";
        std::cin >> userInput;
        // Convert user input to lowercase for case-insensitive comparison
        for (char& c : userInput) {
            c = std::tolower(c);
        }

        if (userInput == "yes" || userInput == "y") {
            std::cout << "Proceeding...\n";
            // find the most likely plugboard connections

            resultDictionary.clear();
            decipheredVector.clear();
            connections.clear();
            
            for (char first = 'A'; first <= 'Z'; ++first) {
                for (char second = first; second <= 'Z'; ++second) {
                    if (first != second ) {
                        pos = savedLetters.find(first);
                        if (pos == std::string::npos) {
                            // first letter not connected already
                            pos = savedLetters.find(second);
                            if (pos == std::string::npos) {
                                // second letter not connected already
                                // check connection
                                couples.clear();
                                combination.clear();
                                plugboardList.clear();
                                combination += first;
                                combination += second;
                                couples.push_back(combination);

                                for (int i=0; i<savedCouples.size(); i++) { // add already found connections
                                    couples.push_back(savedCouples[i]);
                                }

                                plugboardList.push_back(couples);

                                // Create enigma machine
                                Enigma *enigma = new Enigma(rotorList, reflectorList, plugboardList);

                                // Try those settings
                                deciphered = enigma->encode(ciphertext);
                                decipheredVector[IoC] = deciphered;
                                IoC = ComputeIndexOfCoincidence(deciphered);

                                resultDictionary[combination] = IoC;

                                delete enigma;
                            }
                        }

                    }
                }
            }

            // get max value
            sort2(resultDictionary, true);

            savedCouples.push_back(connections[0]);
            savedLetters += connections[0][0]; // add letter already plugged
            savedLetters += connections[0][1]; // so they cant be plugged again

            std::cout << "Deciphered text: \n"
                        << "+----------------------------------------------------+\n"
                        << decipheredVector[resultDictionary[connections[0]]]
                        << "\n+----------------------------------------------------+\n"
                        << std::endl;

            
        } else if (userInput == "no" || userInput == "n") {
            return EXIT_SUCCESS;
        } else {
            std::cerr << "Invalid input.\n";
            return EXIT_FAILURE;
        }

    }
    return 0;
}