#ifndef TUTOR_H
#define TUTOR_H

#include "Person.h"

class Tutor : public Person {
private:
    string subjects;
    string qualification;
    int expYears;
    double hourlyRate;
    string schedule;
    bool isAvailable;
    string registeredClasses;
    string achievements;  // thanh tich

public:
    Tutor();
    Tutor(const string& id, const string& fullName, const string& phone,
          const string& email, const string& address, const string& gender,
          const string& dateOfBirth, const string& subjects, const string& qualification,
          int expYears, double hourlyRate, const string& schedule, bool isAvailable = true,
          const string& registeredClasses = "", const string& achievements = "");

    string getSubjects() const;
    string getQualification() const;
    int getExpYears() const;
    double getHourlyRate() const;
    string getSchedule() const;
    bool getIsAvailable() const;
    string getRegisteredClasses() const;
    string getAchievements() const;

    void setSubjects(const string& subjects);
    void setQualification(const string& qualification);
    void setExpYears(int exp);
    void setHourlyRate(double rate);
    void setSchedule(const string& schedule);
    void setIsAvailable(bool available);
    void setRegisteredClasses(const string& classes);
    void setAchievements(const string& achievements);

    void display() const override;
};

#endif
