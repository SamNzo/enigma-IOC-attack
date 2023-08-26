#include "../include/findRotor.h"
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

/* Amount of rotors to try */
int rotorNumbers = 5; // default number is 5

/* Reflector */
std::string reflectorWiring = "B"; // default wiring is B

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
    const char* const short_options = "h::r:c:R:";
    const struct option long_options[] = {
        {"help", optional_argument, nullptr, 'h'},
        {"rotors", required_argument, nullptr, 'r'},
        {"ciphertext", required_argument, nullptr, 'c'},
        {"reflector", required_argument, nullptr, 'R'},

        {nullptr, 0, nullptr, 0}
    };

    /* Parse command line */
    int option;

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                {
                    // Help
                    printHelp();
                    return EXIT_SUCCESS;
                }
                break;
            case 'r':
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

    // find the most likely rotor order
    findRotorOrder(ciphertext, rotorNumbers, reflectorWiring);

    return 0;
}
