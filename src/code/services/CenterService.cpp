#include "../../include/services/CenterService.h"
#include "../../include/helper/Validator.h"
#include "../../include/helper/IdGenerator.h"
#include <algorithm>
#include <cctype>

CenterService::CenterService() {}

CenterService::~CenterService() {
    // Giải phóng bộ nhớ động của danh sách học viên
    Node<Student*>* curStudent = studentList.getHead();
    while (curStudent != nullptr) {
        delete curStudent->data;
        curStudent = curStudent->next;
    }
    studentList.clear();
    studentTable.clear();

    // Giải phóng bộ nhớ động của danh sách gia sư
    Node<Tutor*>* curTutor = tutorList.getHead();
    while (curTutor != nullptr) {
        delete curTutor->data;
        curTutor = curTutor->next;
    }
    tutorList.clear();
    tutorTable.clear();

    // Giải phóng bộ nhớ động của danh sách lớp học
    Node<Class*>* curClass = classList.getHead();
    while (curClass != nullptr) {
        delete curClass->data;
        curClass = curClass->next;
    }
    classList.clear();
    classTable.clear();

    // Giải phóng bộ nhớ động của danh sách hợp đồng
    Node<Contract*>* curContract = contractList.getHead();
    while (curContract != nullptr) {
        delete curContract->data;
        curContract = curContract->next;
    }
    contractList.clear();
    contractTable.clear();
}

bool CenterService::containsIgnoreCase(const string& str, const string& sub) const {
    if (sub.empty()) return true;
    string strLower = str;
    string subLower = sub;
    for (char& c : strLower) c = tolower(c);
    for (char& c : subLower) c = tolower(c);
    return strLower.find(subLower) != string::npos;
}

// ------------------- 1. Quản lý Học viên & Gia sư -------------------

bool CenterService::addStudent(Student* student) {
    if (student == nullptr) return false;

    // Validate dữ liệu trước khi thêm
    if (!Validator::isNotEmpty(student->getFullName())) {
        cout << "[Loi] Ten hoc vien khong duoc de trong!\n";
        return false;
    }
    if (!Validator::isValidPhone(student->getPhone())) {
        cout << "[Loi] " << Validator::getPhoneError(student->getPhone()) << "\n";
        return false;
    }
    if (!Validator::isValidEmail(student->getEmail())) {
        cout << "[Loi] " << Validator::getEmailError(student->getEmail()) << "\n";
        return false;
    }

    // Tự động sinh ID nếu chưa có
    if (student->getId().empty()) {
        student->setId(IdGenerator::generateStudentId());
    } else {
        // Đồng bộ bộ đếm với ID đã có (tránh trùng lặp khi load từ file)
        IdGenerator::syncFromExistingId(student->getId());
    }

    if (studentTable.containsKey(student->getId())) {
        cout << "[Loi] Ma hoc vien " << student->getId() << " da ton tai!\n";
        return false;
    }
    studentList.push_back(student);
    studentTable.insert(student->getId(), student);
    return true;
}

bool CenterService::addTutor(Tutor* tutor) {
    if (tutor == nullptr) return false;

    // Validate dữ liệu trước khi thêm
    if (!Validator::isNotEmpty(tutor->getFullName())) {
        cout << "[Loi] Ten gia su khong duoc de trong!\n";
        return false;
    }
    if (!Validator::isValidPhone(tutor->getPhone())) {
        cout << "[Loi] " << Validator::getPhoneError(tutor->getPhone()) << "\n";
        return false;
    }
    if (!Validator::isValidEmail(tutor->getEmail())) {
        cout << "[Loi] " << Validator::getEmailError(tutor->getEmail()) << "\n";
        return false;
    }
    if (!Validator::isValidExpYears(tutor->getExpYears())) {
        cout << "[Loi] So nam kinh nghiem phai >= 0!\n";
        return false;
    }

    // Tự động sinh ID nếu chưa có
    if (tutor->getId().empty()) {
        tutor->setId(IdGenerator::generateTutorId());
    } else {
        IdGenerator::syncFromExistingId(tutor->getId());
    }

    if (tutorTable.containsKey(tutor->getId())) {
        cout << "[Loi] Ma gia su " << tutor->getId() << " da ton tai!\n";
        return false;
    }
    tutorList.push_back(tutor);
    tutorTable.insert(tutor->getId(), tutor);
    return true;
}

Student* CenterService::findStudentById(const string& id) const {
    Student* res = nullptr;
    if (studentTable.find(id, res)) return res;
    return nullptr;
}

Tutor* CenterService::findTutorById(const string& id) const {
    Tutor* res = nullptr;
    if (tutorTable.find(id, res)) return res;
    return nullptr;
}

void CenterService::displayAllStudents() const {
    cout << "\n===== DANH SACH HOC VIEN (" << studentList.getSize() << ") =====\n";
    Node<Student*>* cur = studentList.getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}

void CenterService::displayAllTutors() const {
    cout << "\n===== DANH SACH GIA SU (" << tutorList.getSize() << ") =====\n";
    Node<Tutor*>* cur = tutorList.getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}

