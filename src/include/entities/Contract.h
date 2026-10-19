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

    void setContractId(const string& id);
    void setTutorId(const string& id);
    void setStudentId(const string& id);
    void setStartDate(const string& start);
    void setEndDate(const string& end);
    void setSubjects(const string& sub);
    void setStatus(const string& st);

    virtual void display() const;
};

#endif