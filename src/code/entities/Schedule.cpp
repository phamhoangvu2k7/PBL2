#include "../../include/entities/Schedule.h"
#include "../../include/helper/StringUtils.h"
#include <cctype>
#include <cstdio>

namespace {

const char* DAY_CODES[Schedule::DAY_COUNT] = {"T2", "T3", "T4", "T5", "T6", "T7", "CN"};
const char* DAY_NAMES[Schedule::DAY_COUNT] = {"Thu 2", "Thu 3", "Thu 4", "Thu 5", "Thu 6", "Thu 7", "Chu Nhat"};
const unsigned ALL_DAYS_MASK = (1u << Schedule::DAY_COUNT) - 1; 
const int MAX_MINUTE = 23 * 60 + 45;                           

// check trung 
bool hai_khung_trung(const ScheduleSlot& a, const ScheduleSlot& b) {
    return (a.dayMask & b.dayMask) != 0 && a.startMin < b.endMin && b.startMin < a.endMin;
}

// "T2,T4,T6" -> mask
bool doc_cac_thu(const string& text, unsigned& mask, string* err) {
    mask = 0;
    if (StringUtils::trim(text).empty()) {//text rong
        StringUtils::gan(err, "Chua chon thu nao trong khung gio!");
        return false;
    }
    size_t pos = 0;
    while (pos <= text.size()) {
        size_t comma = text.find(',', pos);
        if (comma == string::npos) comma = text.size();

        string code = text.substr(pos, comma - pos);
        int day = Schedule::thu_tu_ma(code);

        if (day < 0) {
            StringUtils::gan(err, "Thu '" + StringUtils::trim(code) + "' khong hop le (chi nhan T2..T7, CN)!");
            return false;
        }
        mask |= (1u << day); //bat bit thu day
        pos = comma + 1;
    }
    return true;
}

//mask -> T1,T2
string noi_cac_thu(unsigned mask, const char* const names[], const string& sep) {
    string out;
    for (int d = 0; d < Schedule::DAY_COUNT; d++) {
        if (mask & (1u << d)) {
            if (!out.empty()) out += sep;
            out += names[d];
        }
    }
    return out;
}

} 

// ngay / gio

string Schedule::ma_thu(int day) {
    return (day >= 0 && day < DAY_COUNT) ? DAY_CODES[day] : "";
}

int Schedule::thu_tu_ma(const string& code) {
    string c = StringUtils::toUpper(StringUtils::trim(code));
    for (int d = 0; d < DAY_COUNT; d++) {
        if (c == DAY_CODES[d]) return d;
    }
    return -1;
}

// 1110 -> "18:30"
string Schedule::gio_thanh_chuoi(int minutes) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", minutes / 60, minutes % 60);
    return buf;
}

// "18:30" -> 1110;
bool Schedule::doc_gio(const string& text, int& minutes) {
    string t = StringUtils::trim(text);
    size_t colon = t.find(':');
    if (colon == string::npos || colon == 0 || colon > 2 || t.size() != colon + 3) return false;
    for (size_t i = 0; i < t.size(); i++) {
        if (i != colon && !isdigit((unsigned char)t[i])) return false;
    }
    int h = stoi(t.substr(0, colon));
    int m = stoi(t.substr(colon + 1));
    if (h > 23 || m > 59 || m % STEP_MINUTES != 0) return false;
    minutes = h * 60 + m;
    return true;
}

// thao tac

bool Schedule::them_khung(unsigned dayMask, int startMin, int endMin, string* err) {

    if (dayMask == 0 || (dayMask & ~ALL_DAYS_MASK) != 0) {
        StringUtils::gan(err, "Chua chon thu nao trong khung gio!");
        return false;
    }
    bool validTime = startMin >= 0 && endMin >= 0 && startMin <= MAX_MINUTE && endMin <= MAX_MINUTE &&
                     startMin % STEP_MINUTES == 0 && endMin % STEP_MINUTES == 0;
    if (!validTime) {
        StringUtils::gan(err, "Gio phai nam trong 00:00-23:45 va phut la 00, 15, 30 hoac 45!");
        return false;
    }
    if (endMin <= startMin) {
        StringUtils::gan(err, "Gio ket thuc phai lon hon gio bat dau!");
        return false;
    }

    ScheduleSlot slot = {dayMask, startMin, endMin};
    for (const ScheduleSlot& s : entries) {
        if (hai_khung_trung(slot, s)){
            StringUtils::gan(err, "Khung gio " + gio_thanh_chuoi(startMin) + "-" + gio_thanh_chuoi(endMin) +
                                     " bi trung voi khung " + gio_thanh_chuoi(s.startMin) + "-" +
                                     gio_thanh_chuoi(s.endMin) + " da them!");
            return false;
        }
    }
    entries.push_back(slot);
    return true;
}

