#include "../include/ioc.h"

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