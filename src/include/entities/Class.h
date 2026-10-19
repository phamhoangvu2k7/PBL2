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
    Class();
    Class(const string& classId, const string& contractId, const string& studentId,
          const string& tutorId, const string& subject, const string& schedule,
          const string& startDate, const string& endDate, const string& status,
          double tuitionFee);

    string getClassId() const;
    string getContractId() const;
    string getStudentId() const;
    string getTutorId() const;
    string getSubject() const;
    string getSchedule() const;
    string getStartDate() const;
    string getEndDate() const;
    string getStatus() const;
    double getTuitionFee() const;

    void setClassId(const string& id);
    void setContractId(const string& id);
    void setStudentId(const string& id);
    void setTutorId(const string& id);
    void setSubject(const string& sub);
    void setSchedule(const string& sch);
    void setStartDate(const string& start);
    void setEndDate(const string& end);
    void setStatus(const string& st);
    void setTuitionFee(double fee);

    void display() const;
};

#endif