// ------------------- 2. Gia sư mở ca học -------------------

bool CenterService::tutorOpenClass(const string& tutorId, const string& subject,
                                   const string& schedule, double tuitionFee,
                                   const string& startDate, const string& endDate,
                                   string* outClassId) {
    if (!tutorTable.containsKey(tutorId)) {
        cout << "[Loi] Khong tim thay Gia su co ma " << tutorId << "!\n";
        return false;
    }
    if (!Validator::isValidSchedule(schedule)) {
        cout << "[Loi] Lich hoc khong hop le! " << Validator::getScheduleError(schedule) << "\n";
        return false;
    }
    if (!Validator::isValidFee(tuitionFee)) {
        cout << "[Loi] Hoc phi phai lon hon 0!\n";
        return false;
    }
    if (!Validator::isNotEmpty(subject)) {
        cout << "[Loi] Ten mon hoc khong duoc de trong!\n";
        return false;
    }

    // Tu dong sinh ma lop
    string classId = IdGenerator::generateClassId();
    if (outClassId != nullptr) *outClassId = classId;

    Class* newClass = new Class(classId, "", "", tutorId, subject, schedule,
                                startDate, endDate, "AVAILABLE", tuitionFee);
    classList.push_back(newClass);
    classTable.insert(classId, newClass);

    cout << "[Thanh Cong] Gia su " << tutorId << " da mo thanh cong ca hoc "
         << classId << " (Mon: " << subject << " | Lich: " << schedule << ")\n";
    return true;
}

// ------------------- 3. Tìm kiếm khung giờ còn trống -------------------

void CenterService::displayAvailableClassesBySubject(const string& subject) const {
    cout << "\n===== KHUNG GIO HOC CON TRONG THEO MON: \"" << subject << "\" =====\n";
    int count = 0;
    Node<Class*>* cur = classList.getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getStatus() == "AVAILABLE" && containsIgnoreCase(c->getSubject(), subject)) {
            count++;
            Tutor* t = findTutorById(c->getTutorId());
            string tutorName = (t != nullptr) ? t->getFullName() : "N/A";

            cout << "  [" << count << "] Ma Lop: " << c->getClassId()
                 << " | Mon: " << c->getSubject()
                 << " | Lich hoc: " << c->getSchedule()
                 << " | Hoc phi: " << c->getTuitionFee() << " VND"
                 << " | Gia su: " << tutorName << " (" << c->getTutorId() << ")\n";
        }
        cur = cur->next;
    }
    if (count == 0) {
        cout << "  (Khong co ca hoc nao phu hop dang trong)\n";
    }
}

// ------------------- 4. Học viên chọn khung giờ -------------------

bool CenterService::studentBookClass(const string& studentId, const string& classId) {
    if (!studentTable.containsKey(studentId)) {
        cout << "[Loi] Khong tim thay Hoc vien " << studentId << "!\n";
        return false;
    }

    Class* cls = nullptr;
    if (!classTable.find(classId, cls) || cls == nullptr) {
        cout << "[Loi] Khong tim thay Lop hoc " << classId << "!\n";
        return false;
    }

    if (cls->getStatus() != "AVAILABLE") {
        cout << "[Loi] Ca hoc " << classId << " da duoc nguoi khac chon hoac dang hoat dong!\n";
        return false;
    }

    // Đổi trạng thái sang PENDING và gán studentId
    cls->setStudentId(studentId);
    cls->setStatus("PENDING");

    cout << "[Thanh Cong] Hoc vien " << studentId << " da dang ky ca hoc " << classId 
         << ". Ca hoc da tam khoa cho Gia su xac nhan!\n";
    return true;
}

// ------------------- 5. Gia sư xem và duyệt/từ chối -------------------

void CenterService::displayPendingRequestsByTutor(const string& tutorId) const {
    cout << "\n===== YEU CAU CHO DUYET CUA GIA SU " << tutorId << " =====\n";
    int count = 0;
    Node<Class*>* cur = classList.getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getTutorId() == tutorId && c->getStatus() == "PENDING") {
            count++;
            Student* st = findStudentById(c->getStudentId());
            string studentName = (st != nullptr) ? st->getFullName() : "N/A";
            string studentPhone = (st != nullptr) ? st->getPhone() : "N/A";

            cout << "  [" << count << "] Ma Lop: " << c->getClassId()
                 << " | Mon: " << c->getSubject()
                 << " | Lich: " << c->getSchedule()
                 << " | Hoc sinh DK: " << studentName << " (ID: " << c->getStudentId() 
                 << " - SDT: " << studentPhone << ")\n";
        }
        cur = cur->next;
    }
    if (count == 0) {
        cout << "  (Khong co yeu cau nao dang cho duyet)\n";
    }
}

