//
// Created by SuperPC on 10/1/2019.
//

#ifndef HASHING_RYTISMASTERCLASSONHASHINGALGORITHMS_H
#define HASHING_RYTISMASTERCLASSONHASHINGALGORITHMS_H

#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <sstream>

const int hashSize = 24;
const int goodCharsLength = 64;
auto goodChars = const_cast<char *>("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+/");
const int byteShift [6][64] = {
        {0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
        {0, 32, 1,  33, 2,  34, 3,  35, 4,  36, 5,  37, 6,  38, 7,  39, 8,  40, 9,  41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63},
        {0, 16, 32, 48, 1,  17, 33, 49, 2,  18, 34, 50, 3,  19, 35, 51, 4,  20, 36, 52, 5,  21, 37, 53, 6,  22, 38, 54, 7,  23, 39, 55, 8,  24, 40, 56, 9,  25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59, 12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63},
        {0, 8,  16, 24, 32, 40, 48, 56, 1,  9,  17, 25, 33, 41, 49, 57, 2,  10, 18, 26, 34, 42, 50, 58, 3,  11, 19, 27, 35, 43, 51, 59, 4,  12, 20, 28, 36, 44, 52, 60, 5,  13, 21, 29, 37, 45, 53, 61, 6,  14, 22, 30, 38, 46, 54, 62, 7,  15, 23, 31, 39, 47, 55, 63},
        {0, 4,  8,  12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 1,  5,  9,  13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 2,  6,  10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 3,  7,  11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63},
        {0, 2,  4,  6,  8,  10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 1,  3,  5,  7,  9,  11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63}
};

std::vector<int> convertToASCII(std::string s) {
    std::vector<int> newlineArray;
    for (int i = 0; i < s.length(); i++) {
        if((int) s[i] >= 0) {
            newlineArray.push_back((int) s[i]);
        }
    }
    return newlineArray;
}

int ror63(int toBits, unsigned int moves) {
    std::bitset<6> bits(toBits);
    std::bitset<6> newSet;
    int moveBy = moves % 6;
    for (int i = 5; i >= 0; i--) {
        newSet[i] = bits[(i + moveBy) % 6];
    }
    return (int) newSet.to_ulong();
}

std::string hash(const std::string &input) {
    std::vector<int> passVector{};
    std::vector<int> tempVector{};
    for (long double number : convertToASCII(input)) {
        tempVector.clear();
        long double temp = pow(number, 1 / 691.) * pow(10, 5);
        temp -= (long double) (int) temp;
        int tempInt;
        for (int i = 0; i < hashSize; i++) {
            tempVector.push_back(tempInt = static_cast<int>(temp *= 100));
            temp -= (long double) tempInt;
        }
        if (passVector.empty())
            for (auto single : tempVector) passVector.push_back(single % goodCharsLength);
        else {
            int i = 0;
            for (auto single : tempVector) {
                passVector.at(i) = byteShift[single%6][(passVector.at(i) + single) % goodCharsLength];
                i++;
            }
        }
    };
    std::ostringstream pass;
    for (auto single : passVector)
        pass << goodChars[single];
    return pass.str();
}
std::vector<int> hashForCompare(const std::string &input) {
    std::vector<int> passVector{};
    std::vector<int> tempVector{};
    for (long double number : convertToASCII(input)) {
        tempVector.clear();
        long double temp = pow(number, 1 / 691.) * pow(10, 5);
        temp -= (long double) (int) temp;
        int tempInt;
        for (int i = 0; i < hashSize; i++) {
            tempVector.push_back(tempInt = static_cast<int>(temp *= 100));
            temp -= (long double) tempInt;
        }
        if (passVector.empty())
            for (auto single : tempVector) passVector.push_back(single % goodCharsLength);
        else {
            int i = 0;
            for (auto single : tempVector) {
                passVector.at(i) = byteShift[single%6][(passVector.at(i) + single) % goodCharsLength];
                i++;
            }
        }
    };
    return passVector;
}
double HashComparer(std::string string1, std::string string2){
    int r = 0;
    auto hash1{hashForCompare(string1)},hash2{hashForCompare(string2)};
    for(int i = 0; i < hash1.size(); i++){
        std::bitset<6> b1(hash1.at(i));
        std::bitset<6> b2(hash2.at(i));
        r += (b1&b2).count();
        r += ((~b1)&(~b2)).count();
    }
    return (double)r/(hash1.size()*6);
}
#endif //HASHING_RYTISMASTERCLASSONHASHINGALGORITHMS_H
