#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using namespace std;

class Validator {
public:
    // lich hoc
    // gio HH:MM, phut 00 / 15 / 30 / 45
    static bool isValidTime(const string& time);


    static bool isValidSchedule(const string& schedule);

    static bool isValidPhone(const string& phone); 
    static bool isValidEmail(const string& email);  
    static bool isValidDate(const string& date);  
    static bool isValidName(const string& name);  
    static bool isValidGender(const string& gender);

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
