#include "../../include/services/CenterService.h"
#include "../../include/helper/Validator.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

CenterService::CenterService(StudentRepository& students, TutorRepository& tutors,
                             ClassRepository& classes, ContractRepository& contracts)
    : studentRepo(students), tutorRepo(tutors), classRepo(classes), contractRepo(contracts) {}

bool CenterService::nap_du_lieu() {
    bool found = false;
    found = studentRepo.load() || found;
    found = tutorRepo.load() || found;
    found = classRepo.load() || found;
    found = contractRepo.load() || found;
    return found;
}

namespace {

// lop con chiem lich
bool chiem_lich(const Class* c) {
    const string& st = c->getStatus();
    return st == "AVAILABLE" || st == "PENDING" || st == "ACTIVE";
}

}  // namespace

Class* CenterService::tim_trung_lich_gia_su(const string& tutorId, const Schedule& schedule) const {
    Node<Class*>* cur = classRepo.getAll().getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        if (c->getTutorId() == tutorId && chiem_lich(c)) {
            Schedule other;
            if (Schedule::doc_lich(c->getSchedule(), other, nullptr) && schedule.trung_lich(other)) {
                return c;
            }
        }
        cur = cur->next;
    }
    return nullptr;
}

Class* CenterService::tim_trung_lich_hoc_vien(const string& studentId, const Schedule& schedule) const {
    Node<Class*>* cur = classRepo.getAll().getHead();
    while (cur != nullptr) {
        Class* c = cur->data;
        // hoc vien chi bi chiem lich boi lop PENDING / ACTIVE
        if (c->getStudentId() == studentId &&
            (c->getStatus() == "PENDING" || c->getStatus() == "ACTIVE")) {
            Schedule other;
            if (Schedule::doc_lich(c->getSchedule(), other, nullptr) && schedule.trung_lich(other)) {
                return c;
            }
        }
        cur = cur->next;
    }
    return nullptr;
}

// hoc vien & gia su

bool CenterService::dam_bao_ho_so_hv(const string& id, const string& fullName,
                                         const string& phone, const string& email,
                                         string* outError) {
    if (id.empty()) {
        StringUtils::gan(outError, "Ma hoc vien rong!");
        return false;
    }
    if (studentRepo.findById(id) != nullptr) return true;

    Student* st = new Student(id, fullName, "", phone, email, "", "Nam", "", "", "", 0.0);
    if (!them_hoc_vien(st)) {
        delete st;
        StringUtils::gan(outError, "Thong tin tai khoan " + id + " khong hop le (ten/SDT/email)!");
        return false;
    }
    return true;
}

bool CenterService::dam_bao_ho_so_gs(const string& id, const string& fullName,
                                       const string& phone, const string& email,
                                       string* outError) {
    if (id.empty()) {
        StringUtils::gan(outError, "Ma gia su rong!");
        return false;
    }
    if (tutorRepo.findById(id) != nullptr) return true;

    Tutor* tu = new Tutor(id, fullName, phone, email, "", "Nam", "", "", "", 0, 0.0, "");
    if (!them_gia_su(tu)) {
        delete tu;
        StringUtils::gan(outError, "Thong tin tai khoan " + id + " khong hop le (ten/SDT/email)!");
        return false;
    }
    return true;
}

bool CenterService::them_hoc_vien(Student* student) {
    if (student == nullptr) return false;

    // kiem tra du lieu
    if (!Validator::khong_rong(student->getFullName())) {
        return false;
    }
    if (!Validator::sdt_hop_le(student->getPhone())) {
        return false;
    }
    if (!Validator::email_hop_le(student->getEmail())) {
        return false;
    }

    // sinh ID neu chua co
    if (student->getId().empty()) {
        student->setId(IdGenerator::generateStudentId());
    } else {
        // dong bo bo dem de khong trung ID
        IdGenerator::dong_bo_ma(student->getId());
    }

    if (studentRepo.findById(student->getId()) != nullptr) {
        return false;
    }
    return studentRepo.add(student);
}

