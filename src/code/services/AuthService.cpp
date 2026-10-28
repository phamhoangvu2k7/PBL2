#include "../../include/services/AuthService.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/Validator.h"
#include "../../include/helper/StringUtils.h"
#include "../../include/helper/PasswordHasher.h"
#include <iostream>
using namespace std;

namespace {

bool fail(string* outError, const string& msg) {
    cout << "[Loi] " << msg << "\n";
    StringUtils::assign(outError, msg);
    return false;
}

}  // namespace


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
                                  string* outError) {
    string fullName = StringUtils::trim(fullNameIn);
    string phone    = StringUtils::trim(phoneIn);
    string email    = StringUtils::trim(emailIn);

    // kiem tra dau vao
    if (username.empty() || password.empty()) {
        return fail(outError, "Username va mat khau khong duoc de trong!");
    }
    if (StringUtils::hasWhitespace(username) || StringUtils::containsAny(username, "|\r\n")) {
        return fail(outError, "Ten dang nhap khong duoc chua khoang trang hoac ky tu '|'!");
    }
    if (accountRepo.findByUsername(username) != nullptr) {
        return fail(outError, "Username '" + username + "' da ton tai!");
    }
    if (password.length() < 6) {
        return fail(outError, "Mat khau phai co it nhat 6 ky tu!");
    }
    if (role != "Student" && role != "Tutor" && role != "Admin") {
        return fail(outError, "Vai tro khong hop le!");
    }

    // Student/Tutor bat buoc co ten, sdt, email
    if (role != "Admin") {
        if (!Validator::isNotEmpty(fullName)) {
            return fail(outError, "Ho va ten khong duoc de trong!");
        }
        if (!Validator::isValidName(fullName) || StringUtils::containsAny(fullName, "|\r\n")) {
            return fail(outError, "Ho va ten chi duoc chua chu cai va khoang trang!");
        }
        if (!Validator::isValidPhone(phone)) {
            return fail(outError, Validator::getPhoneError(phone));
        }
        if (email.empty()) {
            return fail(outError, "Email khong duoc de trong!");
        }
        if (!Validator::isValidEmail(email) || StringUtils::containsAny(email, "|\r\n") || StringUtils::hasWhitespace(email)) {
            string why = Validator::getEmailError(email);
            return fail(outError, why.empty() ? "Email khong hop le!" : why);
        }
    }

    // ma SV/GS se cap sau trong assignMissingPersonIds()
    string personId = (role == "Admin") ? "ADMIN" : "";

    string accId    = IdGenerator::generateAccountId();
    string passHash = PasswordHasher::hash(password);
    Account* newAcc = new Account(accId, username, passHash, personId, role,
                                  fullName, phone, email);

    if (!accountRepo.add(newAcc)) {
        delete newAcc;
        return fail(outError, "Khong the tao tai khoan!");
    }

    cout << "[Thanh Cong] Tai khoan '" << username << "' (Role: " << role << ") da duoc tao!\n";
    return true;
}

int AuthService::assignMissingPersonIds() {
    int count = 0;
    Node<Account*>* cur = accountRepo.getAll().getHead();
    while (cur != nullptr) {
        Account* a = cur->data;
        if (a->getPersonId().empty()) {
            if (a->getRole() == "Student") {
                a->setPersonId(IdGenerator::generateStudentId());
                count++;
            } else if (a->getRole() == "Tutor") {
                a->setPersonId(IdGenerator::generateTutorId());
                count++;
            }
        }
        cur = cur->next;
    }
    if (count > 0) accountRepo.save();
    return count;
}

// dang nhap

bool AuthService::login(const string& username, const string& password) {
    Account* acc = accountRepo.findByUsername(username);
    if (acc == nullptr) {
        cout << "[Loi] Tai khoan '" << username << "' khong ton tai!\n";
        return false;
    }

    string inputHash = PasswordHasher::hash(password);
    if (acc->getPasswordHash() != inputHash) {
        cout << "[Loi] Mat khau khong chinh xac!\n";
        return false;
    }

    currentUser = acc;
    cout << "[OK] Dang nhap: " << username << " (Role: " << acc->getRole() << ")\n";
    return true;
}

// dang xuat

void AuthService::logout() {
    currentUser = nullptr;
}

// trang thai dang nhap

bool    AuthService::isLoggedIn()       const { return currentUser != nullptr; }
Account* AuthService::getCurrentUser()  const { return currentUser; }

string AuthService::getCurrentRole() const {
    return currentUser ? currentUser->getRole() : "";
}

string AuthService::getCurrentPersonId() const {
    return currentUser ? currentUser->getPersonId() : "";
}

string AuthService::getCurrentDisplayName() const {
    return currentUser ? currentUser->getDisplayName() : "";
}

bool AuthService::isStudent() const { return currentUser && currentUser->getRole() == "Student"; }
bool AuthService::isTutor()   const { return currentUser && currentUser->getRole() == "Tutor"; }
bool AuthService::isAdmin()   const { return currentUser && currentUser->getRole() == "Admin"; }

bool AuthService::usernameExists(const string& username) const {
    return accountRepo.findByUsername(username) != nullptr;
}
