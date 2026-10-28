#include "../../include/entities/Account.h"
#include <iostream>
using namespace std;

Account::Account() : accountId(""), username(""), passwordHash(""), personId(""), role("") {}

Account::Account(const string& accountId, const string& username,
                 const string& passwordHash, const string& personId,
                 const string& role, const string& fullName,
                 const string& phone, const string& email)
    : accountId(accountId), username(username), passwordHash(passwordHash),
      personId(personId), role(role), fullName(fullName), phone(phone), email(email) {}

string Account::getAccountId()   const { return accountId; }
string Account::getUsername()    const { return username; }
string Account::getPasswordHash() const { return passwordHash; }
string Account::getPersonId()    const { return personId; }
string Account::getRole()        const { return role; }
string Account::getFullName()    const { return fullName; }
string Account::getPhone()       const { return phone; }
string Account::getEmail()       const { return email; }

string Account::getDisplayName() const { return fullName.empty() ? username : fullName; }

void Account::setAccountId(const string& id)    { this->accountId = id; }
void Account::setUsername(const string& uname)  { this->username = uname; }
void Account::setPasswordHash(const string& h)  { this->passwordHash = h; }
void Account::setPersonId(const string& pid)    { this->personId = pid; }
void Account::setRole(const string& r)          { this->role = r; }
void Account::setFullName(const string& name)   { this->fullName = name; }
void Account::setPhone(const string& p)         { this->phone = p; }
void Account::setEmail(const string& e)         { this->email = e; }

void Account::display() const {
    cout << "[Account] Ma: " << accountId
         << " | Username: " << username
         << " | Ten: " << getDisplayName()
         << " | PersonId: " << personId
         << " | Role: " << role << "\n";
}
