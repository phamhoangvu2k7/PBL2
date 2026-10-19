#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using namespace std;

class Validator {
public:
    // --- Kiểm tra Lịch học / Khung giờ ---
    // Kiểm tra giờ hợp lệ "HH:MM" với MM ∈ {00, 15, 30, 45}
    static bool isValidTime(const string& time);

    // Kiểm tra chuỗi lịch VD: "T2-T4 (18:00-20:00)"
    // Giờ bắt đầu < Giờ kết thúc, cả 2 phải chẵn phút
    static bool isValidSchedule(const string& schedule);

    // --- Kiểm tra Thông tin cá nhân ---
    static bool isValidPhone(const string& phone);       // 10 chữ số, bắt đầu bằng 0
    static bool isValidEmail(const string& email);        // Chứa @ và dấu .
    static bool isValidDate(const string& date);          // dd/mm/yyyy, ngày hợp lệ
    static bool isValidName(const string& name);          // Không rỗng, chỉ chữ cái và khoảng trắng
    static bool isValidGender(const string& gender);      // "Nam" hoặc "Nu"

    // --- Kiểm tra Dữ liệu nghiệp vụ ---
    static bool isValidFee(double fee);                   // fee > 0
    static bool isValidExpYears(int years);               // years >= 0
    static bool isNotEmpty(const string& str);            // Chuỗi không rỗng (sau khi trim)

    // --- Hàm trả về thông báo lỗi chi tiết ---
    static string getScheduleError(const string& schedule);
    static string getPhoneError(const string& phone);
    static string getEmailError(const string& email);
    static string getDateError(const string& date);
    static string getNameError(const string& name);

private:
    // Phụ trợ: tách giờ phút từ chuỗi "HH:MM"
    static bool parseTime(const string& time, int& hour, int& minute);
    // Phụ trợ: tách ngày/tháng/năm từ "dd/mm/yyyy"
    static bool parseDate(const string& date, int& day, int& month, int& year);
    // Phụ trợ: kiểm tra năm nhuận
    static bool isLeapYear(int year);
    // Phụ trợ: số ngày tối đa trong tháng
    static int daysInMonth(int month, int year);
};

#endif
