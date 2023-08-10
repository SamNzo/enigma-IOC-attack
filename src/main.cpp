#include "../enigma/include/Enigma.h"
#include "../include/ioc.h"

int main(int argc, char** argv) {
    std::string text = "THERE ARETW OWAYS OFCON STRUC TINGA SOFTW AREDE SIGNO NEWAY ISTOM AKEIT SOSIM PLETH ATTHE REARE OBVIO USLYN ODEFI CIENC IESAN DTHEO THERW AYIST OMAKE ITSOC OMPLI CATED THATT HEREA RENOO BVIOU SDEFI CIENC IESTH EFIRS TMETH ODISF ARMOR EDIFF ICULT"; // Replace with the actual text
    
    if (isValid(text)) {
        float ioc = ComputeIndexOfCoincidence(text);
        std::cout << "Index of Coincidence: " << ioc << std::endl;
    }

    return 0;
}
