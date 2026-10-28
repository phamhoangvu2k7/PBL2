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

    bool registerAccount(const string& username, const string& password,
                         const string& role, const string& fullName = "",
                         const string& phone = "", const string& email = "",
                         string* outError = nullptr);

    // cap ma SV/GS cho tai khoan chua co
    int assignMissingPersonIds();

    // dang nhap
    bool login(const string& username, const string& password);

    // dang xuat
    void logout();

    // trang thai dang nhap
    bool isLoggedIn() const;
    Account* getCurrentUser() const;
    string getCurrentRole() const;
    string getCurrentPersonId() const;
    string getCurrentDisplayName() const;

    bool isStudent() const;
    bool isTutor() const;
    bool isAdmin() const;

    bool usernameExists(const string& username) const;

    // luu tai khoan ra file (goi sau khi sua thong tin tai khoan)
    bool saveToFile() const { return accountRepo.save(); }

    vector<Account*> getAccounts() const { return accountRepo.toVector(); }
};

#endif
