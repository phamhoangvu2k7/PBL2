#include "../../include/services/CenterService.h"
#include "../../include/helper/Validator.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

CenterService::CenterService(StudentRepository& students, TutorRepository& tutors,
                             ClassRepository& classes, ContractRepository& contracts)
    : studentRepo(students), tutorRepo(tutors), classRepo(classes), contractRepo(contracts) {}

bool CenterService::loadData() {
    bool found = false;
    found = studentRepo.load() || found;
    found = tutorRepo.load() || found;
    found = classRepo.load() || found;
    found = contractRepo.load() || found;
    return found;
}

namespace {

// lop con chiem lich
bool occupiesSchedule(const Class* c) {
    const string& st = c->getStatus();
    return st == "AVAILABLE" || st == "PENDING" || st == "ACTIVE";
}

}  // namespace

Class* CenterService::findTutorConflict(const string& tutorId, const Schedule& schedule) const {
    Node<Class*>* cur = classRepo.getAll().getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getTutorId() == tutorId && occupiesSchedule(c)) {
            Schedule other;
            if (Schedule::parse(c->getSchedule(), other, nullptr) && schedule.overlaps(other)) {
                return c;
            }
        }
        cur = cur->next;
    }
    return nullptr;
}

Class* CenterService::findStudentConflict(const string& studentId, const Schedule& schedule) const {
    Node<Class*>* cur = classRepo.getAll().getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        // hoc vien chi bi chiem lich boi lop PENDING / ACTIVE
        if (c->getStudentId() == studentId &&
            (c->getStatus() == "PENDING" || c->getStatus() == "ACTIVE")) {
            Schedule other;
            if (Schedule::parse(c->getSchedule(), other, nullptr) && schedule.overlaps(other)) {
                return c;
            }
        }
        cur = cur->next;
    }
    return nullptr;
}

// hoc vien & gia su

bool CenterService::ensureStudentProfile(const string& id, const string& fullName,
                                         const string& phone, const string& email,
                                         string* outError) {
    if (id.empty()) {
        StringUtils::assign(outError, "Ma hoc vien rong!");
        return false;
    }
    if (studentRepo.findById(id) != nullptr) return true;

    Student* st = new Student(id, fullName, "", phone, email, "", "Nam", "", "", "", 0.0);
    if (!addStudent(st)) {
        delete st;
        StringUtils::assign(outError, "Thong tin tai khoan " + id + " khong hop le (ten/SDT/email)!");
        return false;
    }
    return true;
}

bool CenterService::ensureTutorProfile(const string& id, const string& fullName,
                                       const string& phone, const string& email,
                                       string* outError) {
    if (id.empty()) {
        StringUtils::assign(outError, "Ma gia su rong!");
        return false;
    }
    if (tutorRepo.findById(id) != nullptr) return true;

    Tutor* tu = new Tutor(id, fullName, phone, email, "", "Nam", "", "", "", 0, 0.0, "");
    if (!addTutor(tu)) {
        delete tu;
        StringUtils::assign(outError, "Thong tin tai khoan " + id + " khong hop le (ten/SDT/email)!");
        return false;
    }
    return true;
}

bool CenterService::addStudent(Student* student) {
    if (student == nullptr) return false;

    // kiem tra du lieu
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

    // sinh ID neu chua co
    if (student->getId().empty()) {
        student->setId(IdGenerator::generateStudentId());
    } else {
        // dong bo bo dem de khong trung ID
        IdGenerator::syncFromExistingId(student->getId());
    }

    if (studentRepo.findById(student->getId()) != nullptr) {
        cout << "[Loi] Ma hoc vien " << student->getId() << " da ton tai!\n";
        return false;
    }
    return studentRepo.add(student);
}

bool CenterService::addTutor(Tutor* tutor) {
    if (tutor == nullptr) return false;

    // kiem tra du lieu
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

    // sinh ID neu chua co
    if (tutor->getId().empty()) {
        tutor->setId(IdGenerator::generateTutorId());
    } else {
        IdGenerator::syncFromExistingId(tutor->getId());
    }

    if (tutorRepo.findById(tutor->getId()) != nullptr) {
        cout << "[Loi] Ma gia su " << tutor->getId() << " da ton tai!\n";
        return false;
    }
    return tutorRepo.add(tutor);
}

