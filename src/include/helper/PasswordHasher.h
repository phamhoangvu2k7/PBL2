#ifndef PASSWORD_HASHER_H
#define PASSWORD_HASHER_H

#include <string>
using namespace std;

// bam mat khau mot chieu (DJB2)
class PasswordHasher {
public:
    PasswordHasher() = delete;

    static string hash(const string& plaintext);
    static bool verify(const string& plaintext, const string& hashed);
};

#endif
