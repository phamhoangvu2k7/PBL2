#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <vector>
using namespace std;

// 1 khung gio: nhieu thu dung chung 1 cap gio
// dayMask: bit0 = T2 ... bit5 = T7, bit6 = CN
struct ScheduleSlot {
    unsigned dayMask;
    int startMin; 
    int endMin;
};

// lich hoc gom nhieu khung gio
class Schedule {
private:
    vector<ScheduleSlot> entries;

public:
    static constexpr int DAY_COUNT = 7;
    static constexpr int STEP_MINUTES = 15;

    // ngay / gio
    static string dayCode(int day);
    static string dayLongName(int day);
    static int dayFromCode(const string& code);  // -1 neu sai
    static string formatTime(int minutes);
    static bool parseTime(const string& text, int& minutes);  // phut phai chia het cho 15

    // thao tac
    // them 1 khung, false neu chua chon thu, gio sai,
    // bat dau >= ket thuc hoac trung khung da co
    bool addSlot(unsigned dayMask, int startMin, int endMin, string* err = nullptr);
    bool removeSlot(int index);
    void clear();

    int size() const;
    bool empty() const;
    const ScheduleSlot& slot(int index) const;

    // trung lich: chung 1 thu va gio giao nhau
    bool overlaps(const Schedule& other) const;

    // doi chuoi
    string toString() const;  // dang luu
    string toDisplayString() const;  // dang hien thi

    static bool parse(const string& text, Schedule& out, string* err = nullptr);
};

#endif
