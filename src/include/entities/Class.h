#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
using namespace std;

class Class {
private:
    string classId;
    string contractId;
    string studentId;
    string tutorId;
    string subject;
    string schedule;
    string startDate;
    string endDate;
    string status;
    double tuitionFee;

public:
    Class() : tuitionFee(0.0), status("Pending") {}

    Class(const string& classId, const string& contractId, const string& studentId,
          const string& tutorId, const string& subject, const string& schedule,
          const string& startDate, const string& endDate, const string& status,
          double tuitionFee)
        : classId(classId), contractId(contractId), studentId(studentId), tutorId(tutorId),
          subject(subject), schedule(schedule), startDate(startDate), endDate(endDate),
          status(status), tuitionFee(tuitionFee) {}

    string getClassId() const { return classId; }
    string getContractId() const { return contractId; }
    string getStudentId() const { return studentId; }
    string getTutorId() const { return tutorId; }
    string getSubject() const { return subject; }
    string getSchedule() const { return schedule; }
    string getStartDate() const { return startDate; }
    string getEndDate() const { return endDate; }
    string getStatus() const { return status; }
    double getTuitionFee() const { return tuitionFee; }

    void setClassId(const string& id) { this->classId = id; }
    void setContractId(const string& id) { this->contractId = id; }
    void setStudentId(const string& id) { this->studentId = id; }
    void setTutorId(const string& id) { this->tutorId = id; }
    void setSubject(const string& sub) { this->subject = sub; }
    void setSchedule(const string& sch) { this->schedule = sch; }
    void setStartDate(const string& start) { this->startDate = start; }
    void setEndDate(const string& end) { this->endDate = end; }
    void setStatus(const string& st) { this->status = st; }
    void setTuitionFee(double fee) { this->tuitionFee = fee; }

    void display() const {
        cout << "[Lop Hoc] Ma: " << classId 
             << " | Hoc sinh: " << studentId 
             << " | Gia su: " << tutorId 
             << " | Mon: " << subject 
             << " | Lich: " << schedule 
             << " | Hoc phi: " << tuitionFee << " VND"
             << " | Trang thai: " << status 
             << endl;
    }
};

#endif