#include <string.h>
#include <cctype>

double calculateIndexOfCoincidence(const std::string &text) {
    int frequency[26] = {0};
    int totalCharacters = 0;

    // Count the frequency of each letter
    for (char c : text) {
        if (isalpha(c)) {
            c = tolower(c); // Convert to lowercase for case-insensitivity
            frequency[c - 'a']++;
            totalCharacters++;
        }
    }

    // Calculate the Index of Coincidence
    double indexOfCoincidence = 0.0;
    for (int i = 0; i < 26; ++i) {
        double f = static_cast<double>(frequency[i]) / totalCharacters;
        indexOfCoincidence += f * (f - 1);
    }

    return indexOfCoincidence;
}