#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
using namespace std;

class Account {
private:
    string accountId;
    string username;  // ten dang nhap
    string passwordHash;  // mat khau da bam
    string personId;  // ma SV / GS / ADMIN
    string role;
    string fullName; 
    string phone;  
    string email;

public:
    Account();
    Account(const string& accountId, const string& username,
            const string& passwordHash, const string& personId,
            const string& role, const string& fullName = "",
            const string& phone = "", const string& email = "");

    string getAccountId() const;
    string getUsername() const;
    string getPasswordHash() const;
    string getPersonId() const;
    string getRole() const;
    string getFullName() const;
    string getPhone() const;
    string getEmail() const;

    // ten hien thi, thieu thi dung username
    string getDisplayName() const;

    void setAccountId(const string& id);
    void setUsername(const string& uname);
    void setPasswordHash(const string& hash);
    void setPersonId(const string& pid);
    void setRole(const string& r);
    void setFullName(const string& name);
    void setPhone(const string& phone);
    void setEmail(const string& email);

    void display() const;
};

#endif
