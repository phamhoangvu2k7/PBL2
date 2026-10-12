#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"

class Student : public Person {
private:
    string grade;
    string subjects;
    string schedule;
    double budget;
    bool hasTutor;
    string registeredClasses;

public:
    Student();
    Student(const string& id, const string& fullName, const string& grade,
            const string& phone, const string& email, const string& address,
            const string& gender, const string& dateOfBirth, const string& subjects,
            const string& schedule, double budget = 0.0, bool hasTutor = false,
            const string& registeredClasses = "");

    string getGrade() const;
    string getSubjects() const;
    string getSchedule() const;
    double getBudget() const;
    bool getHasTutor() const;
    string getRegisteredClasses() const;

    void setGrade(const string& grade);
    void setSubjects(const string& subjects);
    void setSchedule(const string& schedule);
    void setBudget(double budget);
    void setHasTutor(bool hasTutor);
    void setRegisteredClasses(const string& classes);

    void display() const override;
};

#endif