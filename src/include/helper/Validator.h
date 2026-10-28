#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using namespace std;

class Validator {
public:
    // lich hoc
    // gio HH:MM, phut 00 / 15 / 30 / 45
    static bool isValidTime(const string& time);

    // lich nhieu khung, VD "T2,T4 (19:00-21:00); T6 (08:00-10:00)"
    // dang cu "T2-T4 (18:00-20:00)" van hop le, xu ly boi Schedule
    // bat dau < ket thuc, cac khung khong trung nhau
    static bool isValidSchedule(const string& schedule);

    // thong tin ca nhan
    static bool isValidPhone(const string& phone);  // 10 so, dau 0
    static bool isValidEmail(const string& email);  // co @ va dau .
    static bool isValidDate(const string& date);  // dd/mm/yyyy
    static bool isValidName(const string& name);  // chi chu cai va dau cach
    static bool isValidGender(const string& gender);  // Nam hoac Nu

    // du lieu nghiep vu
    static bool isValidFee(double fee);                   // fee > 0
    static bool isValidExpYears(int years);               // years >= 0
    static bool isNotEmpty(const string& str);  // khong rong sau khi trim

    // so sanh 2 ngay hop le dd/mm/yyyy: -1 (a truoc b), 0 (bang), 1 (a sau b)
    static int compareDates(const string& a, const string& b);

    // thong bao loi
    static string getScheduleError(const string& schedule);
    static string getPhoneError(const string& phone);
    static string getEmailError(const string& email);
    static string getDateError(const string& date);
    static string getNameError(const string& name);

private:
    // tach gio phut
    static bool parseTime(const string& time, int& hour, int& minute);
    // tach ngay thang nam
    static bool parseDate(const string& date, int& day, int& month, int& year);
    // nam nhuan
    static bool isLeapYear(int year);
    // so ngay trong thang
    static int daysInMonth(int month, int year);
};

#endif
