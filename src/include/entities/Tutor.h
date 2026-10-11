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

 public:
  Tutor() : Person(), expYears(0), hourlyRate(0.0), isAvailable(true) {
    this->role = "Tutor";
  }

  Tutor(const string& id, const string& fullName, const string& phone,
        const string& email, const string& address, const string& gender,
        const string& dateOfBirth, const string& subjects,
        const string& qualification, int expYears, double hourlyRate,
        const string& schedule, bool isAvailable = true,
        const string& registeredClasses = "")
      : Person(id, fullName, phone, email, address, gender, dateOfBirth,
               "Tutor"),
        subjects(subjects),
        qualification(qualification),
        expYears(expYears),
        hourlyRate(hourlyRate),
        schedule(schedule),
        isAvailable(isAvailable),
        registeredClasses(registeredClasses) {}

  string getSubjects() const { return subjects; }
  string getQualification() const { return qualification; }
  int getExpYears() const { return expYears; }
  double getHourlyRate() const { return hourlyRate; }
  string getSchedule() const { return schedule; }
  bool getIsAvailable() const { return isAvailable; }
  string getRegisteredClasses() const { return registeredClasses; }

  void setSubjects(const string& subjects) { this->subjects = subjects; }
  void setQualification(const string& qualification) {
    this->qualification = qualification;
  }
  void setExpYears(int exp) { this->expYears = exp; }
  void setHourlyRate(double rate) { this->hourlyRate = rate; }
  void setSchedule(const string& schedule) { this->schedule = schedule; }
  void setIsAvailable(bool available) { this->isAvailable = available; }
  void setRegisteredClasses(const string& classes) {
    this->registeredClasses = classes;
  }

  void display() const override {
    cout << "[Gia Su] ID: " << id << " | Ten: " << fullName
         << " | Mon day: " << subjects << " | Kinh nghiem: " << expYears
         << " nam"
         << " | Hoc phi: " << hourlyRate << " VND/buoi"
         << " | Trang thai: "
         << (isAvailable ? "San sang nhan lop" : "Da kin lich") << endl;
  }
};

#endif
