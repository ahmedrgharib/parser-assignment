# IPv4 Address Extraction Program

This program searches a line of text for a valid IPv4 address with an optional port number.

The program is written in C++.

## Rules

A valid IPv4 address contains four octets separated by periods.

Each octet:
- contains 1 to 3 digits
- has a value from 0 to 255
- cannot have a leading zero unless the octet is exactly 0

An optional port may follow the address.
A port:
- begins with a colon
- contains 1 to 5 digits
- has a value from 0 to 65535
- cannot have a leading zero unless the port is exactly 0

The program manually converts digit characters into numeric values and does not use built-in string-to-number conversion functions.

## Compiling

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o ipv4_parser