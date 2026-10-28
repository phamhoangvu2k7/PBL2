#include "../../include/entities/Schedule.h"
#include "../../include/helper/StringUtils.h"
#include <cctype>

namespace {

const char* DAY_CODES[Schedule::DAY_COUNT] = {"T2", "T3", "T4", "T5", "T6", "T7", "CN"};
const char* DAY_NAMES[Schedule::DAY_COUNT] = {"Thu 2", "Thu 3", "Thu 4", "Thu 5",
                                              "Thu 6", "Thu 7", "Chu Nhat"};
const unsigned ALL_DAYS_MASK = (1u << Schedule::DAY_COUNT) - 1;
const int MAX_MINUTE = 23 * 60 + 45;

// doi "T2,T4" hoac "T2-T4" thanh mask ngay
bool parseDays(const string& text, unsigned& mask, string* err) {
    string t = StringUtils::trim(text);
    if (t.empty()) {
        StringUtils::assign(err, "Chua chon thu nao trong khung gio!");
        return false;
    }
    mask = 0;

    // dang cu: T2-T4
    size_t dash = t.find('-');
    if (dash != string::npos) {
        int from = Schedule::dayFromCode(t.substr(0, dash));
        int to = Schedule::dayFromCode(t.substr(dash + 1));
        if (from < 0 || to < 0 || from > to) {
            StringUtils::assign(err, "Khoang thu '" + t + "' khong hop le (VD: T2-T4)!");
            return false;
        }
        for (int d = from; d <= to; d++) mask |= (1u << d);
        return true;
    }

    size_t pos = 0;
    while (pos <= t.size()) {
        size_t comma = t.find(',', pos);
        if (comma == string::npos) comma = t.size();
        string token = StringUtils::trim(t.substr(pos, comma - pos));
        int d = Schedule::dayFromCode(token);
        if (d < 0) {
            StringUtils::assign(err, "Thu '" + token + "' khong hop le (chi nhan T2..T7, CN)!");
            return false;
        }
        mask |= (1u << d);
        pos = comma + 1;
    }
    return true;
}

}  // namespace

// ngay / gio

string Schedule::dayCode(int day) {
    if (day < 0 || day >= DAY_COUNT) return "";
    return DAY_CODES[day];
}

string Schedule::dayLongName(int day) {
    if (day < 0 || day >= DAY_COUNT) return "";
    return DAY_NAMES[day];
}

int Schedule::dayFromCode(const string& code) {
    string c = StringUtils::toUpper(StringUtils::trim(code));
    for (int i = 0; i < DAY_COUNT; i++) {
        if (c == DAY_CODES[i]) return i;
    }
    return -1;
}

string Schedule::formatTime(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    string s;
    if (h < 10) s += '0';
    s += to_string(h);
    s += ':';
    if (m < 10) s += '0';
    s += to_string(m);
    return s;
}

bool Schedule::parseTime(const string& text, int& minutes) {
    string t = StringUtils::trim(text);
    size_t colon = t.find(':');
    if (colon == string::npos) return false;

    string hs = t.substr(0, colon);
    string ms = t.substr(colon + 1);
    if (hs.empty() || hs.size() > 2 || ms.size() != 2) return false;
    for (char c : hs) if (!isdigit((unsigned char)c)) return false;
    for (char c : ms) if (!isdigit((unsigned char)c)) return false;

    int h = stoi(hs);
    int m = stoi(ms);
    if (h > 23 || m > 59 || m % STEP_MINUTES != 0) return false;
    minutes = h * 60 + m;
    return true;
}

// thao tac

bool Schedule::addSlot(unsigned dayMask, int startMin, int endMin, string* err) {
    if (dayMask == 0 || (dayMask & ~ALL_DAYS_MASK) != 0) {
        StringUtils::assign(err, "Chua chon thu nao trong khung gio!");
        return false;
    }
    if (startMin < 0 || endMin < 0 || startMin > MAX_MINUTE || endMin > MAX_MINUTE ||
        startMin % STEP_MINUTES != 0 || endMin % STEP_MINUTES != 0) {
        StringUtils::assign(err, "Gio phai nam trong 00:00-23:45 va phut la 00, 15, 30 hoac 45!");
        return false;
    }
    if (endMin <= startMin) {
        StringUtils::assign(err, "Gio ket thuc phai lon hon gio bat dau!");
        return false;
    }

    ScheduleSlot candidate = {dayMask, startMin, endMin};
    for (const ScheduleSlot& s : entries) {
        if ((s.dayMask & dayMask) != 0 && startMin < s.endMin && s.startMin < endMin) {
            StringUtils::assign(err, "Khung gio " + formatTime(startMin) + "-" + formatTime(endMin) +
                        " bi trung voi khung " + formatTime(s.startMin) + "-" +
                        formatTime(s.endMin) + " da them!");
            return false;
        }
    }
    entries.push_back(candidate);
    return true;
}

