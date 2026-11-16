#include "../../include/services/AuthService.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/Validator.h"
#include "../../include/helper/StringUtils.h"
#include "../../include/helper/PasswordHasher.h"
#include <iostream>
using namespace std;


bool bao_loi(string* outError, const string& msg) {
    cout << "[Loi] " << msg << "\n";
    StringUtils::gan(outError, msg);
    return false;
}

AuthService::AuthService(AccountRepository& accounts)
    : accountRepo(accounts), currentUser(nullptr) {
    // chua co admin thi tao mac dinh
    if (accountRepo.findByUsername("admin") == nullptr) {
        string adminId  = IdGenerator::generateAccountId();
        string passHash = PasswordHasher::hash("admin123");
        accountRepo.add(new Account(adminId, "admin", passHash, "ADMIN", "Admin", "Quan tri vien"));
    }
}

// dang ky

bool AuthService::registerAccount(const string& username, const string& password,
                                  const string& role, const string& fullNameIn,
                                  const string& phoneIn, const string& emailIn,
                                  string* outError, string* outPersonId) {
    string fullName = StringUtils::trim(fullNameIn);
    string phone    = StringUtils::trim(phoneIn);
    string email    = StringUtils::trim(emailIn);

    // kiem tra dau vao
    if (username.empty() || password.empty()) {
        return bao_loi(outError, "Username va mat khau khong duoc de trong!");
    }
    if (StringUtils::co_khoang_trang(username) || StringUtils::co_ky_tu(username, "|\r\n")) {
        return bao_loi(outError, "Ten dang nhap khong duoc chua khoang trang hoac ky tu '|'!");
    }
    if (accountRepo.findByUsername(username) != nullptr) {
        return bao_loi(outError, "Username '" + username + "' da ton tai!");
    }
    if (password.length() < 6) {
        return bao_loi(outError, "Mat khau phai co it nhat 6 ky tu!");
    }
    if (role != "Student" && role != "Tutor" && role != "Admin") {
        return bao_loi(outError, "Vai tro khong hop le!");
    }

    // Student/Tutor bat buoc co ten, sdt, email
    if (role != "Admin") {
        if (!Validator::khong_rong(fullName)) {
            return bao_loi(outError, "Ho va ten khong duoc de trong!");
        }
        if (!Validator::ten_hop_le(fullName) || StringUtils::co_ky_tu(fullName, "|\r\n")) {
            return bao_loi(outError, "Ho va ten chi duoc chua chu cai va khoang trang!");
        }
        if (!Validator::sdt_hop_le(phone)) {
            return bao_loi(outError, Validator::loi_sdt(phone));
        }
        if (email.empty()) {
            return bao_loi(outError, "Email khong duoc de trong!");
        }
        if (!Validator::email_hop_le(email) || StringUtils::co_ky_tu(email, "|\r\n") || StringUtils::co_khoang_trang(email)) {
            string why = Validator::loi_email(email);
            return bao_loi(outError, why.empty() ? "Email khong hop le!" : why);
        }
    }

    string personId = (role == "Admin")   ? "ADMIN"
                    : (role == "Student") ? IdGenerator::generateStudentId()
                                          : IdGenerator::generateTutorId();

    string accId    = IdGenerator::generateAccountId();
    string passHash = PasswordHasher::hash(password);
    Account* newAcc = new Account(accId, username, passHash, personId, role,
                                  fullName, phone, email);

    if (!accountRepo.add(newAcc)) {
        delete newAcc;
        return bao_loi(outError, "Khong the tao tai khoan!");
    }

    StringUtils::gan(outPersonId, personId);
    return true;
}

// dang nhap

bool AuthService::login(const string& username, const string& password) {
    Account* acc = accountRepo.findByUsername(username);
    if (acc == nullptr) {
        return false;
    }

    if (!PasswordHasher::verify(password, acc->getPasswordHash())) {
        return false;
    }

    currentUser = acc;
    return true;
}

// dang xuat

void AuthService::logout() {
    currentUser = nullptr;
}

// trang thai dang nhap

bool    AuthService::da_dang_nhap()       const { return currentUser != nullptr; }
Account* AuthService::tai_khoan_hien_tai()  const { return currentUser; }

string AuthService::vai_tro_hien_tai() const {
    return currentUser ? currentUser->getRole() : "";
}

string AuthService::ma_nguoi_hien_tai() const {
    return currentUser ? currentUser->getPersonId() : "";
}

string AuthService::ten_hien_tai() const {
    return currentUser ? currentUser->getDisplayName() : "";
}

bool AuthService::la_admin()   const { return currentUser && currentUser->getRole() == "Admin"; }

bool AuthService::username_ton_tai(const string& username) const {
    return accountRepo.findByUsername(username) != nullptr;
}

// xoa tai khoan

vector<string> AuthService::tai_khoan_cua(const string& personId) const {
    vector<string> names;
    for (Account* a : accountRepo.toVector()) {
        if (!personId.empty() && a->getPersonId() == personId) names.push_back(a->getUsername());
    }
    return names;
}

bool AuthService::co_the_xoa_tai_khoan(const string& personId, string* outError) const {
    if (personId.empty() || personId == "ADMIN") {
        return bao_loi(outError, "Khong the xoa tai khoan Admin!");
    }
    for (Account* a : accountRepo.toVector()) {
        if (a->getPersonId() != personId) continue;
        if (a->getRole() == "Admin") return bao_loi(outError, "Khong the xoa tai khoan Admin!");
        if (a == currentUser) return bao_loi(outError, "Khong the xoa tai khoan dang dang nhap!");
    }
    return true;
}

bool AuthService::xoa_tai_khoan_cua(const string& personId, string* outError) {
    // kiem tra het truoc, bi chan thi chua xoa gi
    if (!co_the_xoa_tai_khoan(personId, outError)) return false;
    for (Account* a : accountRepo.toVector()) {
        if (a->getPersonId() == personId) accountRepo.remove(a->getAccountId());
    }
    return true;
}
