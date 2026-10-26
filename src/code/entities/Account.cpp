#include "../../include/entities/Account.h"
#include <iostream>
using namespace std;

Account::Account() : accountId(""), username(""), passwordHash(""), personId(""), role("") {}

Account::Account(const string& accountId, const string& username,
                 const string& passwordHash, const string& personId,
                 const string& role)
    : accountId(accountId), username(username), passwordHash(passwordHash),
      personId(personId), role(role) {}

string Account::getAccountId()   const { return accountId; }
string Account::getUsername()    const { return username; }
string Account::getPasswordHash() const { return passwordHash; }
string Account::getPersonId()    const { return personId; }
string Account::getRole()        const { return role; }

void Account::setAccountId(const string& id)    { this->accountId = id; }
void Account::setUsername(const string& uname)  { this->username = uname; }
void Account::setPasswordHash(const string& h)  { this->passwordHash = h; }
void Account::setPersonId(const string& pid)    { this->personId = pid; }
void Account::setRole(const string& r)          { this->role = r; }

void Account::display() const {
    cout << "[Account] Ma: " << accountId
         << " | Username: " << username
         << " | PersonId: " << personId
         << " | Role: " << role << "\n";
}