bool Schedule::removeSlot(int index) {
    if (index < 0 || index >= (int)entries.size()) return false;
    entries.erase(entries.begin() + index);
    return true;
}

void Schedule::clear() { entries.clear(); }

int Schedule::size() const { return (int)entries.size(); }
bool Schedule::empty() const { return entries.empty(); }
const ScheduleSlot& Schedule::slot(int index) const { return entries.at(index); }

bool Schedule::overlaps(const Schedule& other) const {
    for (const ScheduleSlot& a : entries) {
        for (const ScheduleSlot& b : other.entries) {
            if ((a.dayMask & b.dayMask) != 0 && a.startMin < b.endMin && b.startMin < a.endMin) {
                return true;
            }
        }
    }
    return false;
}

// doi chuoi

string Schedule::toString() const {
    string out;
    for (size_t i = 0; i < entries.size(); i++) {
        if (i > 0) out += "; ";
        bool first = true;
        for (int d = 0; d < DAY_COUNT; d++) {
            if (entries[i].dayMask & (1u << d)) {
                if (!first) out += ",";
                out += DAY_CODES[d];
                first = false;
            }
        }
        out += " (" + formatTime(entries[i].startMin) + "-" + formatTime(entries[i].endMin) + ")";
    }
    return out;
}

string Schedule::toDisplayString() const {
    string out;
    for (size_t i = 0; i < entries.size(); i++) {
        if (i > 0) out += "; ";
        bool first = true;
        for (int d = 0; d < DAY_COUNT; d++) {
            if (entries[i].dayMask & (1u << d)) {
                if (!first) out += ", ";
                out += DAY_NAMES[d];
                first = false;
            }
        }
        out += ": " + formatTime(entries[i].startMin) + " - " + formatTime(entries[i].endMin);
    }
    return out;
}

bool Schedule::parse(const string& text, Schedule& out, string* err) {
    out.clear();
    if (StringUtils::trim(text).empty()) {
        StringUtils::assign(err, "Lich hoc khong duoc de trong!");
        return false;
    }

    size_t pos = 0;
    while (pos <= text.size()) {
        size_t semi = text.find(';', pos);
        if (semi == string::npos) semi = text.size();
        string part = StringUtils::trim(text.substr(pos, semi - pos));
        pos = semi + 1;
        if (part.empty()) {
            StringUtils::assign(err, "Co khung gio rong trong lich hoc!");
            out.clear();
            return false;
        }

        size_t open = part.find('(');
        size_t close = part.find(')');
        if (open == string::npos || close == string::npos || close != part.size() - 1 ||
            close <= open + 1) {
            StringUtils::assign(err, "Dinh dang lich hoc sai! VD dung: T2,T4 (19:00-21:00); T6 (08:00-10:00)");
            out.clear();
            return false;
        }

        unsigned mask = 0;
        if (!parseDays(part.substr(0, open), mask, err)) {
            out.clear();
            return false;
        }

        string range = part.substr(open + 1, close - open - 1);
        size_t dash = range.find('-');
        if (dash == string::npos) {
            StringUtils::assign(err, "Thieu dau '-' giua gio bat dau va gio ket thuc!");
            out.clear();
            return false;
        }
        int startMin = 0, endMin = 0;
        string startText = StringUtils::trim(range.substr(0, dash));
        string endText = StringUtils::trim(range.substr(dash + 1));
        if (!parseTime(startText, startMin)) {
            StringUtils::assign(err, "Gio bat dau '" + startText + "' khong hop le!\n"
                        "Phut phai la 00, 15, 30 hoac 45 (VD: 18:00, 19:15, 20:30)");
            out.clear();
            return false;
        }
        if (!parseTime(endText, endMin)) {
            StringUtils::assign(err, "Gio ket thuc '" + endText + "' khong hop le!\n"
                        "Phut phai la 00, 15, 30 hoac 45 (VD: 18:00, 19:15, 20:30)");
            out.clear();
            return false;
        }
        if (!out.addSlot(mask, startMin, endMin, err)) {
            out.clear();
            return false;
        }
    }
    return true;
}
