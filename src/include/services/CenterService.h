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

// hau qua khi xoa 1 nguoi, de admin xem truoc
struct DeleteImpact {
    bool allowed = false;
    string reason;                   // ly do bi chan
    vector<string> deletedClasses;   // lop bi xoa
    vector<string> releasedClasses;  // lop PENDING tra ve AVAILABLE
    vector<string> deletedAccounts;  // username bi xoa
    int keptHistory = 0;             // so hop dong Ended giu lai
};

class CenterService {
private:
    StudentRepository& studentRepo;
    TutorRepository& tutorRepo;
    ClassRepository& classRepo;
    ContractRepository& contractRepo;

    Class* tim_trung_lich_gia_su(const string& tutorId, const Schedule& schedule) const;
    Class* tim_trung_lich_hoc_vien(const string& studentId, const Schedule& schedule) const;
    // tim hop dong ma personId la 1 ben, kiem tra trang thai
    Contract* tim_hop_dong_cua(const string& personId, const string& contractId,
                              const string& status, string* outError) const;
    // chuyen hop dong sang Ended, lop sang ENDED, tinh lai hasTutor
    void ket_thuc_hop_dong(Contract* contract);

public:
    //DI
    CenterService(StudentRepository& students, TutorRepository& tutors,
                  ClassRepository& classes, ContractRepository& contracts);

    bool nap_du_lieu();

    // 1. them hoc vien , gia su
    bool them_hoc_vien(Student* student);
    bool them_gia_su(Tutor* tutor);

    // ktr tk đã đk chưa
    bool dam_bao_ho_so_hv(const string& id, const string& fullName,
                              const string& phone, const string& email,
                              string* outError = nullptr);
    bool dam_bao_ho_so_gs(const string& id, const string& fullName,
                            const string& phone, const string& email,
                            string* outError = nullptr);
    // sua ho so 
    bool sua_ho_so_hv(const string& id, const StudentProfile& p, string* outError = nullptr);
    bool sua_ho_so_gs(const string& id, const TutorProfile& p, string* outError = nullptr);

    Student* tim_hoc_vien(const string& id) const;
    Tutor* tim_gia_su(const string& id) const;


    // 2. gia su mo ca 
    bool mo_lop(const string& tutorId, const string& subject,
                        const string& schedule, double tuitionFee,
                        const string& startDate = "", const string& endDate = "",
                        string* outClassId = nullptr, string* outError = nullptr);

    // 3. hoc vien dat ca
    // tu choi neu trung lich lop PENDING/ACTIVE cua hoc vien
    bool dang_ky_lop(const string& studentId, const string& classId,
                          string* outError = nullptr);

    // 4. gia su xem yeu cau, duyet / tu choi
    bool duyet_lop(const string& tutorId, const string& classId);
    bool tu_choi_lop(const string& tutorId, const string& classId);

    // gia su xoa lop trong (AVAILABLE) cua minh
    bool xoa_lop_trong(const string& tutorId, const string& classId, string* outError = nullptr);
    // hoc vien rut dang ky lop dang cho duyet (PENDING)
    bool rut_dang_ky(const string& studentId, const string& classId, string* outError = nullptr);

    // 5. tra cuu lop & hop dong
    Contract* tim_hop_dong(const string& contractId) const;
    Class* tim_lop(const string& classId) const;

    // 6. ket thuc hop dong: Active -> EndRequested -> Ended
    // 1 ben yeu cau (kem ly do), ben con lai dong y / tu choi, nguoi yeu cau co the rut
    bool yeu_cau_ket_thuc(const string& personId, const string& contractId,
                            const string& reason, string* outError = nullptr);
    bool dong_y_ket_thuc(const string& personId, const string& contractId, string* outError = nullptr);
    bool huy_yeu_cau_ket_thuc(const string& personId, const string& contractId, string* outError = nullptr);
    // admin ket thuc ngay (xu ly tranh chap)
    bool admin_ket_thuc(const string& contractId, const string& reason, string* outError = nullptr);
    // con hop dong Active / EndRequested khong
    bool con_hop_dong_mo(const string& personId) const;

    // 7. xoa ho so (AdminService goi theo thu tu: lop -> tai khoan -> ho so)
    // bi chan neu con hop dong Active / EndRequested; lop ENDED, hop dong Ended giu lai
    DeleteImpact xem_truoc_xoa_hv(const string& id) const;
    DeleteImpact xem_truoc_xoa_gs(const string& id) const;
    void tra_lop_cua_hv(const string& id);   // lop PENDING -> AVAILABLE
    void xoa_lop_mo_cua_gs(const string& id);  // xoa lop AVAILABLE / PENDING
    bool xoa_ho_so_hv(const string& id);
    bool xoa_ho_so_gs(const string& id);

    // 8. getter cho GUI
    vector<Student*> ds_hoc_vien() const { return studentRepo.toVector(); }
    vector<Tutor*> ds_gia_su() const { return tutorRepo.toVector(); }
    vector<Class*> ds_lop() const { return classRepo.toVector(); }
    vector<Contract*> ds_hop_dong() const { return contractRepo.toVector(); }
};

#endif
