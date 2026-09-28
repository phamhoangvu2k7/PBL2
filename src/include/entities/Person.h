#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
using namespace std;

class Person {
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
        Person();
        Person(string fullName, string address, string gender, string dateOfBirth, int age, string email, string phone);
        Person(string uid, string fullName, string address, string gender, string dateOfBirth, int age, string email, string phone, string role);
        virtual ~Person(); // virtual : hàm hủy con gọi trc khi gọi hàm hủy lớp cha

        // Getters
        string getUid() const;
        string getFullName() const;
        string getAddress() const;
        string getGender() const;
        string getEmail() const;
        string getPhone() const;
        string getDateOfBirth() const;
        string getRole() const;
        int getAge() const;

        // Setters
        void setUid(const string &uid);
        void setFullName(const string &fullName);
        void setAddress(const string &address);
        void setGender(const string &gender);
        void setEmail(const string &email);
        void setPhone(const string &phone);
        void setDateOfBirth(const string &dateOfBirth);
        void setAge(int age);
        void setRole(const string &role);

        // Helper methods
        void displayBasicInfo() const;
        void updateContactInfo(const string &phone, const string &email, const string &address);
        bool validateEmail(const string &email) const;
        bool validatePhone(const string &phone) const;
};

#endif