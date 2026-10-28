#ifndef CENTER_SERVICE_H
#define CENTER_SERVICE_H

#include <iostream>
#include <string>
#include <vector>
#include "../entities/Student.h"
#include "../entities/Tutor.h"
#include "../entities/Class.h"
#include "../entities/Contract.h"
#include "../entities/Schedule.h"
#include "../helper/Validator.h"
#include "../helper/IdGenerator.h"
#include "../repositories/StudentRepository.h"
#include "../repositories/TutorRepository.h"
#include "../repositories/ClassRepository.h"
#include "../repositories/ContractRepository.h"

using namespace std;

struct StudentProfile {
    string fullName, phone, email, address, gender, dateOfBirth;
    string grade, subjects, schedule;
    double budget = 0.0;
};

struct TutorProfile {
    string fullName, phone, email, address, gender, dateOfBirth;
    string subjects, qualification, achievements, schedule;
    int expYears = 0;
};

class CenterService {
private:
    StudentRepository& studentRepo;
    TutorRepository& tutorRepo;
    ClassRepository& classRepo;
    ContractRepository& contractRepo;

    Class* findTutorConflict(const string& tutorId, const Schedule& schedule) const;
    Class* findStudentConflict(const string& studentId, const Schedule& schedule) const;

public:
    //DI
    CenterService(StudentRepository& students, TutorRepository& tutors,
                  ClassRepository& classes, ContractRepository& contracts);

    bool loadData();

    // 1. them hoc vien , gia su
    bool addStudent(Student* student);
    bool addTutor(Tutor* tutor);

    // dam bao co ho so cho tai khoan: da co thi giu nguyen,
    bool ensureStudentProfile(const string& id, const string& fullName,
                              const string& phone, const string& email,
                              string* outError = nullptr);
    bool ensureTutorProfile(const string& id, const string& fullName,
                            const string& phone, const string& email,
                            string* outError = nullptr);
    // sua ho so 

    bool updateStudentProfile(const string& id, const StudentProfile& p, string* outError = nullptr);
    bool updateTutorProfile(const string& id, const TutorProfile& p, string* outError = nullptr);
    Student* findStudentById(const string& id) const;
    Tutor* findTutorById(const string& id) const;
    void displayAllStudents() const;
    void displayAllTutors() const;

    // 2. gia su mo ca 

    bool tutorOpenClass(const string& tutorId, const string& subject,
                        const string& schedule, double tuitionFee,
                        const string& startDate = "", const string& endDate = "",
                        string* outClassId = nullptr, string* outError = nullptr);

    // 3. hoc vien tim ca con trong theo mon
    void displayAvailableClassesBySubject(const string& subject) const;

    // 4. hoc vien dat ca 
    // tu choi neu trung lich lop PENDING/ACTIVE cua hoc vien
    bool studentBookClass(const string& studentId, const string& classId,
                          string* outError = nullptr);

    // 5. gia su xem yeu cau, duyet / tu choi
    void displayPendingRequestsByTutor(const string& tutorId) const;
    bool tutorApproveClass(const string& tutorId, const string& classId);
    bool tutorRejectClass(const string& tutorId, const string& classId);

    // 6. lop cua toi
    void displayClassesByStudent(const string& studentId) const;
    void displayClassesByTutor(const string& tutorId) const;

    // 7. hop dong & tat ca lop
    Contract* findContractById(const string& contractId) const;
    Class* findClassById(const string& classId) const;
    void displayAllClasses() const;
    void displayAllContracts() const;
    // 8. getter cho GUI
    vector<Student*> getStudents() const { return studentRepo.toVector(); }
    vector<Tutor*> getTutors() const { return tutorRepo.toVector(); }
    vector<Class*> getClasses() const { return classRepo.toVector(); }
    vector<Contract*> getContracts() const { return contractRepo.toVector(); }
};

#endif