namespace {

// kiem tra phan chung cua ho so, tra ve "" neu hop le
string validateCommonProfile(const string& fullName, const string& phone, const string& email,
                             const string& gender, const string& dob) {
    if (!Validator::isNotEmpty(fullName)) return "Ho va ten khong duoc de trong!";
    if (!Validator::isValidName(fullName)) return "Ho va ten chi duoc chua chu cai va khoang trang!";
    if (!Validator::isValidPhone(phone)) return Validator::getPhoneError(phone);
    if (!Validator::isNotEmpty(email)) return "Email khong duoc de trong!";
    if (!Validator::isValidEmail(email)) {
        string why = Validator::getEmailError(email);
        return why.empty() ? "Email khong hop le!" : why;
    }
    if (!gender.empty() && !Validator::isValidGender(gender)) return "Gioi tinh chi nhan 'Nam' hoac 'Nu'!";
    if (!dob.empty() && !Validator::isValidDate(dob)) return Validator::getDateError(dob);
    return "";
}

}  // namespace

bool CenterService::updateStudentProfile(const string& id, const StudentProfile& p, string* outError) {
    Student* st = findStudentById(id);
    if (st == nullptr) {
        StringUtils::assign(outError, "Khong tim thay Hoc vien " + id + "!");
        return false;
    }
    string err = validateCommonProfile(p.fullName, p.phone, p.email, p.gender, p.dateOfBirth);
    if (err.empty() && p.budget < 0) err = "Hoc phi mong muon khong duoc am!";
    if (!err.empty()) {
        StringUtils::assign(outError, err);
        return false;
    }

    st->setFullName(p.fullName);
    st->setPhone(p.phone);
    st->setEmail(p.email);
    st->setAddress(p.address);
    st->setGender(p.gender);
    st->setDateOfBirth(p.dateOfBirth);
    st->setGrade(p.grade);
    st->setSubjects(p.subjects);
    st->setSchedule(p.schedule);
    st->setBudget(p.budget);
    studentRepo.update(st);
    return true;
}

bool CenterService::updateTutorProfile(const string& id, const TutorProfile& p, string* outError) {
    Tutor* tu = findTutorById(id);
    if (tu == nullptr) {
        StringUtils::assign(outError, "Khong tim thay Gia su " + id + "!");
        return false;
    }
    string err = validateCommonProfile(p.fullName, p.phone, p.email, p.gender, p.dateOfBirth);
    if (err.empty() && !Validator::isValidExpYears(p.expYears)) err = "So nam kinh nghiem phai >= 0!";
    if (!err.empty()) {
        StringUtils::assign(outError, err);
        return false;
    }

    tu->setFullName(p.fullName);
    tu->setPhone(p.phone);
    tu->setEmail(p.email);
    tu->setAddress(p.address);
    tu->setGender(p.gender);
    tu->setDateOfBirth(p.dateOfBirth);
    tu->setSubjects(p.subjects);
    tu->setQualification(p.qualification);
    tu->setAchievements(p.achievements);
    tu->setSchedule(p.schedule);
    tu->setExpYears(p.expYears);
    tutorRepo.update(tu);
    return true;
}

Student* CenterService::findStudentById(const string& id) const {
    return studentRepo.findById(id);
}

Tutor* CenterService::findTutorById(const string& id) const {
    return tutorRepo.findById(id);
}

void CenterService::displayAllStudents() const {
    cout << "\n===== DANH SACH HOC VIEN (" << studentRepo.size() << ") =====\n";
    Node<Student*>* cur = studentRepo.getAll().getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}

void CenterService::displayAllTutors() const {
    cout << "\n===== DANH SACH GIA SU (" << tutorRepo.size() << ") =====\n";
    Node<Tutor*>* cur = tutorRepo.getAll().getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}

// gia su mo ca

