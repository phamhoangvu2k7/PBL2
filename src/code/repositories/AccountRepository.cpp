#include "../../include/repositories/AccountRepository.h"

#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

const size_t ACCOUNT_ID = 0;
const size_t USERNAME = 1;
const size_t PASSWORD_HASH = 2;
const size_t PERSON_ID = 3;
const size_t ROLE = 4;
const size_t FULL_NAME = 5;
const size_t PHONE = 6;
const size_t EMAIL = 7;

string AccountRepository::idOf(const Account& a) const {
  return a.getAccountId();
}

vector<string> AccountRepository::toRecord(const Account& a) const {
  return {a.getAccountId(), a.getUsername(), a.getPasswordHash(),
          a.getPersonId(),  a.getRole(),     a.getFullName(),
          a.getPhone(),     a.getEmail()};
}

Account* AccountRepository::fromRecord(const vector<string>& r) const {
  if (r.size() < ROLE + 1 || r[ACCOUNT_ID].empty() || r[USERNAME].empty() ||
      r[PASSWORD_HASH].empty())
    return nullptr;
  string role = StringUtils::trim(r[ROLE]);
  if (role != "Student" && role != "Tutor" && role != "Admin") return nullptr;

  IdGenerator::syncFromExistingId(r[ACCOUNT_ID]);
  IdGenerator::syncFromExistingId(r[PERSON_ID]);
  return new Account(r[ACCOUNT_ID], r[USERNAME], r[PASSWORD_HASH], r[PERSON_ID],
                     role, r.size() > FULL_NAME ? r[FULL_NAME] : "",
                     r.size() > PHONE ? r[PHONE] : "",
                     r.size() > EMAIL ? r[EMAIL] : "");
}

bool AccountRepository::onAdd(Account* a) {
  // trung username
  if (usernameIndex.containsKey(a->getUsername())) return false;
  usernameIndex.insert(a->getUsername(), a);
  return true;
}

void AccountRepository::onRemove(Account* a) {
  usernameIndex.remove(a->getUsername());
}

Account* AccountRepository::findByUsername(const string& username) const {
  Account* result = nullptr;
  usernameIndex.find(username, result);
  return result;
}
