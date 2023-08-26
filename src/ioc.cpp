#include "../include/ioc.h"

float ComputeIndexOfCoincidence(std::string text) {
    int c = 26;
    float IoC = 0;
    std::vector<int> n;
    std::vector<char> doneLetters;

    // remove spaces
    text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());

    int N = text.size();

    for (char letter: text) {
        // if it has not already been counted
        if (!(std::find(doneLetters.begin(), doneLetters.end(), letter) != doneLetters.end())) {
            // add occurence number
            n.push_back(std::count(text.begin(), text.end(), letter));
            doneLetters.push_back(letter);
        }
    }

    for (int j=0; j<n.size(); j++) {
        IoC += n[j] * (n[j] - 1);
    }

    IoC = IoC / (N * (N - 1)); 

    return IoC;
}