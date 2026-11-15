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
    // ket thuc hop dong
    string requestedBy;  // ma nguoi yeu cau ket thuc
    string endReason;
    string endedOn;      // ngay ket thuc thuc te

public:
    Contract();
    Contract(const string& contractId, const string& tutorId, const string& studentId,
             const string& startDate, const string& endDate, const string& subjects,
             const string& status);
    virtual ~Contract();

    string getContractId() const;
    string getTutorId() const;
    string getStudentId() const;
    string getStartDate() const;
    string getEndDate() const;
    string getSubjects() const;
    string getStatus() const;
    string getRequestedBy() const;
    string getEndReason() const;
    string getEndedOn() const;

    void setContractId(const string& id);
    void setTutorId(const string& id);
    void setStudentId(const string& id);
    void setStartDate(const string& start);
    void setEndDate(const string& end);
    void setSubjects(const string& sub);
    void setStatus(const string& st);
    void setRequestedBy(const string& personId);
    void setEndReason(const string& reason);
    void setEndedOn(const string& date);

    virtual void display() const;
};

#endif