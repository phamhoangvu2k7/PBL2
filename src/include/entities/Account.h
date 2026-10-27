#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
using namespace std;

class Account {
private:
    string accountId;      // "AC001"
    string username;       // Tên đăng nhập duy nhất
    string passwordHash;   // Mật khẩu sau khi băm một chiều
    string personId;       // Liên kết tới Student/Tutor/Admin (VD: "SV001", "GS002")
    string role;           // "Student", "Tutor", "Admin"

public:
    Account();
    Account(const string& accountId, const string& username,
            const string& passwordHash, const string& personId,
            const string& role);

    string getAccountId() const;
    string getUsername() const;
    string getPasswordHash() const;
    string getPersonId() const;
    string getRole() const;

    void setAccountId(const string& id);
    void setUsername(const string& uname);
    void setPasswordHash(const string& hash);
    void setPersonId(const string& pid);
    void setRole(const string& r);

    void display() const;
};

#endif
