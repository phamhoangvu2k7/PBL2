#ifndef CENTER_SERVICE_H
#define CENTER_SERVICE_H

#include <iostream>
#include <string>
#include "../dsa/DoublyLinkedList.h"
#include "../dsa/HashTable.h"
#include "../entities/Student.h"
#include "../entities/Tutor.h"
#include "../entities/Class.h"
#include "../entities/Contract.h"
#include "../entities/Schedule.h"
#include "../helper/Validator.h"
#include "../helper/IdGenerator.h"

using namespace std;

// ho so ca nhan nguoi dung tu sua, khong gom ma
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
    // du lieu chinh
    DoublyLinkedList<Student*> studentList;
    DoublyLinkedList<Tutor*> tutorList;
    DoublyLinkedList<Class*> classList;
    DoublyLinkedList<Contract*> contractList;

    // bang bam tra theo ID, O(1)
    HashTable<string, Student*> studentTable;
    HashTable<string, Tutor*> tutorTable;
    HashTable<string, Class*> classTable;
    HashTable<string, Contract*> contractTable;

    // thu muc chua center_*.txt, rong = khong luu
    string dataDir;
    bool loading = false;  // dang nap file thi khong ghi lai

    // ghi file sau moi thay doi, chua bat luu tru thi bo qua
    void autoSave() const;


    // tim lop bi trung lich, tra ve lop dau tien hoac nullptr
    Class* findTutorConflict(const string& tutorId, const Schedule& schedule) const;
    Class* findStudentConflict(const string& studentId, const Schedule& schedule) const;

public:
    CenterService();
    ~CenterService();

    // luu tru: nap du lieu tu dataDir/center_*.txt roi tu ghi lai
    // tra ve true neu da co du lieu,
    // false = lan chay dau, nen nap du lieu mau
    bool enablePersistence(const string& dataDir);
    bool saveAll() const;

    // 1. hoc vien & gia su
    bool addStudent(Student* student);
    bool addTutor(Tutor* tutor);
    // dam bao co ho so cho tai khoan: da co thi giu nguyen,
    // chua co thi tao tu thong tin tai khoan, false neu du lieu sai
    bool ensureStudentProfile(const string& id, const string& fullName,
                              const string& phone, const string& email,
                              string* outError = nullptr);
    bool ensureTutorProfile(const string& id, const string& fullName,
                            const string& phone, const string& email,
                            string* outError = nullptr);
    // sua ho so (moi truong tru ma), du lieu sai thi giu nguyen
    // va outError nhan ly do
    bool updateStudentProfile(const string& id, const StudentProfile& p, string* outError = nullptr);
    bool updateTutorProfile(const string& id, const TutorProfile& p, string* outError = nullptr);
    Student* findStudentById(const string& id) const;
    Tutor* findTutorById(const string& id) const;
    void displayAllStudents() const;
    void displayAllTutors() const;

    // 2. gia su mo ca (lop o trang thai AVAILABLE)
    // classId tu sinh, schedule phai hop le
    // va khong trung lich voi lop khac cua gia su
    // lich luu dang chuan (Schedule::toString())
    bool tutorOpenClass(const string& tutorId, const string& subject,
                        const string& schedule, double tuitionFee,
                        const string& startDate = "", const string& endDate = "",
                        string* outClassId = nullptr, string* outError = nullptr);

    // 3. hoc vien tim ca con trong theo mon
    void displayAvailableClassesBySubject(const string& subject) const;

    // 4. hoc vien dat ca (tam khoa -> PENDING)
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
    const DoublyLinkedList<Student*>& getStudentList() const { return studentList; }
    const DoublyLinkedList<Tutor*>& getTutorList() const { return tutorList; }
    const DoublyLinkedList<Class*>& getClassList() const { return classList; }
    const DoublyLinkedList<Contract*>& getContractList() const { return contractList; }
};

#endif
