#include "../../include/entities/Person.h"

Person::Person() : role("User") {}

Person::Person(const string& id, const string& fullName, const string& phone,
               const string& email, const string& address, const string& gender,
               const string& dateOfBirth, const string& role)
    : id(id),
      fullName(fullName),
      phone(phone),
      email(email),
      address(address),
      gender(gender),
      dateOfBirth(dateOfBirth),
      role(role) {}

Person::~Person() {}

string Person::getId() const { return id; }
string Person::getUid() const { return id; }
string Person::getFullName() const { return fullName; }
string Person::getPhone() const { return phone; }
string Person::getEmail() const { return email; }
string Person::getAddress() const { return address; }
string Person::getGender() const { return gender; }
string Person::getDateOfBirth() const { return dateOfBirth; }
string Person::getRole() const { return role; }

void Person::setId(const string& id) { this->id = id; }
void Person::setFullName(const string& fullName) { this->fullName = fullName; }
void Person::setPhone(const string& phone) { this->phone = phone; }
void Person::setEmail(const string& email) { this->email = email; }
void Person::setAddress(const string& address) { this->address = address; }
void Person::setGender(const string& gender) { this->gender = gender; }
void Person::setDateOfBirth(const string& dob) { this->dateOfBirth = dob; }
void Person::setRole(const string& role) { this->role = role; }

void Person::display() const {
  cout << "[" << role << "] ID: " << id << " | Ten: " << fullName
       << " | SDT: " << phone << " | Email: " << email << endl;
}