#include "../../include/repositories/AccountRepository.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

string AccountRepository::idOf(const Account& a) const { return a.getAccountId(); }

vector<string> AccountRepository::toRecord(const Account& a) const {
    return {a.getAccountId(), a.getUsername(), a.getPasswordHash(), a.getPersonId(), a.getRole(),
            a.getFullName(), a.getPhone(), a.getEmail()};
}

Account* AccountRepository::fromRecord(const vector<string>& r) const {
    if (r.size() < 5 || r[0].empty() || r[1].empty() || r[2].empty()) return nullptr;
    string role = StringUtils::trim(r[4]);
    if (role != "Student" && role != "Tutor" && role != "Admin") return nullptr;

    IdGenerator::syncFromExistingId(r[0]);
    IdGenerator::syncFromExistingId(r[3]);
    return new Account(r[0], r[1], r[2], r[3], role,
                       r.size() > 5 ? r[5] : "", r.size() > 6 ? r[6] : "", r.size() > 7 ? r[7] : "");
}

bool AccountRepository::onAdd(Account* a) {
    // trung username
    if (usernameIndex.containsKey(a->getUsername())) return false;
    usernameIndex.insert(a->getUsername(), a);
    return true;
}

Account* AccountRepository::findByUsername(const string& username) const {
    Account* result = nullptr;
    usernameIndex.find(username, result);
    return result;
}
