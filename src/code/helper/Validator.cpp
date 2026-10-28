#include "../../include/helper/Validator.h"
#include "../../include/entities/Schedule.h"
#include <cctype>
#include <sstream>

// ham phu tro

bool Validator::parseTime(const string& time, int& hour, int& minute) {
    // dang HH:MM hoac H:MM
    size_t colonPos = time.find(':');
    if (colonPos == string::npos || colonPos == 0 || colonPos >= time.length() - 1) {
        return false;
    }

    string hourStr = time.substr(0, colonPos);
    string minStr = time.substr(colonPos + 1);

    // toan so, toi da 2 chu so de stoi khong tran
    if (hourStr.size() > 2 || minStr.size() > 2) return false;
    for (char c : hourStr) if (!isdigit((unsigned char)c)) return false;
    for (char c : minStr) if (!isdigit((unsigned char)c)) return false;

    hour = stoi(hourStr);
    minute = stoi(minStr);
    return true;
}

bool Validator::parseDate(const string& date, int& day, int& month, int& year) {
    // dang dd/mm/yyyy
    if (date.length() < 8 || date.length() > 10) return false;

    size_t slash1 = date.find('/');
    if (slash1 == string::npos) return false;
    size_t slash2 = date.find('/', slash1 + 1);
    if (slash2 == string::npos) return false;

    string dStr = date.substr(0, slash1);
    string mStr = date.substr(slash1 + 1, slash2 - slash1 - 1);
    string yStr = date.substr(slash2 + 1);

    for (char c : dStr) if (!isdigit(c)) return false;
    for (char c : mStr) if (!isdigit(c)) return false;
    for (char c : yStr) if (!isdigit(c)) return false;

    if (dStr.empty() || mStr.empty() || yStr.empty()) return false;

    day = stoi(dStr);
    month = stoi(mStr);
    year = stoi(yStr);
    return true;
}

bool Validator::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Validator::daysInMonth(int month, int year) {
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    if (month >= 1 && month <= 12) return days[month];
    return 0;
}

// kiem tra lich hoc

bool Validator::isValidTime(const string& time) {
    int hour, minute;
    if (!parseTime(time, hour, minute)) return false;
    if (hour < 0 || hour > 23) return false;
    // phut chi 00, 15, 30, 45
    if (minute != 0 && minute != 15 && minute != 30 && minute != 45) return false;
    return true;
}

bool Validator::isValidSchedule(const string& schedule) {
    // 1 hoac nhieu khung, ngan cach bang ';'
    Schedule parsed;
    return Schedule::parse(schedule, parsed, nullptr);
}

string Validator::getScheduleError(const string& schedule) {
    Schedule parsed;
    string err;
    if (Schedule::parse(schedule, parsed, &err)) return "";
    return err;
}

// kiem tra thong tin ca nhan

bool Validator::isValidPhone(const string& phone) {
    if (phone.length() != 10) return false;
    if (phone[0] != '0') return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string Validator::getPhoneError(const string& phone) {
    if (phone.empty()) return "So dien thoai khong duoc de trong!";
    if (phone.length() != 10) return "So dien thoai phai co dung 10 chu so!";
    if (phone[0] != '0') return "So dien thoai phai bat dau bang so 0!";
    for (char c : phone) {
        if (!isdigit(c)) return "So dien thoai chi duoc chua chu so!";
    }
    return "";
}

bool Validator::isValidEmail(const string& email) {
    if (email.empty()) return true;  // email co the de trong
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return false;
    if (email.find('@', atPos + 1) != string::npos) return false;  // chi 1 dau @
    if (email.find(' ') != string::npos) return false;
    string domain = email.substr(atPos + 1);
    if (domain[0] == '.' || domain.back() == '.' || domain.find("..") != string::npos) return false;
    if (domain.find('.') == string::npos) return false;
    return true;
}

string Validator::getEmailError(const string& email) {
    if (email.empty()) return "";  // email co the de trong
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return "Email phai chua ky tu '@'!";
    size_t dotPos = email.find('.', atPos + 1);
    if (dotPos == string::npos || dotPos == email.length() - 1) {
        return "Email phai chua dau '.' sau ky tu '@'!";
    }
    return "";
}

bool Validator::isValidDate(const string& date) {
    int d, m, y;
    if (!parseDate(date, d, m, y)) return false;
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > daysInMonth(m, y)) return false;
    return true;
}

int Validator::compareDates(const string& a, const string& b) {
    int d1 = 0, m1 = 0, y1 = 0, d2 = 0, m2 = 0, y2 = 0;
    if (!parseDate(a, d1, m1, y1) || !parseDate(b, d2, m2, y2)) return 0;
    int ka = y1 * 10000 + m1 * 100 + d1;
    int kb = y2 * 10000 + m2 * 100 + d2;
    return ka < kb ? -1 : (ka > kb ? 1 : 0);
}

string Validator::getDateError(const string& date) {
    if (date.empty()) return "Ngay sinh khong duoc de trong!";
    int d, m, y;
    if (!parseDate(date, d, m, y)) {
        return "Dinh dang ngay sai! Dung dinh dang dd/mm/yyyy (VD: 15/03/2007)";
    }
    if (y < 1900 || y > 2100) return "Nam sinh khong hop le!";
    if (m < 1 || m > 12) return "Thang phai tu 1 den 12!";
    if (d < 1 || d > daysInMonth(m, y)) {
        return "Ngay " + to_string(d) + " khong hop le cho thang " + to_string(m) + "!";
    }
    return "";
}

bool Validator::isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ' && (unsigned char)c < 128) {
            // cho phep chu co dau (byte > 127) va dau cach
            return false;
        }
    }
    return true;
}

string Validator::getNameError(const string& name) {
    if (name.empty()) return "Ho va ten khong duoc de trong!";
    return "";
}

bool Validator::isValidGender(const string& gender) {
    return gender == "Nam" || gender == "Nu";
}

// kiem tra du lieu nghiep vu

bool Validator::isValidFee(double fee) {
    return fee > 0;
}

bool Validator::isValidExpYears(int years) {
    return years >= 0;
}

bool Validator::isNotEmpty(const string& str) {
    for (char c : str) {
        if (c != ' ' && c != '\t' && c != '\n') return true;
    }
    return false;
}
