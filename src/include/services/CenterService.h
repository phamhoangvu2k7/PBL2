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
#include "../helper/Validator.h"
#include "../helper/IdGenerator.h"

using namespace std;

class CenterService {
private:
    // Cấu trúc dữ liệu chính (Primary Storage)
    DoublyLinkedList<Student*> studentList;
    DoublyLinkedList<Tutor*> tutorList;
    DoublyLinkedList<Class*> classList;
    DoublyLinkedList<Contract*> contractList;

    // Bảng băm tra cứu theo ID O(1) (Index Tables)
    HashTable<string, Student*> studentTable;
    HashTable<string, Tutor*> tutorTable;
    HashTable<string, Class*> classTable;
    HashTable<string, Contract*> contractTable;

    // Hàm phụ trợ so sánh chuỗi không phân biệt hoa thường
    bool containsIgnoreCase(const string& str, const string& sub) const;

public:
    CenterService();
    ~CenterService();

    // 1. Quản lý Học viên & Gia sư
    bool addStudent(Student* student);
    bool addTutor(Tutor* tutor);
    Student* findStudentById(const string& id) const;
    Tutor* findTutorById(const string& id) const;
    void displayAllStudents() const;
    void displayAllTutors() const;

    // 2. Gia sư mở khung giờ dạy (Tạo lớp ở trạng thái AVAILABLE)
    // classId được tự động sinh bởi IdGenerator, schedule phải hợp lệ theo Validator
    bool tutorOpenClass(const string& tutorId, const string& subject,
                        const string& schedule, double tuitionFee,
                        const string& startDate = "", const string& endDate = "",
                        string* outClassId = nullptr);

    // 3. Học viên tìm kiếm khung giờ còn trống theo môn học
    void displayAvailableClassesBySubject(const string& subject) const;

    // 4. Học viên đặt lịch (Khóa tạm thời khung giờ -> PENDING)
    bool studentBookClass(const string& studentId, const string& classId);

    // 5. Gia sư xem yêu cầu & Phê duyệt / Từ chối
    void displayPendingRequestsByTutor(const string& tutorId) const;
    bool tutorApproveClass(const string& tutorId, const string& classId);
    bool tutorRejectClass(const string& tutorId, const string& classId);

    // 6. Xem lớp của mình (My Classes)
    void displayClassesByStudent(const string& studentId) const;
    void displayClassesByTutor(const string& tutorId) const;

    // 7. Quản lý Hợp đồng & Toàn bộ lớp học
    Contract* findContractById(const string& contractId) const;
    Class* findClassById(const string& classId) const;
    void displayAllClasses() const;
    void displayAllContracts() const;
    // 8. Getters truy xuất cấu trúc dữ liệu cho GUI
    const DoublyLinkedList<Student*>& getStudentList() const { return studentList; }
    const DoublyLinkedList<Tutor*>& getTutorList() const { return tutorList; }
    const DoublyLinkedList<Class*>& getClassList() const { return classList; }
    const DoublyLinkedList<Contract*>& getContractList() const { return contractList; }
};

#endif
