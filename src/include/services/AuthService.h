#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <string>
#include <vector>
#include "../entities/Account.h"
#include "../repositories/AccountRepository.h"
using namespace std;

class AuthService {
private:
    AccountRepository& accountRepo; 
    Account* currentUser;     

public:
    explicit AuthService(AccountRepository& accounts);

    // cap ma SV / GS ngay khi tao, tra ma qua outPersonId
    bool registerAccount(const string& username, const string& password,
                         const string& role, const string& fullName = "",
                         const string& phone = "", const string& email = "",
                         string* outError = nullptr, string* outPersonId = nullptr);

    bool login(const string& username, const string& password);

    void logout();

    // trang thai dang nhap
    bool da_dang_nhap() const;
    Account* tai_khoan_hien_tai() const;
    string vai_tro_hien_tai() const;
    string ma_nguoi_hien_tai() const;
    string ten_hien_tai() const;

    bool la_hoc_vien() const;
    bool la_gia_su() const;
    bool la_admin() const;

    bool username_ton_tai(const string& username) const;

    // xoa tai khoan theo ma SV / GS
    vector<string> tai_khoan_cua(const string& personId) const;  // username
    // khong xoa Admin va nguoi dang dang nhap
    bool co_the_xoa_tai_khoan(const string& personId, string* outError = nullptr) const;
    bool xoa_tai_khoan_cua(const string& personId, string* outError = nullptr);

    // luu tai khoan ra file (goi sau khi sua thong tin tai khoan)
    bool luu_file() const { return accountRepo.save(); }

    vector<Account*> ds_tai_khoan() const { return accountRepo.toVector(); }
};

#endif
