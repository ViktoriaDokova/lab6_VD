#pragma once

#include <string>


// The program reads its parameters from a text file: input.txt in the working
// directory by default, or the file passed as the first command-line argument.
//
// The file must contain exactly two integers separated by whitespace
// (spaces, tabs or new lines): count seed
//   count - how many random numbers to generate, from 1 to 1000
//   seed  - seed of the random number generator, from 0 to 4294967295
//           (the same seed always gives the same sequence)
// 
// The program prints an error message and stops if:
//   - the file cannot be opened or read
//   - the file contains fewer or more than two values
//   - a value is not an integer (e.g. "abc", "5.5", "12x")
//   - a value is outside its allowed range
struct Config {
    int count = 0;
    unsigned seed = 0;
};

const int MAX_COUNT = 1000;

// Throws std::runtime_error with a readable message if the file is invalid.
Config readConfig(const std::string& path);