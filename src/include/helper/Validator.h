#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using namespace std;

class Validator {
public:
    static bool sdt_hop_le(const string& phone); 
    static bool email_hop_le(const string& email);  
    static bool ngay_hop_le(const string& date);  
    static bool ten_hop_le(const string& name);  
    static bool gioi_tinh_hop_le(const string& gender);

    static bool hoc_phi_hop_le(double fee);                   // fee > 0
    static bool kinh_nghiem_hop_le(int years);               // years >= 0
    static bool khong_rong(const string& str);  // khong rong sau khi trim

    // so sanh 2 ngay hop le dd/mm/yyyy: -1 (a truoc b), 0 (bang), 1 (a sau b)
    static int so_sanh_ngay(const string& a, const string& b);
    // ngay hien tai dd/mm/yyyy
    static string hom_nay();

    // thong bao loi
    static string loi_sdt(const string& phone);
    static string loi_email(const string& email);
    static string loi_ngay(const string& date);

private:
    // tach ngay thang nam
    static bool tach_ngay(const string& date, int& day, int& month, int& year);
    // nam nhuan
    static bool nam_nhuan(int year);
    // so ngay trong thang
    static int so_ngay_trong_thang(int month, int year);
};

#endif
