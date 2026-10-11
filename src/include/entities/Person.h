#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string id;
    string fullName;
    string phone;
    string email;
    string address;
    string gender;
    string dateOfBirth;
    string role;

public:
    Person() : role("User") {}

    Person(const string& id, const string& fullName, const string& phone,
           const string& email, const string& address, const string& gender,
           const string& dateOfBirth, const string& role = "User")
        : id(id), fullName(fullName), phone(phone), email(email),
          address(address), gender(gender), dateOfBirth(dateOfBirth), role(role) {}

    virtual ~Person() {}

    string getId() const { return id; }
    string getUid() const { return id; }
    string getFullName() const { return fullName; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }
    string getGender() const { return gender; }
    string getDateOfBirth() const { return dateOfBirth; }
    string getRole() const { return role; }

    void setId(const string& id) { this->id = id; }
    void setFullName(const string& fullName) { this->fullName = fullName; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setEmail(const string& email) { this->email = email; }
    void setAddress(const string& address) { this->address = address; }
    void setGender(const string& gender) { this->gender = gender; }
    void setDateOfBirth(const string& dob) { this->dateOfBirth = dob; }
    void setRole(const string& role) { this->role = role; }

    virtual void display() const {
        cout << "[" << role << "] ID: " << id << " | Ten: " << fullName 
             << " | SDT: " << phone << " | Email: " << email << endl;
    }
};

#endif