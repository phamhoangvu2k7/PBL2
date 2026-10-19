#include "../../include/entities/Class.h"

Class::Class() : status("Pending"), tuitionFee(0.0) {}

Class::Class(const string& classId, const string& contractId,
             const string& studentId, const string& tutorId,
             const string& subject, const string& schedule,
             const string& startDate, const string& endDate,
             const string& status, double tuitionFee)
    : classId(classId),
      contractId(contractId),
      studentId(studentId),
      tutorId(tutorId),
      subject(subject),
      schedule(schedule),
      startDate(startDate),
      endDate(endDate),
      status(status),
      tuitionFee(tuitionFee) {}

string Class::getClassId() const { return classId; }
string Class::getContractId() const { return contractId; }
string Class::getStudentId() const { return studentId; }
string Class::getTutorId() const { return tutorId; }
string Class::getSubject() const { return subject; }
string Class::getSchedule() const { return schedule; }
string Class::getStartDate() const { return startDate; }
string Class::getEndDate() const { return endDate; }
string Class::getStatus() const { return status; }
double Class::getTuitionFee() const { return tuitionFee; }

void Class::setClassId(const string& id) { this->classId = id; }
void Class::setContractId(const string& id) { this->contractId = id; }
void Class::setStudentId(const string& id) { this->studentId = id; }
void Class::setTutorId(const string& id) { this->tutorId = id; }
void Class::setSubject(const string& sub) { this->subject = sub; }
void Class::setSchedule(const string& sch) { this->schedule = sch; }
void Class::setStartDate(const string& start) { this->startDate = start; }
void Class::setEndDate(const string& end) { this->endDate = end; }
void Class::setStatus(const string& st) { this->status = st; }
void Class::setTuitionFee(double fee) { this->tuitionFee = fee; }

void Class::display() const {
  cout << "[Lop Hoc] Ma: " << classId << " | Hoc sinh: " << studentId
       << " | Gia su: " << tutorId << " | Mon: " << subject
       << " | Lich: " << schedule << " | Hoc phi: " << tuitionFee << " VND"
       << " | Trang thai: " << status << endl;
}