bool CenterService::them_gia_su(Tutor* tutor) {
    if (tutor == nullptr) return false;

    // kiem tra du lieu
    if (!Validator::khong_rong(tutor->getFullName())) {
        return false;
    }
    if (!Validator::sdt_hop_le(tutor->getPhone())) {
        return false;
    }
    if (!Validator::email_hop_le(tutor->getEmail())) {
        return false;
    }
    if (!Validator::kinh_nghiem_hop_le(tutor->getExpYears())) {
        return false;
    }

    // sinh ID neu chua co
    if (tutor->getId().empty()) {
        tutor->setId(IdGenerator::generateTutorId());
    } else {
        IdGenerator::dong_bo_ma(tutor->getId());
    }

    if (tutorRepo.findById(tutor->getId()) != nullptr) {
        return false;
    }
    return tutorRepo.add(tutor);
}

namespace {

// kiem tra phan chung cua ho so, tra ve "" neu hop le
string kiem_tra_ho_so(const string& fullName, const string& phone, const string& email,
                             const string& gender, const string& dob) {
    if (!Validator::khong_rong(fullName)) return "Ho va ten khong duoc de trong!";
    if (!Validator::ten_hop_le(fullName)) return "Ho va ten chi duoc chua chu cai va khoang trang!";
    if (!Validator::sdt_hop_le(phone)) return Validator::loi_sdt(phone);
    if (!Validator::khong_rong(email)) return "Email khong duoc de trong!";
    if (!Validator::email_hop_le(email)) {
        string why = Validator::loi_email(email);
        return why.empty() ? "Email khong hop le!" : why;
    }
    if (!gender.empty() && !Validator::gioi_tinh_hop_le(gender)) return "Gioi tinh chi nhan 'Nam' hoac 'Nu'!";
    if (!dob.empty() && !Validator::ngay_hop_le(dob)) return Validator::loi_ngay(dob);
    return "";
}

}  // namespace

