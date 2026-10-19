#include "../../include/helper/Validator.h"
#include <cctype>
#include <sstream>

// ======================== PHỤ TRỢ ========================

bool Validator::parseTime(const string& time, int& hour, int& minute) {
    // Định dạng mong đợi: "HH:MM" hoặc "H:MM"
    size_t colonPos = time.find(':');
    if (colonPos == string::npos || colonPos == 0 || colonPos >= time.length() - 1) {
        return false;
    }

    string hourStr = time.substr(0, colonPos);
    string minStr = time.substr(colonPos + 1);

    // Kiểm tra toàn ký tự số
    for (char c : hourStr) if (!isdigit(c)) return false;
    for (char c : minStr) if (!isdigit(c)) return false;

    hour = stoi(hourStr);
    minute = stoi(minStr);
    return true;
}

bool Validator::parseDate(const string& date, int& day, int& month, int& year) {
    // Định dạng: dd/mm/yyyy
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

// ======================== KIỂM TRA LỊCH HỌC ========================

bool Validator::isValidTime(const string& time) {
    int hour, minute;
    if (!parseTime(time, hour, minute)) return false;
    if (hour < 0 || hour > 23) return false;
    // Phút chỉ được phép là 00, 15, 30, 45
    if (minute != 0 && minute != 15 && minute != 30 && minute != 45) return false;
    return true;
}

bool Validator::isValidSchedule(const string& schedule) {
    // Tìm cặp ngoặc chứa thời gian: "(HH:MM-HH:MM)"
    size_t openParen = schedule.find('(');
    size_t closeParen = schedule.find(')');
    if (openParen == string::npos || closeParen == string::npos || closeParen <= openParen + 1) {
        return false;
    }

    string timeRange = schedule.substr(openParen + 1, closeParen - openParen - 1);

    // Tách giờ bắt đầu và kết thúc qua dấu '-'
    size_t dashPos = timeRange.find('-');
    if (dashPos == string::npos) return false;

    string startTime = timeRange.substr(0, dashPos);
    string endTime = timeRange.substr(dashPos + 1);

    if (!isValidTime(startTime) || !isValidTime(endTime)) return false;

    // Giờ bắt đầu phải trước giờ kết thúc
    int sh, sm, eh, em;
    parseTime(startTime, sh, sm);
    parseTime(endTime, eh, em);

    int startMinutes = sh * 60 + sm;
    int endMinutes = eh * 60 + em;
    if (endMinutes <= startMinutes) return false;

    return true;
}

string Validator::getScheduleError(const string& schedule) {
    if (schedule.empty()) return "Lich hoc khong duoc de trong!";

    size_t openParen = schedule.find('(');
    size_t closeParen = schedule.find(')');
    if (openParen == string::npos || closeParen == string::npos) {
        return "Dinh dang lich hoc sai! VD dung: T2-T4 (18:00-20:00)";
    }

    string timeRange = schedule.substr(openParen + 1, closeParen - openParen - 1);
    size_t dashPos = timeRange.find('-');
    if (dashPos == string::npos) {
        return "Thieu dau '-' giua gio bat dau va gio ket thuc!";
    }

    string startTime = timeRange.substr(0, dashPos);
    string endTime = timeRange.substr(dashPos + 1);

    if (!isValidTime(startTime)) {
        return "Gio bat dau '" + startTime + "' khong hop le!\n"
               "Phut phai la 00, 15, 30 hoac 45 (VD: 18:00, 19:15, 20:30)";
    }
    if (!isValidTime(endTime)) {
        return "Gio ket thuc '" + endTime + "' khong hop le!\n"
               "Phut phai la 00, 15, 30 hoac 45 (VD: 18:00, 19:15, 20:30)";
    }

    int sh, sm, eh, em;
    parseTime(startTime, sh, sm);
    parseTime(endTime, eh, em);
    if (eh * 60 + em <= sh * 60 + sm) {
        return "Gio ket thuc phai lon hon gio bat dau!";
    }

    return "";
}

// ======================== KIỂM TRA THÔNG TIN CÁ NHÂN ========================

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
    if (email.empty()) return true; // Email là tùy chọn
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return false;
    size_t dotPos = email.find('.', atPos + 1);
    if (dotPos == string::npos || dotPos == email.length() - 1) return false;
    return true;
}

string Validator::getEmailError(const string& email) {
    if (email.empty()) return ""; // Email là tùy chọn
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
            // Cho phép ký tự Unicode (UTF-8 byte > 127) và khoảng trắng
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

// ======================== KIỂM TRA DỮ LIỆU NGHIỆP VỤ ========================

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
