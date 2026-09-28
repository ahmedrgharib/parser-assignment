#include <iostream>
#include <string>
#include <cctype>

// Returns true if a valid address was found, false otherwise.
// On success: outAddress holds the 32-bit value,
// and outPort holds the port number, or -1 if no port was present.
// On failure: outAddress is set to 0 and outPort is set to -1.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);

namespace {

bool isTokenChar(char ch) {
    return std::isdigit(static_cast<unsigned char>(ch)) || ch == '.' || ch == ':';
}

// Parses an unsigned decimal field without using library conversion functions.
bool parseNumber(const std::string& str,
                 std::size_t& pos,
                 std::size_t end,
                 int maxDigits,
                 unsigned long maxValue,
                 unsigned long& value) {
    if (pos >= end || !std::isdigit(static_cast<unsigned char>(str[pos]))) {
        return false;
    }

    const std::size_t first = pos;
    value = 0;
    int digitCount = 0;

    while (pos < end && std::isdigit(static_cast<unsigned char>(str[pos]))) {
        value = value * 10UL + static_cast<unsigned long>(str[pos] - '0');
        ++digitCount;
        ++pos;

        if (digitCount > maxDigits || value > maxValue) {
            return false;
        }
    }

    // Multi-digit values cannot start with zero.
    if (digitCount > 1 && str[first] == '0') {
        return false;
    }

    return true;
}

bool parseCandidate(const std::string& str,
                    std::size_t start,
                    std::size_t end,
                    unsigned long& address,
                    int& port) {
    std::size_t pos = start;
    unsigned long octets[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; ++i) {
        if (!parseNumber(str, pos, end, 3, 255UL, octets[i])) {
            return false;
        }

        if (i < 3) {
            if (pos >= end || str[pos] != '.') {
                return false;
            }

            ++pos;
        }
    }

    port = -1;

    if (pos < end) {
        if (str[pos] != ':') {
            return false;
        }

        ++pos;

        unsigned long parsedPort = 0;

        if (!parseNumber(str, pos, end, 5, 65535UL, parsedPort)) {
            return false;
        }

        port = static_cast<int>(parsedPort);
    }

    // Nothing may remain in the candidate.
    if (pos != end) {
        return false;
    }

    address = (octets[0] << 24)
            | (octets[1] << 16)
            | (octets[2] << 8)
            | octets[3];

    return true;
}

} // namespace

bool extractIPv4(const std::string& str,
                 unsigned long& outAddress,
                 int& outPort) {
    outAddress = 0;
    outPort = -1;

    bool found = false;
    unsigned long foundAddress = 0;
    int foundPort = -1;

    std::size_t i = 0;

    while (i < str.size()) {
        if (!isTokenChar(str[i])) {
            ++i;
            continue;
        }

        const std::size_t start = i;

        while (i < str.size() && isTokenChar(str[i])) {
            ++i;
        }

        const std::size_t end = i;

        unsigned long candidateAddress = 0;
        int candidatePort = -1;

        if (parseCandidate(str,
                           start,
                           end,
                           candidateAddress,
                           candidatePort)) {

            if (found) {
                outAddress = 0;
                outPort = -1;
                return false;
            }

            found = true;
            foundAddress = candidateAddress;
            foundPort = candidatePort;
        }
    }

    if (!found) {
        return false;
    }

    outAddress = foundAddress;
    outPort = foundPort;

    return true;
}

#ifndef UNIT_TEST

int main() {
    std::string input;

    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        std::getline(std::cin, input);

        if (!std::cin) {
            break;
        }

        if (input == "END") {
            std::cout << "Program terminated." << std::endl;
            break;
        }

        unsigned long address = 0;
        int port = -1;

        if (extractIPv4(input, address, port)) {
            const unsigned long a = (address >> 24) & 0xFFUL;
            const unsigned long b = (address >> 16) & 0xFFUL;
            const unsigned long c = (address >> 8) & 0xFFUL;
            const unsigned long d = address & 0xFFUL;

            std::cout << "Extracted IPv4 address: "
                      << a << '.'
                      << b << '.'
                      << c << '.'
                      << d
                      << " (decimal value: "
                      << address
                      << ", port: ";

            if (port == -1) {
                std::cout << "none";
            } else {
                std::cout << port;
            }

            std::cout << ')' << std::endl;
        } else {
            std::cout
                << "Invalid input: no valid IPv4 address found"
                << std::endl;
        }
    }

    return 0;
}

#endif