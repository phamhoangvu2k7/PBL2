#include "../../include/entities/Person.h"
#include <iostream>
#include <cctype>

// Constructor mặc định
Person::Person() {
    this->uid = "";
    this->fullName = "";
    this->address = "";
    this->gender = "";
    this->email = "";
    this->phone = "";
    this->dateOfBirth = "";
    this->role = "";
    this->age = 0;
}

// Constructor với 7 tham số
Person::Person(string fullName, string address, string gender, string dateOfBirth, int age, string email, string phone) {
    this->uid = "";
    this->fullName = fullName;
    this->address = address;
    this->gender = gender;
    this->dateOfBirth = dateOfBirth;
    this->age = age;
    this->email = email;
    this->phone = phone;
    this->role = "";
}

// Constructor đầy đủ tham số
Person::Person(string uid, string fullName, string address, string gender, string dateOfBirth, int age, string email, string phone, string role) {
    this->uid = uid;
    this->fullName = fullName;
    this->address = address;
    this->gender = gender;
    this->dateOfBirth = dateOfBirth;
    this->age = age;
    this->email = email;
    this->phone = phone;
    this->role = role;
}

// Destructor
Person::~Person() {
}

// Getter methods
string Person::getUid() const {
    return uid;
}

string Person::getFullName() const {
    return fullName;
}

string Person::getAddress() const {
    return address;
}

string Person::getGender() const {
    return gender;
}

string Person::getEmail() const {
    return email;
}

string Person::getPhone() const {
    return phone;
}

string Person::getDateOfBirth() const {
    return dateOfBirth;
}

string Person::getRole() const {
    return role;
}

int Person::getAge() const {
    return age;
}

// Setter methods
void Person::setUid(const string &uid) {
    this->uid = uid;
}

void Person::setFullName(const string &fullName) {
    this->fullName = fullName;
}

void Person::setAddress(const string &address) {
    this->address = address;
}

void Person::setGender(const string &gender) {
    this->gender = gender;
}

void Person::setEmail(const string &email) {
    this->email = email;
}

void Person::setPhone(const string &phone) {
    this->phone = phone;
}

void Person::setDateOfBirth(const string &dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}

void Person::setAge(int age) {
    this->age = age;
}

void Person::setRole(const string &role) {
    this->role = role;
}

// Phương thức hiển thị thông tin cơ bản
void Person::displayBasicInfo() const {
    cout << "UID: " << uid << endl;
    cout << "Ho ten: " << fullName << endl;
    cout << "Tuoi: " << age << endl;
    cout << "Gioi tinh: " << gender << endl;
    cout << "Ngay sinh: " << dateOfBirth << endl;
    cout << "Dia chi: " << address << endl;
    cout << "Dien thoai: " << phone << endl;
    cout << "Email: " << email << endl;
    cout << "Vai tro: " << role << endl;
}

// Cập nhật thông tin liên lạc
void Person::updateContactInfo(const string &phone, const string &email, const string &address) {
    this->phone = phone;
    this->email = email;
    this->address = address;
}

// Kiểm tra email hợp lệ (có @ và .)
bool Person::validateEmail(const string &email) const {
    return (email.find('@') != string::npos && email.find('.') != string::npos);
}

// Kiểm tra số điện thoại hợp lệ (chỉ chứa số và độ dài 10-11)
bool Person::validatePhone(const string &phone) const {
    if (phone.length() < 10 || phone.length() > 11) return false;
    for (char c : phone) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}