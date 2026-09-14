#ifndef PERSON_H
#define PERSON_H

#include <iostream>
using namespace std;

class Person{
    protected:
        string uid;
        string fullName;
        string address;
        string gender;
        string email;
        string phone;
        string dateOfBirth;
        string role;
        int age;

    public:
        Person() : age(0) {}

        Person(string fullName, string address, string gender, string dateOfBirth,  int age, string email, string phone){
            this->fullName = fullName;
            this->age = age;
            this->email = email;
            this->phone = phone;
            this->gender = gender;
            this->dateOfBirth = dateOfBirth;
            this->address = address;
        }

        string getUid() const { return uid; }
        string getFullName() const { return fullName; }
        string getAddress() const { return address; }
        string getGender() const { return gender; }
        string getEmail() const { return email; }
        string getPhone() const { return phone; }
        string getDateOfBirth() const { return dateOfBirth; }
        string getRole() const { return role; }
        int getAge() const { return age; }

        void setUid(const string &uid) { this->uid = uid; }
        void setFullName(const string &fullName) { this->fullName = fullName; }
        void setAddress(const string &address) { this->address = address; }
        void setGender(const string &gender) { this->gender = gender; }
        void setEmail(const string &email) { this->email = email; }
        void setPhone(const string &phone) { this->phone = phone; }
        void setDateOfBirth(const string &dateOfBirth) { this->dateOfBirth = dateOfBirth; }
        void setAge(int age) { this->age = age; }
        void setRole(const string &role) { this->role = role; }
};

#endif