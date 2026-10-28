#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
using namespace std;

// tien ich chuoi
class StringUtils {
public:
    StringUtils() = delete;

    static string trim(const string& s);
    static string toUpper(string s);
    static string toLower(string s);
    static bool containsIgnoreCase(const string& str, const string& sub);
    static bool hasWhitespace(const string& s);
    static bool containsAny(const string& s, const string& chars);

    // doi so an toan, sai dinh dang thi tra ve fallback
    static int toInt(const string& s, int fallback = 0);
    static double toDouble(const string& s, double fallback = 0.0);

    // gan gia tri cho tham so ra, bo qua neu out la nullptr
    static void assign(string* out, const string& value);
};

#endif