bool CenterService::sua_ho_so_hv(const string& id, const StudentProfile& p, string* outError) {
    Student* st = tim_hoc_vien(id);
    if (st == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Hoc vien " + id + "!");
        return false;
    }
    string err = kiem_tra_ho_so(p.fullName, p.phone, p.email, p.gender, p.dateOfBirth);
    if (err.empty() && p.budget < 0) err = "Hoc phi mong muon khong duoc am!";
    if (!err.empty()) {
        StringUtils::gan(outError, err);
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

bool CenterService::sua_ho_so_gs(const string& id, const TutorProfile& p, string* outError) {
    Tutor* tu = tim_gia_su(id);
    if (tu == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Gia su " + id + "!");
        return false;
    }
    string err = kiem_tra_ho_so(p.fullName, p.phone, p.email, p.gender, p.dateOfBirth);
    if (err.empty() && !Validator::kinh_nghiem_hop_le(p.expYears)) err = "So nam kinh nghiem phai >= 0!";
    if (!err.empty()) {
        StringUtils::gan(outError, err);
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

Student* CenterService::tim_hoc_vien(const string& id) const {
    return studentRepo.findById(id);
}

Tutor* CenterService::tim_gia_su(const string& id) const {
    return tutorRepo.findById(id);
}

// gia su mo ca

bool CenterService::mo_lop(const string& tutorId, const string& subject,
                                   const string& schedule, double tuitionFee,
                                   const string& startDate, const string& endDate,
                                   string* outClassId, string* outError) {
    if (tutorRepo.findById(tutorId) == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Gia su co ma " + tutorId + "!");
        return false;
    }

    Schedule parsed;
    string scheduleErr;
    if (!Schedule::doc_lich(schedule, parsed, &scheduleErr)) {
        StringUtils::gan(outError, scheduleErr);
        return false;
    }
    if (!Validator::hoc_phi_hop_le(tuitionFee)) {
        StringUtils::gan(outError, "Hoc phi phai lon hon 0!");
        return false;
    }
    if (!Validator::khong_rong(subject)) {
        StringUtils::gan(outError, "Ten mon hoc khong duoc de trong!");
        return false;
    }
    if (StringUtils::co_ky_tu(subject, "|\r\n")) {
        StringUtils::gan(outError, "Ten mon hoc khong duoc chua ky tu '|' hoac xuong dong!");
        return false;
    }

    // ngay bat dau / ket thuc co thi phai hop le, bat dau <= ket thuc
    if ((!startDate.empty() && !Validator::ngay_hop_le(startDate)) ||
        (!endDate.empty() && !Validator::ngay_hop_le(endDate))) {
        StringUtils::gan(outError, "Ngay bat dau/ket thuc khong hop le (dung dd/mm/yyyy)!");
        return false;
    }
    if (!startDate.empty() && !endDate.empty() && Validator::so_sanh_ngay(endDate, startDate) < 0) {
        StringUtils::gan(outError, "Ngay ket thuc phai sau ngay bat dau!");
        return false;
    }

    // gia su khong day 2 ca cung luc
    Class* conflict = tim_trung_lich_gia_su(tutorId, parsed);
    if (conflict != nullptr) {
        string msg = "Lich bi trung voi ca " + conflict->getClassId() + " (" +
                     conflict->getSubject() + ": " + conflict->getSchedule() + ") cua ban!";
        StringUtils::gan(outError, msg);
        return false;
    }

    // sinh ma lop, luu lich dang chuan
    string classId = IdGenerator::generateClassId();
    if (outClassId != nullptr) *outClassId = classId;

    Class* newClass = new Class(classId, "", "", tutorId, subject, parsed.chuoi_luu(),
                                startDate, endDate, "AVAILABLE", tuitionFee);
    classRepo.add(newClass);

    return true;
}

// hoc vien dat ca

bool CenterService::dang_ky_lop(const string& studentId, const string& classId,
                                     string* outError) {
    if (studentRepo.findById(studentId) == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Hoc vien " + studentId + "!");
        return false;
    }

    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Lop hoc " + classId + "!");
        return false;
    }

    if (cls->getStatus() != "AVAILABLE") {
        StringUtils::gan(outError, "Ca hoc " + classId + " da duoc nguoi khac chon hoac dang hoat dong!");
        return false;
    }

    // hoc vien khong hoc 2 ca cung luc
    Schedule wanted;
    if (Schedule::doc_lich(cls->getSchedule(), wanted, nullptr)) {
        Class* conflict = tim_trung_lich_hoc_vien(studentId, wanted);
        if (conflict != nullptr) {
            string msg = "Lich bi trung voi lop " + conflict->getClassId() + " (" +
                         conflict->getSubject() + ": " + conflict->getSchedule() + ") ban da dang ky!";
            StringUtils::gan(outError, msg);
            return false;
        }
    }

    // chuyen PENDING va gan hoc vien
    cls->setStudentId(studentId);
    cls->setStatus("PENDING");

    classRepo.update(cls);
    return true;
}

// gia su duyet / tu choi

bool CenterService::duyet_lop(const string& tutorId, const string& classId) {
    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
        return false;
    }

    if (cls->getTutorId() != tutorId) {
        return false;
    }

    if (cls->getStatus() != "PENDING") {
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
    Student* st = tim_hoc_vien(cls->getStudentId());
    if (st != nullptr) {
        st->setHasTutor(true);
        studentRepo.update(st);
    }

    classRepo.update(cls);
    return true;
}

bool CenterService::tu_choi_lop(const string& tutorId, const string& classId) {
    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
        return false;
    }

    if (cls->getTutorId() != tutorId) {
        return false;
    }

    if (cls->getStatus() != "PENDING") {
        return false;
    }

    // bo hoc vien, mo lai ca
    cls->setStudentId("");
    cls->setStatus("AVAILABLE");

    classRepo.update(cls);
    return true;
}

bool CenterService::xoa_lop_trong(const string& tutorId, const string& classId, string* outError) {
    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Lop hoc " + classId + "!");
        return false;
    }
    if (cls->getTutorId() != tutorId) {
        StringUtils::gan(outError, "Lop hoc " + classId + " khong thuoc ve Gia su " + tutorId + "!");
        return false;
    }
    // chi xoa lop chua lien quan toi hoc vien nao
    if (cls->getStatus() != "AVAILABLE") {
        StringUtils::gan(outError, "Chi xoa duoc lop trong (AVAILABLE), lop " + classId +
                                      " dang o trang thai " + cls->getStatus() + "!");
        return false;
    }
    classRepo.remove(classId);
    return true;
}

bool CenterService::rut_dang_ky(const string& studentId, const string& classId, string* outError) {
    Class* cls = classRepo.findById(classId);
    if (cls == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Lop hoc " + classId + "!");
        return false;
    }
    if (cls->getStudentId() != studentId) {
        StringUtils::gan(outError, "Ban khong dang ky lop " + classId + "!");
        return false;
    }
    if (cls->getStatus() != "PENDING") {
        StringUtils::gan(outError, "Chi rut duoc lop dang cho duyet (PENDING)!");
        return false;
    }
    // tra lop ve trong cho nguoi khac
    cls->setStudentId("");
    cls->setStatus("AVAILABLE");
    classRepo.update(cls);
    return true;
}

// tra cuu lop & hop dong

Contract* CenterService::tim_hop_dong(const string& contractId) const {
    return contractRepo.findById(contractId);
}

Class* CenterService::tim_lop(const string& classId) const {
    return classRepo.findById(classId);
}

// ket thuc hop dong

Contract* CenterService::tim_hop_dong_cua(const string& personId, const string& contractId,
                                         const string& status, string* outError) const {
    Contract* ct = contractRepo.findById(contractId);
    if (ct == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Hop dong " + contractId + "!");
        return nullptr;
    }
    if (personId.empty() || (ct->getTutorId() != personId && ct->getStudentId() != personId)) {
        StringUtils::gan(outError, "Ban khong phai mot ben cua hop dong " + contractId + "!");
        return nullptr;
    }
    if (ct->getStatus() != status) {
        StringUtils::gan(outError, "Hop dong " + contractId + " dang o trang thai " +
                                      ct->getStatus() + ", can " + status + "!");
        return nullptr;
    }
    return ct;
}

void CenterService::ket_thuc_hop_dong(Contract* ct) {
    ct->setStatus("Ended");
    ct->setEndedOn(Validator::hom_nay());
    contractRepo.update(ct);

    // lop cua hop dong: luu lich su, giai phong lich
    for (Class* c : classRepo.toVector()) {
        if (c->getContractId() == ct->getContractId() && c->getStatus() == "ACTIVE") {
            c->setStatus("ENDED");
            classRepo.update(c);
        }
    }

    Student* st = tim_hoc_vien(ct->getStudentId());
    if (st != nullptr) {
        st->setHasTutor(con_hop_dong_mo(st->getId()));
        studentRepo.update(st);
    }
}

bool CenterService::yeu_cau_ket_thuc(const string& personId, const string& contractId,
                                       const string& reason, string* outError) {
    string why = StringUtils::trim(reason);
    if (!Validator::khong_rong(why)) {
        StringUtils::gan(outError, "Vui long nhap ly do ket thuc hop dong!");
        return false;
    }
    Contract* ct = tim_hop_dong_cua(personId, contractId, "Active", outError);
    if (ct == nullptr) return false;

    ct->setStatus("EndRequested");
    ct->setRequestedBy(personId);
    ct->setEndReason(why);
    contractRepo.update(ct);
    return true;
}

bool CenterService::dong_y_ket_thuc(const string& personId, const string& contractId, string* outError) {
    Contract* ct = tim_hop_dong_cua(personId, contractId, "EndRequested", outError);
    if (ct == nullptr) return false;
    // nguoi yeu cau khong tu dong y
    if (ct->getRequestedBy() == personId) {
        StringUtils::gan(outError, "Can ben con lai dong y ket thuc hop dong!");
        return false;
    }
    ket_thuc_hop_dong(ct);
    return true;
}

bool CenterService::huy_yeu_cau_ket_thuc(const string& personId, const string& contractId, string* outError) {
    Contract* ct = tim_hop_dong_cua(personId, contractId, "EndRequested", outError);
    if (ct == nullptr) return false;
    // nguoi yeu cau rut lai, hoac ben con lai tu choi
    ct->setStatus("Active");
    ct->setRequestedBy("");
    ct->setEndReason("");
    contractRepo.update(ct);
    return true;
}

bool CenterService::admin_ket_thuc(const string& contractId, const string& reason, string* outError) {
    Contract* ct = contractRepo.findById(contractId);
    if (ct == nullptr) {
        StringUtils::gan(outError, "Khong tim thay Hop dong " + contractId + "!");
        return false;
    }
    if (ct->getStatus() != "Active" && ct->getStatus() != "EndRequested") {
        StringUtils::gan(outError, "Hop dong " + contractId + " da ket thuc!");
        return false;
    }
    string why = StringUtils::trim(reason);
    ct->setRequestedBy("ADMIN");
    ct->setEndReason(why.empty() ? "Admin ket thuc" : why);
    ket_thuc_hop_dong(ct);
    return true;
}

bool CenterService::con_hop_dong_mo(const string& personId) const {
    for (Contract* ct : contractRepo.toVector()) {
        bool party = ct->getTutorId() == personId || ct->getStudentId() == personId;
        if (party && (ct->getStatus() == "Active" || ct->getStatus() == "EndRequested")) return true;
    }
    return false;
}

// xoa ho so

namespace {

// dem hop dong da ket thuc cua 1 nguoi
int dem_hop_dong_da_ket_thuc(const vector<Contract*>& contracts, const string& personId) {
    int n = 0;
    for (Contract* ct : contracts) {
        bool party = ct->getTutorId() == personId || ct->getStudentId() == personId;
        if (party && ct->getStatus() == "Ended") n++;
    }
    return n;
}

}  // namespace

DeleteImpact CenterService::xem_truoc_xoa_hv(const string& id) const {
    DeleteImpact impact;
    if (tim_hoc_vien(id) == nullptr) {
        impact.reason = "Khong tim thay Hoc vien " + id + "!";
        return impact;
    }
    if (con_hop_dong_mo(id)) {
        impact.reason = "Hoc vien " + id + " con hop dong dang hieu luc, can ket thuc hop dong truoc!";
        return impact;
    }
    for (Class* c : classRepo.toVector()) {
        if (c->getStudentId() == id && c->getStatus() == "PENDING") impact.releasedClasses.push_back(c->getClassId());
    }
    impact.keptHistory = dem_hop_dong_da_ket_thuc(contractRepo.toVector(), id);
    impact.allowed = true;
    return impact;
}

DeleteImpact CenterService::xem_truoc_xoa_gs(const string& id) const {
    DeleteImpact impact;
    if (tim_gia_su(id) == nullptr) {
        impact.reason = "Khong tim thay Gia su " + id + "!";
        return impact;
    }
    if (con_hop_dong_mo(id)) {
        impact.reason = "Gia su " + id + " con hop dong dang hieu luc, can ket thuc hop dong truoc!";
        return impact;
    }
    for (Class* c : classRepo.toVector()) {
        bool open = c->getStatus() == "AVAILABLE" || c->getStatus() == "PENDING";
        if (c->getTutorId() == id && open) impact.deletedClasses.push_back(c->getClassId());
    }
    impact.keptHistory = dem_hop_dong_da_ket_thuc(contractRepo.toVector(), id);
    impact.allowed = true;
    return impact;
}

void CenterService::tra_lop_cua_hv(const string& id) {
    for (const string& classId : xem_truoc_xoa_hv(id).releasedClasses) {
        Class* c = classRepo.findById(classId);
        c->setStudentId("");
        c->setStatus("AVAILABLE");
        classRepo.update(c);
    }
}

void CenterService::xoa_lop_mo_cua_gs(const string& id) {
    for (const string& classId : xem_truoc_xoa_gs(id).deletedClasses) classRepo.remove(classId);
}

bool CenterService::xoa_ho_so_hv(const string& id) {
    if (con_hop_dong_mo(id)) return false;
    return studentRepo.remove(id);
}

bool CenterService::xoa_ho_so_gs(const string& id) {
    if (con_hop_dong_mo(id)) return false;
    return tutorRepo.remove(id);
}