bool CenterService::tutorOpenClass(const string& tutorId, const string& subject,
                                   const string& schedule, double tuitionFee,
                                   const string& startDate, const string& endDate,
                                   string* outClassId, string* outError) {
    if (tutorRepo.findById(tutorId) == nullptr) {
        cout << "[Loi] Khong tim thay Gia su co ma " << tutorId << "!\n";
        StringUtils::assign(outError, "Khong tim thay Gia su co ma " + tutorId + "!");
        return false;
    }

    Schedule parsed;
    string scheduleErr;
    if (!Schedule::parse(schedule, parsed, &scheduleErr)) {
        cout << "[Loi] Lich hoc khong hop le! " << scheduleErr << "\n";
        StringUtils::assign(outError, scheduleErr);
        return false;
    }
    if (!Validator::isValidFee(tuitionFee)) {
        cout << "[Loi] Hoc phi phai lon hon 0!\n";
        StringUtils::assign(outError, "Hoc phi phai lon hon 0!");
        return false;
    }
    if (!Validator::isNotEmpty(subject)) {
        cout << "[Loi] Ten mon hoc khong duoc de trong!\n";
        StringUtils::assign(outError, "Ten mon hoc khong duoc de trong!");
        return false;
    }
    if (StringUtils::containsAny(subject, "|\r\n")) {
        cout << "[Loi] Ten mon hoc khong duoc chua ky tu '|' hoac xuong dong!\n";
        StringUtils::assign(outError, "Ten mon hoc khong duoc chua ky tu '|' hoac xuong dong!");
        return false;
    }

    // ngay bat dau / ket thuc co thi phai hop le, bat dau <= ket thuc
    if ((!startDate.empty() && !Validator::isValidDate(startDate)) ||
        (!endDate.empty() && !Validator::isValidDate(endDate))) {
        cout << "[Loi] Ngay bat dau/ket thuc khong hop le (dd/mm/yyyy)!\n";
        StringUtils::assign(outError, "Ngay bat dau/ket thuc khong hop le (dung dd/mm/yyyy)!");
        return false;
    }
    if (!startDate.empty() && !endDate.empty() && Validator::compareDates(endDate, startDate) < 0) {
        cout << "[Loi] Ngay ket thuc phai sau ngay bat dau!\n";
        StringUtils::assign(outError, "Ngay ket thuc phai sau ngay bat dau!");
        return false;
    }

    // gia su khong day 2 ca cung luc
    Class* conflict = findTutorConflict(tutorId, parsed);
    if (conflict != nullptr) {
        string msg = "Lich bi trung voi ca " + conflict->getClassId() + " (" +
                     conflict->getSubject() + ": " + conflict->getSchedule() + ") cua ban!";
        cout << "[Loi] " << msg << "\n";
        StringUtils::assign(outError, msg);
        return false;
    }

    // sinh ma lop, luu lich dang chuan
    string classId = IdGenerator::generateClassId();
    if (outClassId != nullptr) *outClassId = classId;

    Class* newClass = new Class(classId, "", "", tutorId, subject, parsed.toString(),
                                startDate, endDate, "AVAILABLE", tuitionFee);
    classRepo.add(newClass);

    cout << "[Thanh Cong] Gia su " << tutorId << " da mo thanh cong ca hoc "
         << classId << " (Mon: " << subject << " | Lich: " << schedule << ")\n";
    return true;
}

// tim ca con trong

