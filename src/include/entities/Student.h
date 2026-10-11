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
  Student() : Person(), budget(0.0), hasTutor(false) { this->role = "Student"; }

  Student(const string& id, const string& fullName, const string& grade,
          const string& phone, const string& email, const string& address,
          const string& gender, const string& dateOfBirth,
          const string& subjects, const string& schedule, double budget = 0.0,
          bool hasTutor = false, const string& registeredClasses = "")
      : Person(id, fullName, phone, email, address, gender, dateOfBirth,
               "Student"),
        grade(grade),
        subjects(subjects),
        schedule(schedule),
        budget(budget),
        hasTutor(hasTutor),
        registeredClasses(registeredClasses) {}

  string getGrade() const { return grade; }
  string getSubjects() const { return subjects; }
  string getSchedule() const { return schedule; }
  double getBudget() const { return budget; }
  bool getHasTutor() const { return hasTutor; }
  string getRegisteredClasses() const { return registeredClasses; }

  void setGrade(const string& grade) { this->grade = grade; }
  void setSubjects(const string& subjects) { this->subjects = subjects; }
  void setSchedule(const string& schedule) { this->schedule = schedule; }
  void setBudget(double budget) { this->budget = budget; }
  void setHasTutor(bool hasTutor) { this->hasTutor = hasTutor; }
  void setRegisteredClasses(const string& classes) {
    this->registeredClasses = classes;
  }

  void display() const override {
    cout << "[Hoc Sinh] ID: " << id << " | Ten: " << fullName
         << " | Lop: " << grade << " | Mon hoc: " << subjects
         << " | Hoc phi mong muon: " << budget << " VND"
         << " | Trang thai: " << (hasTutor ? "Da co gia su" : "Chua co gia su")
         << endl;
  }
};

#endif