bool Schedule::xoa_khung(int index) {
    if (index < 0 || index >= so_khung()) return false;
    entries.erase(entries.begin() + index);
    return true;
}

void Schedule::xoa_het() { entries.clear(); }
int Schedule::so_khung() const { return (int)entries.size(); }
bool Schedule::rong() const { return entries.empty(); }
const ScheduleSlot& Schedule::lay_khung(int index) const { return entries.at(index); }

//check trung lich lop khac
bool Schedule::trung_lich(const Schedule& other) const {
    for (const ScheduleSlot& a : entries) {
        for (const ScheduleSlot& b : other.entries) {
            if (hai_khung_trung(a, b)) return true;
        }
    }
    return false;
}


// "T2,T4 (18:00-20:00); CN (08:00-10:00)"
string Schedule::chuoi_luu() const {
    string out;
    for (const ScheduleSlot& s : entries) {
        if (!out.empty()) out += "; ";
        out += noi_cac_thu(s.dayMask, DAY_CODES, ",") + " (" + gio_thanh_chuoi(s.startMin) + "-" + gio_thanh_chuoi(s.endMin) + ")";
    }
    return out;
}

// "Thu 2, Thu 4: 18:00 - 20:00; Chu Nhat: 08:00 - 10:00"
string Schedule::chuoi_hien_thi() const {
    string out;
    for (const ScheduleSlot& s : entries) {
        if (!out.empty()) out += "; ";
        out += noi_cac_thu(s.dayMask, DAY_NAMES, ", ") + ": " + gio_thanh_chuoi(s.startMin) + " - " + gio_thanh_chuoi(s.endMin);
    }
    return out;
}

// moi khung "<cac thu> (<bat dau>-<ket thuc>)", cac khung cach nhau boi ';'
bool Schedule::doc_lich(const string& text, Schedule& out, string* err) {
    out.xoa_het();
    if (StringUtils::trim(text).empty()) {
        StringUtils::gan(err, "Lich hoc khong duoc de trong!");
        return false;
    }

    size_t pos = 0;
    while (pos <= text.size()) {
        size_t semi = text.find(';', pos);
        if (semi == string::npos) semi = text.size();
        string part = StringUtils::trim(text.substr(pos, semi - pos));
        pos = semi + 1;

        string error;
        unsigned mask = 0;
        int startMin = 0, endMin = 0;
        size_t open = part.find('(');
        size_t dash = part.find('-', open == string::npos ? 0 : open);
        size_t close = part.size() - 1;

        if (part.empty()) {
            error = "Co khung gio rong trong lich hoc!";
        }
        else if (open == string::npos || part[close] != ')' || dash == string::npos || dash > close) {
            error = "Dinh dang lich hoc sai! VD dung: T2,T4 (19:00-21:00); T6 (08:00-10:00)";
        }
        else if (!doc_cac_thu(part.substr(0, open), mask, &error)) {
            // error da duoc ghi
        }
        else if (!doc_gio(part.substr(open + 1, dash - open - 1), startMin) ||
                   !doc_gio(part.substr(dash + 1, close - dash - 1), endMin)) {
            error = "Gio '" + part.substr(open + 1, close - open - 1) + "' khong hop le!\n"
                    "Phut phai la 00, 15, 30 hoac 45 (VD: 18:00-20:30)";
        }
        else {
            out.them_khung(mask, startMin, endMin, &error);
        }

        if (!error.empty()) {
            StringUtils::gan(err, error);
            out.xoa_het();
            return false;
        }
    }
    return true;
}
