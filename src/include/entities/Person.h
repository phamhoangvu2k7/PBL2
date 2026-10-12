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
    Person();
    Person(const string& id, const string& fullName, const string& phone,
           const string& email, const string& address, const string& gender,
           const string& dateOfBirth, const string& role = "User");
    virtual ~Person();

    string getId() const;
    string getUid() const;
    string getFullName() const;
    string getPhone() const;
    string getEmail() const;
    string getAddress() const;
    string getGender() const;
    string getDateOfBirth() const;
    string getRole() const;

    void setId(const string& id);
    void setFullName(const string& fullName);
    void setPhone(const string& phone);
    void setEmail(const string& email);
    void setAddress(const string& address);
    void setGender(const string& gender);
    void setDateOfBirth(const string& dob);
    void setRole(const string& role);

    virtual void display() const;
};

#endif