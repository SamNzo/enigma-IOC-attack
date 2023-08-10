#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"
#include "Help.cpp"
#include <getopt.h>

int main(int argc, char** argv) {
    
    /* Program command line options */
    const char* const short_options = "h::r";
    const struct option long_options[] = {
        {"help", optional_argument, nullptr, 'h'},
        {"rotors", required_argument, nullptr, 'r'},

        {nullptr, 0, nullptr, 0}
    };

    /* Parse command line */
    int option;

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                {
                    printHelp();
                    return EXIT_SUCCESS;
                }
                break;
        }
    }

    return 0;
}
