#include "../../include/helper/PasswordHasher.h"

string PasswordHasher::hash(const string& plaintext) {
    unsigned long h = 5381;
    for (unsigned char c : plaintext) {
        h = ((h << 5) + h) + c;
    }
    return to_string(h);
}

bool PasswordHasher::verify(const string& plaintext, const string& hashed) {
    return hash(plaintext) == hashed;
}
