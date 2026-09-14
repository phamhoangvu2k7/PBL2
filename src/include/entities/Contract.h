#include <iostream>
using namespace std;

class Contract{
    protected:
        string contractId;
        string tutorId;
        string studentId;
        string startDate;
        string endDate;
        string subjects;
        string status;
        
    public:
        Contract() {}
        Contract(string contractId, string tutorId, string studentId, string startDate, string endDate, string subjects, string status){
            this->contractId = contractId;
            this->tutorId = tutorId;
            this->studentId = studentId;
            this->startDate = startDate;
            this->endDate = endDate;
            this->subjects = subjects;
            this->status = status;
        }

        string getContractId() const { return contractId; }
        string getTutorId() const { return tutorId; }
        string getStudentId() const { return studentId; }
        string getStartDate() const { return startDate; }
        string getEndDate() const { return endDate; }
        string getSubjects() const { return subjects; }
        string getStatus() const { return status; }

        void setContractId(const string &contractId) { this->contractId = contractId; }
        void setTutorId(const string &tutorId) { this->tutorId = tutorId; }
        void setStudentId(const string &studentId) { this->studentId = studentId; }
        void setStartDate(const string &startDate) { this->startDate = startDate; }
        void setEndDate(const string &endDate) { this->endDate = endDate; }
        void setSubjects(const string &subjects) { this->subjects = subjects; }
        void setStatus(const string &status) { this->status = status; }
};