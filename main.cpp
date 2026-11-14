// Variant 6.
#include "Config.h"
#include "Coroutine.h"
#include "Transfer.h"

#include <exception>
#include <iostream>
#include <random>
#include <string>

using namespace std;

Coroutine generateNumbers(Exchange& exchange, Config config) {
    mt19937 rng(config.seed);
    uniform_int_distribution<int> distribution(1, 256);

    for (int i = 1; i <= config.count; i++) {
        int number = distribution(rng);
        cout << i << ") generated " << number << " -> ";
        co_await SendByParity(exchange, number);
    }
}

// A and B never finish on their own: they are destroyed after the generator is done.
Coroutine printNumber(Exchange& exchange) {
    while (true) {
        cout << "A: " << exchange.value << "\n";
        co_await ReturnToGenerator(exchange);
    }
}

Coroutine printSquare(Exchange& exchange) {
    while (true) {
        cout << "B: " << exchange.value << "^2 = " << exchange.value * exchange.value << "\n";
        co_await ReturnToGenerator(exchange);
    }
}

void runDemo(const Config& config) {
    Exchange exchange;
    Coroutine a = printNumber(exchange);
    Coroutine b = printSquare(exchange);
    exchange.evenPrinter = a.handle();
    exchange.oddPrinter = b.handle();

    Coroutine generator = generateNumbers(exchange, config);
    generator.resume();
}

int main(int argc, char* argv[]) {
    string path = argc > 1 ? argv[1] : "input.txt";

    try {
        Config config = readConfig(path);
        cout << "Input file: " << path << "\n";
        cout << "Count: " << config.count << ", seed: " << config.seed << "\n\n";
        runDemo(config);
        cout << "\nDone." << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}