void CenterService::displayAvailableClassesBySubject(const string& subject) const {
    cout << "\n===== KHUNG GIO HOC CON TRONG THEO MON: \"" << subject << "\" =====\n";
    int count = 0;
    Node<Class*>* cur = classRepo.getAll().getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getStatus() == "AVAILABLE" && StringUtils::containsIgnoreCase(c->getSubject(), subject)) {
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

// hoc vien dat ca

bool CenterService::studentBookClass(const string& studentId, const string& classId,
                                     string* outError) {
    if (studentRepo.findById(studentId) == nullptr) {
        cout << "[Loi] Khong tim thay Hoc vien " << studentId << "!\n";
        StringUtils::assign(outError, "Khong tim thay Hoc vien " + studentId + "!");
        return false;
    }

    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
        cout << "[Loi] Khong tim thay Lop hoc " << classId << "!\n";
        StringUtils::assign(outError, "Khong tim thay Lop hoc " + classId + "!");
        return false;
    }

    if (cls->getStatus() != "AVAILABLE") {
        cout << "[Loi] Ca hoc " << classId << " da duoc nguoi khac chon hoac dang hoat dong!\n";
        StringUtils::assign(outError, "Ca hoc " + classId + " da duoc nguoi khac chon hoac dang hoat dong!");
        return false;
    }

    // hoc vien khong hoc 2 ca cung luc
    Schedule wanted;
    if (Schedule::parse(cls->getSchedule(), wanted, nullptr)) {
        Class* conflict = findStudentConflict(studentId, wanted);
        if (conflict != nullptr) {
            string msg = "Lich bi trung voi lop " + conflict->getClassId() + " (" +
                         conflict->getSubject() + ": " + conflict->getSchedule() + ") ban da dang ky!";
            cout << "[Loi] " << msg << "\n";
            StringUtils::assign(outError, msg);
            return false;
        }
    }

    // chuyen PENDING va gan hoc vien
    cls->setStudentId(studentId);
    cls->setStatus("PENDING");

    cout << "[Thanh Cong] Hoc vien " << studentId << " da dang ky ca hoc " << classId 
         << ". Ca hoc da tam khoa cho Gia su xac nhan!\n";
    classRepo.update(cls);
    return true;
}

// gia su duyet / tu choi

void CenterService::displayPendingRequestsByTutor(const string& tutorId) const {
    cout << "\n===== YEU CAU CHO DUYET CUA GIA SU " << tutorId << " =====\n";
    int count = 0;
    Node<Class*>* cur = classRepo.getAll().getHead();
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
    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
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

    // tao hop dong
    string contractId = IdGenerator::generateContractId();
    Contract* newContract = new Contract(contractId, tutorId, cls->getStudentId(),
                                        cls->getStartDate(), cls->getEndDate(),
                                        cls->getSubject(), "Active");
    contractRepo.add(newContract);

    // kich hoat lop
    cls->setContractId(contractId);
    cls->setStatus("ACTIVE");

    // danh dau hoc vien da co gia su
    Student* st = findStudentById(cls->getStudentId());
    if (st != nullptr) {
        st->setHasTutor(true);
        studentRepo.update(st);
    }

    cout << "[Thanh Cong] Gia su da phe duyet lop " << classId 
         << ". Hop dong " << contractId << " da duoc khoi tao thanh cong!\n";
    classRepo.update(cls);
    return true;
}

bool CenterService::tutorRejectClass(const string& tutorId, const string& classId) {
    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
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

    // bo hoc vien, mo lai ca
    cls->setStudentId("");
    cls->setStatus("AVAILABLE");

    cout << "[Thong Bao] Gia su da tu choi yeu cau. Ca hoc " << classId 
         << " da duoc mo lai (AVAILABLE) cho hoc vien khac!\n";
    classRepo.update(cls);
    return true;
}

// lop cua toi

void CenterService::displayClassesByStudent(const string& studentId) const {
    cout << "\n===== DANH SACH LOP CUA HOC VIEN: " << studentId << " =====\n";
    int count = 0;
    Node<Class*>* cur = classRepo.getAll().getHead();
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
    Node<Class*>* cur = classRepo.getAll().getHead();
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

// hop dong & tat ca lop

Contract* CenterService::findContractById(const string& contractId) const {
    return contractRepo.findById(contractId);
}

Class* CenterService::findClassById(const string& classId) const {
    return classRepo.findById(classId);
}

void CenterService::displayAllClasses() const {
    cout << "\n===== TOAN BO DANH SACH LOP HOC (" << classRepo.size() << ") =====\n";
    Node<Class*>* cur = classRepo.getAll().getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}

void CenterService::displayAllContracts() const {
    cout << "\n===== TOAN BO DANH SACH HOP DONG (" << contractRepo.size() << ") =====\n";
    Node<Contract*>* cur = contractRepo.getAll().getHead();
    while (cur != nullptr) {
        cur->data->display();
        cur = cur->next;
    }
}
