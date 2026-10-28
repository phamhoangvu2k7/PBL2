#include "../../include/entities/Tutor.h"

Tutor::Tutor() : Person(), expYears(0), hourlyRate(0.0), isAvailable(true) {
    this->role = "Tutor";
}

Tutor::Tutor(const string& id, const string& fullName, const string& phone,
             const string& email, const string& address, const string& gender,
             const string& dateOfBirth, const string& subjects, const string& qualification,
             int expYears, double hourlyRate, const string& schedule, bool isAvailable,
             const string& registeredClasses, const string& achievements)
    : Person(id, fullName, phone, email, address, gender, dateOfBirth, "Tutor"),
      subjects(subjects), qualification(qualification), expYears(expYears),
      hourlyRate(hourlyRate), schedule(schedule), isAvailable(isAvailable),
      registeredClasses(registeredClasses), achievements(achievements) {}

string Tutor::getSubjects() const { return subjects; }
string Tutor::getQualification() const { return qualification; }
int Tutor::getExpYears() const { return expYears; }
double Tutor::getHourlyRate() const { return hourlyRate; }
string Tutor::getSchedule() const { return schedule; }
bool Tutor::getIsAvailable() const { return isAvailable; }
string Tutor::getRegisteredClasses() const { return registeredClasses; }
string Tutor::getAchievements() const { return achievements; }

void Tutor::setSubjects(const string& subjects) { this->subjects = subjects; }
void Tutor::setQualification(const string& qualification) { this->qualification = qualification; }
void Tutor::setExpYears(int exp) { this->expYears = exp; }
void Tutor::setHourlyRate(double rate) { this->hourlyRate = rate; }
void Tutor::setSchedule(const string& schedule) { this->schedule = schedule; }
void Tutor::setIsAvailable(bool available) { this->isAvailable = available; }
void Tutor::setRegisteredClasses(const string& classes) { this->registeredClasses = classes; }
void Tutor::setAchievements(const string& achievements) { this->achievements = achievements; }

void Tutor::display() const {
    cout << "[Gia Su] ID: " << id << " | Ten: " << fullName 
         << " | Mon day: " << subjects << " | Kinh nghiem: " << expYears << " nam"
         << " | Hoc phi: " << hourlyRate << " VND/buoi"
         << " | Trang thai: " << (isAvailable ? "San sang nhan lop" : "Da kin lich") 
         << endl;
}

