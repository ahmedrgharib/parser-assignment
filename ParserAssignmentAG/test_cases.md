# IPv4 Parser Test Cases

I tested the parser using the instructor-provided examples as well as additional boundary and malformed-input cases.

| Input | Expected Result | Reason |
|---|---|---|
| `connecting to 192.168.1.1 now` | Valid | Standard IPv4 embedded in text |
| `server=10.0.0.255:8080end` | Valid, port 8080 | Valid IPv4 and port |
| `192a168.1.1.1` | Valid `168.1.1.1` | Letter separates candidate tokens |
| `192.168.1.1.` | Invalid | Trailing period makes whole candidate malformed |
| `192.168.01.1` | Invalid | Leading zero |
| `1.2.3.4:99999` | Invalid | Port greater than 65535 |
| `12.34.56` | Invalid | Only three octets |
| `0.0.0.0` | Valid | Minimum octet values |
| `255.255.255.255` | Valid | Maximum octet values |
| `256.1.1.1` | Invalid | Octet exceeds 255 |
| `1.2.3.4:0` | Valid | Minimum valid port |
| `1.2.3.4:65535` | Valid | Maximum valid port |
| `1.2.3.4:65536` | Invalid | Port exceeds maximum |
| `1.2.3.4:01` | Invalid | Port has a leading zero |
| `01.2.3.4` | Invalid | Octet has a leading zero |
| `1..2.3.4` | Invalid | Empty octet |
| `1.2.3.4:` | Invalid | Colon without port |
| `1.2.3.4:80:90` | Invalid | Second colon |
| `1.2.3.4.5` | Invalid | Extra octet |
| `.1.2.3.4` | Invalid | Stray adjacent period |
| `1111.2.3.4` | Invalid | More than three digits in an octet |
| `1.2.3.4444` | Invalid | More than three digits in an octet |
| `abc1.2.3.4,xyz` | Valid | Letters and comma separate garbage from the candidate |
| `1.2.3.4 and 5.6.7.8` | Invalid | Two valid addresses appear in the same line |

The automated test driver contains 31 total test cases.