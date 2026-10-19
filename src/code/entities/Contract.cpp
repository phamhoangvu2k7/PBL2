#include "../../include/entities/Contract.h"

Contract::Contract() : status("Pending") {}

Contract::Contract(const string& contractId, const string& tutorId,
                   const string& studentId, const string& startDate,
                   const string& endDate, const string& subjects,
                   const string& status)
    : contractId(contractId),
      tutorId(tutorId),
      studentId(studentId),
      startDate(startDate),
      endDate(endDate),
      subjects(subjects),
      status(status) {}

Contract::~Contract() {}

string Contract::getContractId() const { return contractId; }
string Contract::getTutorId() const { return tutorId; }
string Contract::getStudentId() const { return studentId; }
string Contract::getStartDate() const { return startDate; }
string Contract::getEndDate() const { return endDate; }
string Contract::getSubjects() const { return subjects; }
string Contract::getStatus() const { return status; }

void Contract::setContractId(const string& id) { this->contractId = id; }
void Contract::setTutorId(const string& id) { this->tutorId = id; }
void Contract::setStudentId(const string& id) { this->studentId = id; }
void Contract::setStartDate(const string& start) { this->startDate = start; }
void Contract::setEndDate(const string& end) { this->endDate = end; }
void Contract::setSubjects(const string& sub) { this->subjects = sub; }
void Contract::setStatus(const string& st) { this->status = st; }

void Contract::display() const {
  cout << "[Hop Dong] Ma: " << contractId << " | SV: " << studentId
       << " | GS: " << tutorId << " | Mon: " << subjects
       << " | Trang thai: " << status << endl;
}
