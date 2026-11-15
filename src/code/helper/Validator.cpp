#include "../../include/helper/Validator.h"
#include <cctype>
#include <ctime>

// ham phu tro

bool Validator::tach_ngay(const string& date, int& day, int& month, int& year) {
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

bool Validator::nam_nhuan(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Validator::so_ngay_trong_thang(int month, int year) {
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && nam_nhuan(year)) return 29;
    if (month >= 1 && month <= 12) return days[month];
    return 0;
}

// kiem tra thong tin ca nhan

bool Validator::sdt_hop_le(const string& phone) {
    if (phone.length() != 10) return false;
    if (phone[0] != '0') return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string Validator::loi_sdt(const string& phone) {
    if (phone.empty()) return "So dien thoai khong duoc de trong!";
    if (phone.length() != 10) return "So dien thoai phai co dung 10 chu so!";
    if (phone[0] != '0') return "So dien thoai phai bat dau bang so 0!";
    for (char c : phone) {
        if (!isdigit(c)) return "So dien thoai chi duoc chua chu so!";
    }
    return "";
}

bool Validator::email_hop_le(const string& email) {
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

string Validator::loi_email(const string& email) {
    if (email.empty()) return "";  // email co the de trong
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return "Email phai chua ky tu '@'!";
    size_t dotPos = email.find('.', atPos + 1);
    if (dotPos == string::npos || dotPos == email.length() - 1) {
        return "Email phai chua dau '.' sau ky tu '@'!";
    }
    return "";
}

bool Validator::ngay_hop_le(const string& date) {
    int d, m, y;
    if (!tach_ngay(date, d, m, y)) return false;
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > so_ngay_trong_thang(m, y)) return false;
    return true;
}

int Validator::so_sanh_ngay(const string& a, const string& b) {
    int d1 = 0, m1 = 0, y1 = 0, d2 = 0, m2 = 0, y2 = 0;
    if (!tach_ngay(a, d1, m1, y1) || !tach_ngay(b, d2, m2, y2)) return 0;
    int ka = y1 * 10000 + m1 * 100 + d1;
    int kb = y2 * 10000 + m2 * 100 + d2;
    return ka < kb ? -1 : (ka > kb ? 1 : 0);
}

string Validator::hom_nay() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%d/%m/%Y", t);
    return buf;
}

string Validator::loi_ngay(const string& date) {
    if (date.empty()) return "Ngay sinh khong duoc de trong!";
    int d, m, y;
    if (!tach_ngay(date, d, m, y)) {
        return "Dinh dang ngay sai! Dung dinh dang dd/mm/yyyy (VD: 15/03/2007)";
    }
    if (y < 1900 || y > 2100) return "Nam sinh khong hop le!";
    if (m < 1 || m > 12) return "Thang phai tu 1 den 12!";
    if (d < 1 || d > so_ngay_trong_thang(m, y)) {
        return "Ngay " + to_string(d) + " khong hop le cho thang " + to_string(m) + "!";
    }
    return "";
}

bool Validator::ten_hop_le(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ' && (unsigned char)c < 128) {
            // cho phep chu co dau (byte > 127) va dau cach
            return false;
        }
    }
    return true;
}

bool Validator::gioi_tinh_hop_le(const string& gender) {
    return gender == "Nam" || gender == "Nu";
}

// kiem tra du lieu nghiep vu

bool Validator::hoc_phi_hop_le(double fee) {
    return fee > 0;
}

bool Validator::kinh_nghiem_hop_le(int years) {
    return years >= 0;
}

bool Validator::khong_rong(const string& str) {
    for (char c : str) {
        if (c != ' ' && c != '\t' && c != '\n') return true;
    }
    return false;
}
