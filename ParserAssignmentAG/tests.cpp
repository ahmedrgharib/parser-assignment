#define UNIT_TEST
#include "main.cpp"

#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string input;
    bool expectedFound;
    unsigned long expectedAddress;
    int expectedPort;
};

unsigned long makeAddress(unsigned long a,
                          unsigned long b,
                          unsigned long c,
                          unsigned long d) {
    return (a << 24)
         | (b << 16)
         | (c << 8)
         | d;
}

int main() {
    const std::vector<TestCase> tests = {

        {"connecting to 192.168.1.1 now",
         true,
         makeAddress(192, 168, 1, 1),
         -1},

        {"server=10.0.0.255:8080end",
         true,
         makeAddress(10, 0, 0, 255),
         8080},

        {"192a168.1.1.1",
         true,
         makeAddress(168, 1, 1, 1),
         -1},

        {"192.168.1.1.",
         false,
         0,
         -1},

        {"Connection from 192.168.1.1 refused",
         true,
         makeAddress(192, 168, 1, 1),
         -1},

        {"192.168.01.1",
         false,
         0,
         -1},

        {"1.2.3.4:99999",
         false,
         0,
         -1},

        {"12.34.56",
         false,
         0,
         -1},

        {"no number here",
         false,
         0,
         -1},

        {"0.0.0.0",
         true,
         makeAddress(0, 0, 0, 0),
         -1},

        {"255.255.255.255",
         true,
         makeAddress(255, 255, 255, 255),
         -1},

        {"256.1.1.1",
         false,
         0,
         -1},

        {"1.2.3.4:0",
         true,
         makeAddress(1, 2, 3, 4),
         0},

        {"1.2.3.4:65535",
         true,
         makeAddress(1, 2, 3, 4),
         65535},

        {"1.2.3.4:65536",
         false,
         0,
         -1},

        {"1.2.3.4:00000",
         false,
         0,
         -1},

        {"1.2.3.4:01",
         false,
         0,
         -1},

        {"01.2.3.4",
         false,
         0,
         -1},

        {"1.02.3.4",
         false,
         0,
         -1},

        {"1..2.3.4",
         false,
         0,
         -1},

        {"1.2.3.4:",
         false,
         0,
         -1},

        {"1.2.3.4:80:90",
         false,
         0,
         -1},

        {"1.2.3.4.5",
         false,
         0,
         -1},

        {".1.2.3.4",
         false,
         0,
         -1},

        {":1.2.3.4",
         false,
         0,
         -1},

        {"abc1.2.3.4,xyz",
         true,
         makeAddress(1, 2, 3, 4),
         -1},

        {"abc999xyz1.2.3.4",
         true,
         makeAddress(1, 2, 3, 4),
         -1},

        {"1.2.3.4 and 5.6.7.8",
         false,
         0,
         -1},

        {"1111.2.3.4",
         false,
         0,
         -1},

        {"1.2.3.4444",
         false,
         0,
         -1},

        {"text 123.45.67.89:12345 text",
         true,
         makeAddress(123, 45, 67, 89),
         12345}
    };

    int passed = 0;

    for (std::size_t i = 0; i < tests.size(); ++i) {

        unsigned long address = 999;
        int port = 999;

        const bool found =
            extractIPv4(tests[i].input, address, port);

        const bool correct =
            found == tests[i].expectedFound &&
            address == tests[i].expectedAddress &&
            port == tests[i].expectedPort;

        std::cout
            << (correct ? "PASS" : "FAIL")
            << " Test "
            << (i + 1)
            << ": "
            << tests[i].input
            << '\n';

        if (!correct) {
            std::cout
                << "Expected: found="
                << tests[i].expectedFound
                << ", address="
                << tests[i].expectedAddress
                << ", port="
                << tests[i].expectedPort
                << '\n';

            std::cout
                << "Actual: found="
                << found
                << ", address="
                << address
                << ", port="
                << port
                << '\n';
        } else {
            ++passed;
        }
    }

    std::cout
        << '\n'
        << passed
        << "/"
        << tests.size()
        << " tests passed."
        << '\n';

    return passed == static_cast<int>(tests.size())
           ? 0
           : 1;
}