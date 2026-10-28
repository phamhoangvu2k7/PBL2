#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H

#include "Repository.h"
#include "../entities/Account.h"

class AccountRepository : public Repository<Account> {
public:
    explicit AccountRepository(const string& file = "") : Repository<Account>(file) {}

    Account* findByUsername(const string& username) const;

protected:
    string idOf(const Account& item) const override;
    vector<string> toRecord(const Account& item) const override;
    Account* fromRecord(const vector<string>& r) const override;
    bool onAdd(Account* item) override;

private:
    HashTable<string, Account*> usernameIndex;
};

#endif
