#include "../include/findRotor.h"
#include "../include/findPlugboard.h"
#include "../enigma/include/Rotor.h"
#include "../enigma/include/Reflector.h"
#include "Help.cpp"
#include <getopt.h>
#include <fstream>

/* Cipher text file */
std::fstream file;
std::string ciphertext;

/* Booleans */
bool rotorNumberProvided = false;
bool reflectorProvided = false;
bool ciphertextProvided = false;
bool firstStep = false;
bool secondStep = false;
bool batch = false;

/* Help booleans */
bool helpRotor = false;
bool helpReflector = false;
bool helpAttack = false;

/* Amount of rotors to try */
int rotorNumbers = 5; // default number is 5

/* Reflector */
std::string reflectorWiring = "B"; // default wiring is B

/* Rotor configuration */
std::vector<std::string> rotorArguments;

/* Function to check if ciphertext provided is valid */
bool isValid(std::string text) {
    // only letters
    for (char c : text) {
        if (!(std::isalpha(c) &&
              ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)))) {
            return true; // Found a non-alphabetical character
        }
    }
    return false; // No non-alphabetical characters found
}

/* Function to open and read file content */
bool openFile(const std::string &filename) {
    file.open(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 0;
    } else {
        ciphertext = std::string((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
        file.close();
    }

    if (!isValid(ciphertext)) {
        std::cerr << "Ciphertext content is invalid. Make sure that it only contains [Aa-zZ] letters" << std::endl;
        return 0;
    }
    else {
        return 1;
    }
}

int main(int argc, char** argv) {
    
    /* Program command line options */
    const char* const short_options = "h::n:c:R:rb";
    const struct option long_options[] = {
        {"help", optional_argument, nullptr, 'h'},
        {"rotor-number", required_argument, nullptr, 'n'},
        {"ciphertext", required_argument, nullptr, 'c'},
        {"reflector", required_argument, nullptr, 'R'},
        {"rotors", no_argument, nullptr, 'r'},
        {"batch", no_argument, nullptr, 'b'},

        {nullptr, 0, nullptr, 0}
    };

    /* Parse command line */
    int option;

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                {
                    if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
                        optarg = argv[optind++];
                        std::string argument(optarg);
                        if (argument == "rotor") {
                            helpRotor = true;
                        } 
                        else if (argument == "reflector") {
                            helpReflector = true;
                        }
                        else if (argument == "attack") {
                            helpAttack = true;
                        }
                    }
                    printHelp(helpRotor, helpReflector, helpAttack);
                    return EXIT_SUCCESS;
                }
                break;
            case 'n':
                {
                    // Number of rotors to try (between 3 and 8)
                    rotorNumberProvided = true;

                    char* endPtr;
                    errno = 0;
                    rotorNumbers = std::strtol(optarg, &endPtr, 10);
                    
                    if (errno == ERANGE || *endPtr != '\0') {
                        std::cerr << "Number of rotor provided is not supported. : './ioc -h rotors' for more information" << std::endl;
                        return EXIT_FAILURE;
                    }

                    if (rotorNumbers < 3 || rotorNumbers > 8) {
                        std::cerr << "Number of rotor provided is not supported. : './ioc -h rotors' for more information" << std::endl;
                        return EXIT_FAILURE;
                    }
                }
                break;
            case 'c':
                {
                    // Ciphertext file path
                    std::string filename = optarg;
                    ciphertextProvided = true;

                    if (!openFile(filename)) {
                        return EXIT_FAILURE;
                    }
                }
                break;
            case 'R':
                {
                    // Reflector
                    reflectorProvided = true;
                    reflectorWiring = optarg;
                }
                break;
            case 'r':
                {
                    // First step (rotor configuration)
                    firstStep = true;
                }
                break;
            case 'b':
                {
                    batch = true;
                }
                break;
            default:
                std::cerr << "Invalid option" << std::endl;
                break;
        }
    }

    // Check mandatory parameter
    if (!ciphertextProvided) {
        std::cerr << "-c option is mandatory. See `./ioc-attack -h` for more information." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::string> rotorConfig;
    rotorConfig = findRotorConfig(ciphertext, rotorNumbers, reflectorWiring, batch);

    std::string userInput;
    std::cout << "Do you want to work out the plugboard connections? (yes/no) [yes]:\n";
    std::cin >> userInput;
    // Convert user input to lowercase for case-insensitive comparison
    for (char& c : userInput) {
        c = std::tolower(c);
    }

    if (userInput == "yes" || userInput == "y") {
        std::cout << "Proceeding...\n";
        // find the most likely plugboard connections
        std::vector<Rotor> rotorList;
        std::vector<Reflector> reflectorList;
        Reflector reflector = Reflector(rotorConfig[0]);
        Rotor leftRotor = Rotor(rotorConfig[1], 0, stoi(rotorConfig[4]), 1);
        Rotor middleRotor = Rotor(rotorConfig[2], stoi(rotorConfig[7]), stoi(rotorConfig[5]), 2);
        Rotor rightRotor = Rotor(rotorConfig[3], stoi(rotorConfig[8]), stoi(rotorConfig[6]), 3);
        for (int i=0; i<rotorConfig.size(); i++) {
            std::cout << rotorConfig[i] << "\n";
        }
        reflectorList.push_back(reflector);
        rotorList.push_back(leftRotor);
        rotorList.push_back(middleRotor);
        rotorList.push_back(rightRotor);
        int p;
        p = findPlugboardConnections(ciphertext, rotorList, reflectorList);

    } else if (userInput == "no" || userInput == "n") {
        return 0;
    } else {
        std::cerr << "Invalid input.\n";
        return EXIT_FAILURE;
    }


    return 0;
}