bool CenterService::tutorApproveClass(const string& tutorId, const string& classId) {
    Class* cls = nullptr;
    if (!classTable.find(classId, cls) || cls == nullptr) {
        cout << "[Loi] Khong tim thay Lop hoc " << classId << "!\n";
        return false;
    }

    if (cls->getTutorId() != tutorId) {
        cout << "[Loi] Lop hoc " << classId << " khong thuoc quan ly cua Gia su " << tutorId << "!\n";
        return false;
    }

    if (cls->getStatus() != "PENDING") {
        cout << "[Loi] Lop hoc " << classId << " khong o trang thai cho duyet (PENDING)!\n";
        return false;
    }

    // Tạo hợp đồng với ID tự động
    string contractId = IdGenerator::generateContractId();
    Contract* newContract = new Contract(contractId, tutorId, cls->getStudentId(),
                                        cls->getStartDate(), cls->getEndDate(),
                                        cls->getSubject(), "Active");
    contractList.push_back(newContract);
    contractTable.insert(contractId, newContract);

    // Kích hoạt lớp học
    cls->setContractId(contractId);
    cls->setStatus("ACTIVE");

    // Cập nhật trạng thái học viên
    Student* st = findStudentById(cls->getStudentId());
    if (st != nullptr) {
        st->setHasTutor(true);
    }

    cout << "[Thanh Cong] Gia su da phe duyet lop " << classId 
         << ". Hop dong " << contractId << " da duoc khoi tao thanh cong!\n";
    return true;
}

bool CenterService::tutorRejectClass(const string& tutorId, const string& classId) {
    Class* cls = nullptr;
    if (!classTable.find(classId, cls) || cls == nullptr) {
        cout << "[Loi] Khong tim thay Lop hoc " << classId << "!\n";
        return false;
    }

    if (cls->getTutorId() != tutorId) {
        cout << "[Loi] Lop hoc " << classId << " khong thuoc ve Gia su " << tutorId << "!\n";
        return false;
    }

    if (cls->getStatus() != "PENDING") {
        cout << "[Loi] Lop hoc " << classId << " khong o trang thai cho duyet!\n";
        return false;
    }

    // Xóa liên kết học viên và mở lại ca học
    cls->setStudentId("");
    cls->setStatus("AVAILABLE");

    cout << "[Thong Bao] Gia su da tu choi yeu cau. Ca hoc " << classId 
         << " da duoc mo lai (AVAILABLE) cho hoc vien khac!\n";
    return true;
}

// ------------------- 6. Xem danh sách lớp của tôi -------------------

void CenterService::displayClassesByStudent(const string& studentId) const {
    cout << "\n===== DANH SACH LOP CUA HOC VIEN: " << studentId << " =====\n";
    int count = 0;
    Node<Class*>* cur = classList.getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getStudentId() == studentId) {
            count++;
            Tutor* t = findTutorById(c->getTutorId());
            string tutorName = (t != nullptr) ? t->getFullName() : "N/A";

            cout << "  [" << count << "] Ma Lop: " << c->getClassId()
                 << " | Mon: " << c->getSubject()
                 << " | Lich: " << c->getSchedule()
                 << " | Gia su: " << tutorName << " (" << c->getTutorId() << ")"
                 << " | Trang thai: " << c->getStatus();
            if (!c->getContractId().empty()) {
                cout << " | Ma HD: " << c->getContractId();
            }
            cout << "\n";
        }
        cur = cur->next;
    }
    if (count == 0) {
        cout << "  (Hoc vien chua dang ky lop nao)\n";
    }
}

void CenterService::displayClassesByTutor(const string& tutorId) const {
    cout << "\n===== DANH SACH LOP CUA GIA SU: " << tutorId << " =====\n";
    int count = 0;
    Node<Class*>* cur = classList.getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getTutorId() == tutorId) {
            count++;
            cout << "  [" << count << "] Ma Lop: " << c->getClassId()
                 << " | Mon: " << c->getSubject()
                 << " | Lich: " << c->getSchedule()
                 << " | Hoc phi: " << c->getTuitionFee() << " VND"
                 << " | Trang thai: " << c->getStatus();
            if (!c->getStudentId().empty()) {
                Student* st = findStudentById(c->getStudentId());
                string studentName = (st != nullptr) ? st->getFullName() : "N/A";
                cout << " | Hoc vien: " << studentName << " (" << c->getStudentId() << ")";
            }
            if (!c->getContractId().empty()) {
                cout << " | Ma HD: " << c->getContractId();
            }
            cout << "\n";
        }
        cur = cur->next;
    }
    if (count == 0) {
        cout << "  (Gia su chua mo ca hoc nao)\n";
    }
}

// ------------------- 7. Quản lý Hợp đồng & Toàn bộ lớp -------------------

Contract* CenterService::findContractById(const string& contractId) const {
    Contract* res = nullptr;
    if (contractTable.find(contractId, res)) return res;
    return nullptr;
}

Class* CenterService::findClassById(const string& classId) const {
    Class* res = nullptr;
    if (classTable.find(classId, res)) return res;
    return nullptr;
}

void CenterService::displayAllClasses() const {
    cout << "\n===== TOAN BO DANH SACH LOP HOC (" << classList.getSize() << ") =====\n";
    Node<Class*>* cur = classList.getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}

void CenterService::displayAllContracts() const {
    cout << "\n===== TOAN BO DANH SACH HOP DONG (" << contractList.getSize() << ") =====\n";
    Node<Contract*>* cur = contractList.getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}
