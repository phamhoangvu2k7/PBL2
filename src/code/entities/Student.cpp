#include "../../include/entities/Student.h"

Student::Student() : Person(), budget(0.0), hasTutor(false) {
  this->role = "Student";
}

Student::Student(const string& id, const string& fullName, const string& grade,
                 const string& phone, const string& email,
                 const string& address, const string& gender,
                 const string& dateOfBirth, const string& subjects,
                 const string& schedule, double budget, bool hasTutor,
                 const string& registeredClasses)
    : Person(id, fullName, phone, email, address, gender, dateOfBirth,
             "Student"),
      grade(grade),
      subjects(subjects),
      schedule(schedule),
      budget(budget),
      hasTutor(hasTutor),
      registeredClasses(registeredClasses) {}

string Student::getGrade() const { return grade; }
string Student::getSubjects() const { return subjects; }
string Student::getSchedule() const { return schedule; }
double Student::getBudget() const { return budget; }
bool Student::getHasTutor() const { return hasTutor; }
string Student::getRegisteredClasses() const { return registeredClasses; }

void Student::setGrade(const string& grade) { this->grade = grade; }
void Student::setSubjects(const string& subjects) { this->subjects = subjects; }
void Student::setSchedule(const string& schedule) { this->schedule = schedule; }
void Student::setBudget(double budget) { this->budget = budget; }
void Student::setHasTutor(bool hasTutor) { this->hasTutor = hasTutor; }
void Student::setRegisteredClasses(const string& classes) {
  this->registeredClasses = classes;
}

void Student::display() const {
  cout << "[Hoc Sinh] ID: " << id << " | Ten: " << fullName
       << " | Lop: " << grade << " | Mon hoc: " << subjects
       << " | Hoc phi mong muon: " << budget << " VND"
       << " | Trang thai: " << (hasTutor ? "Da co gia su" : "Chua co gia su")
       << endl;
}
