#ifndef TUTOR_H
#define TUTOR_H

#include "Person.h"

class Tutor : public Person{
    string subjects;

    public:
        Tutor(string fullName, string address, string gender, string dateOfBirth,  int age, string email, string phone, string subjects) : Person(fullName, address, gender, dateOfBirth,  age, email, phone) {
            this->subjects = subjects;
            this->role = "Tutor";
        }

        string getSubjects() const { return subjects; }
        void setSubjects(const string &subjects) { this->subjects = subjects; }
};

#endif
