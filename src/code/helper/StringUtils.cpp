#include "../../include/helper/StringUtils.h"
#include <cctype>

string StringUtils::trim(const string& s) {
    size_t b = 0, e = s.size();
    while (b < e && isspace((unsigned char)s[b])) b++;
    while (e > b && isspace((unsigned char)s[e - 1])) e--;
    return s.substr(b, e - b);
}

string StringUtils::toUpper(string s) {
    for (char& c : s) c = (char)toupper((unsigned char)c);
    return s;
}

string StringUtils::toLower(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

bool StringUtils::containsIgnoreCase(const string& str, const string& sub) {
    if (sub.empty()) return true;
    return toLower(str).find(toLower(sub)) != string::npos;
}

bool StringUtils::hasWhitespace(const string& s) {
    for (char c : s) {
        if (isspace((unsigned char)c)) return true;
    }
    return false;
}

bool StringUtils::containsAny(const string& s, const string& chars) {
    return s.find_first_of(chars) != string::npos;
}

int StringUtils::toInt(const string& s, int fallback) {
    try { return stoi(s); } catch (...) { return fallback; }
}

double StringUtils::toDouble(const string& s, double fallback) {
    try { return stod(s); } catch (...) { return fallback; }
}

void StringUtils::assign(string* out, const string& value) {
    if (out != nullptr) *out = value;
}
