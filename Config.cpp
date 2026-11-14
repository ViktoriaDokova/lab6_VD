#include "Config.h"

#include <charconv>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

static vector<string> readTokens(const string& path) {
    ifstream in(path);
    if (!in)
        throw runtime_error("cannot open file '" + path + "'");

    vector<string> tokens;
    string token;
    while (in >> token)
        tokens.push_back(token);

    if (in.bad())
        throw runtime_error("failed to read file '" + path + "'");
    return tokens;
}

static long long parseInteger(const string& text, const string& name, long long min, long long max) {
    long long value = 0;
    const char* end = text.data() + text.size();
    auto [ptr, error] = from_chars(text.data(), end, value);

    if (error == errc::result_out_of_range)
        throw runtime_error(name + " is out of range: '" + text + "'");
    if (error != errc() || ptr != end)
        throw runtime_error(name + " must be an integer, got '" + text + "'");
    if (value < min || value > max)
        throw runtime_error(name + " must be between " + to_string(min) + " and " + to_string(max) +
            ", got " + text);
    return value;
}

Config readConfig(const string& path) {
    vector<string> tokens = readTokens(path);
    if (tokens.size() != 2)
        throw runtime_error("expected 2 values (count and seed), found " + to_string(tokens.size()));

    Config config;
    config.count = static_cast<int>(parseInteger(tokens[0], "count", 1, MAX_COUNT));
    config.seed = static_cast<unsigned>(parseInteger(tokens[1], "seed", 0, 4294967295LL));
    return config;
}