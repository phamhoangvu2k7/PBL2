#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"

class Student : public Person{
    string grade;

    public:
        Student(string fullName, string address, string gender, string dateOfBirth,  int age, string email, string phone, string grade) : Person(fullName, address, gender, dateOfBirth,  age, email, phone) {
            this->grade = grade;
            this->role = "Student";
        }

        string getGrade() const { return grade; }
        void setGrade(const string &grade) { this->grade = grade; }
};

#endif