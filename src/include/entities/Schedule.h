#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
#include <vector>
using namespace std;

// khung gio
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
    static string ma_thu(int day);
    static int thu_tu_ma(const string& code); 
    static string gio_thanh_chuoi(int minutes);
    static bool doc_gio(const string& text, int& minutes);  // phut phai chia het cho 15

    // thao tac
    bool them_khung(unsigned dayMask, int startMin, int endMin, string* err = nullptr);
    bool xoa_khung(int index);
    void xoa_het();

    int so_khung() const;
    bool rong() const;
    const ScheduleSlot& lay_khung(int index) const;

    // trung lich: chung 1 thu va gio giao nhau
    bool trung_lich(const Schedule& other) const;

    string chuoi_luu() const;  
    string chuoi_hien_thi() const;  

    static bool doc_lich(const string& text, Schedule& out, string* err = nullptr);
};

#endif
