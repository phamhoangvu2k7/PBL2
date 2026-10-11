#ifndef CONTRACT_H
#define CONTRACT_H

#include <iostream>
#include <string>
using namespace std;

class Contract {
 protected:
  string contractId;
  string tutorId;
  string studentId;
  string startDate;
  string endDate;
  string subjects;
  string status;

 public:
  Contract() : status("Pending") {}

  Contract(const string& contractId, const string& tutorId,
           const string& studentId, const string& startDate,
           const string& endDate, const string& subjects, const string& status)
      : contractId(contractId),
        tutorId(tutorId),
        studentId(studentId),
        startDate(startDate),
        endDate(endDate),
        subjects(subjects),
        status(status) {}

  virtual ~Contract() {}

  string getContractId() const { return contractId; }
  string getTutorId() const { return tutorId; }
  string getStudentId() const { return studentId; }
  string getStartDate() const { return startDate; }
  string getEndDate() const { return endDate; }
  string getSubjects() const { return subjects; }
  string getStatus() const { return status; }

  void setContractId(const string& id) { this->contractId = id; }
  void setTutorId(const string& id) { this->tutorId = id; }
  void setStudentId(const string& id) { this->studentId = id; }
  void setStartDate(const string& start) { this->startDate = start; }
  void setEndDate(const string& end) { this->endDate = end; }
  void setSubjects(const string& sub) { this->subjects = sub; }
  void setStatus(const string& st) { this->status = st; }

  virtual void display() const {
    cout << "[Hop Dong] Ma: " << contractId << " | SV: " << studentId
         << " | GS: " << tutorId << " | Mon: " << subjects
         << " | Trang thai: " << status << endl;
  }
};

#endif