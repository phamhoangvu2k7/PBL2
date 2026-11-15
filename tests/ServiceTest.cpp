#include "test_framework.h"
#include "services/CenterService.h"
#include "services/AuthService.h"
#include "services/ProfileSync.h"
#include "services/DemoData.h"
#include "services/AdminService.h"
#include "helper/IdGenerator.h"
#include "helper/PasswordHasher.h"
#include <cstdio>
#include <cstdlib>

namespace {

// repository + service chay trong RAM (khong co file)
struct Env {
    StudentRepository students;
    TutorRepository tutors;
    ClassRepository classes;
    ContractRepository contracts;
    CenterService svc;
    Env() : svc(students, tutors, classes, contracts) {
        IdGenerator::reset();
        svc.them_gia_su(new Tutor("GS001", "Pham Hoang Vu", "0934567890", "vu@gmail.com", "", "Nam", "",
                               "Toan", "DHBK", 3, 0, ""));
        svc.them_hoc_vien(new Student("SV001", "Nguyen Van An", "12", "0901234567", "an@gmail.com", "",
                                   "Nam", "", "Toan", "", 100000));
        svc.them_hoc_vien(new Student("SV002", "Tran Thi Binh", "11", "0909876543", "binh@gmail.com", "",
                                   "Nu", "", "Toan", "", 100000));
    }
    string open(const string& schedule, string* err = nullptr) {
        string id, e;
        bool ok = svc.mo_lop("GS001", "Toan", schedule, 100000, "", "", &id, &e);
        if (err) *err = e;
        return ok ? id : "";
    }
};

string tempPath(const string& name) {
    const char* t = getenv("TEMP");
    string p = string(t ? t : ".") + "/pbl2_svc_" + name + ".txt";
    remove(p.c_str());
    return p;
}

}  // namespace

// ---------------- CenterService ----------------

TEST(Svc_OpenBookApproveFlow) {
    Env e;
    string cls = e.open("T2,T4 (18:00-20:00)");
    CHECK_EQ(cls, string("CL001"));
    CHECK_EQ(e.svc.tim_lop(cls)->getSchedule(), string("T2,T4 (18:00-20:00)"));

    string err;
    CHECK_TRUE(e.svc.dang_ky_lop("SV001", cls, &err));
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("PENDING"));
    CHECK_FALSE(e.svc.dang_ky_lop("SV002", cls, &err));   // da co nguoi dat

    CHECK_FALSE(e.svc.duyet_lop("GS999", cls));        // sai gia su
    CHECK_TRUE(e.svc.duyet_lop("GS001", cls));
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("ACTIVE"));
    CHECK_EQ(e.svc.ds_hop_dong().size(), (size_t)1);
    CHECK_TRUE(e.svc.tim_hoc_vien("SV001")->getHasTutor());
}

TEST(Svc_RejectReopensClass) {
    Env e;
    string cls = e.open("T3 (18:00-20:00)");
    e.svc.dang_ky_lop("SV001", cls);
    CHECK_TRUE(e.svc.tu_choi_lop("GS001", cls));
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("AVAILABLE"));
    CHECK_EQ(e.svc.tim_lop(cls)->getStudentId(), string(""));
    CHECK_TRUE(e.svc.dang_ky_lop("SV002", cls));
}

TEST(Svc_TutorScheduleConflictAndBackToBack) {
    Env e;
    CHECK_EQ(e.open("T2 (18:00-20:00)"), string("CL001"));
    string err;
    CHECK_EQ(e.open("T2 (19:00-21:00)", &err), string(""));       // trung
    CHECK_FALSE(err.empty());
    CHECK_EQ(e.open("T2 (20:00-22:00)"), string("CL002"));        // noi tiep, khong trung
    CHECK_EQ(e.open("T3 (18:00-20:00)"), string("CL003"));        // khac ngay
}

TEST(Svc_StudentScheduleConflict) {
    Env e;
    string a = e.open("T2 (18:00-20:00)");
    // gia su thu 2 de co ca trung gio voi ca dau
    e.svc.them_gia_su(new Tutor("GS002", "Le Thi Mai", "0912345678", "mai@gmail.com", "", "Nu", "", "Toan",
                             "DHSP", 2, 0, ""));
    string b;
    e.svc.mo_lop("GS002", "Ly", "T2 (19:00-21:00)", 90000, "", "", &b);
    CHECK_TRUE(e.svc.dang_ky_lop("SV001", a));
    string err;
    CHECK_FALSE(e.svc.dang_ky_lop("SV001", b, &err));
    CHECK_FALSE(err.empty());
    CHECK_TRUE(e.svc.dang_ky_lop("SV002", b));                // hoc vien khac thi duoc
}

TEST(Svc_OpenClassValidation) {
    Env e;
    string err;
    CHECK_EQ(e.open("T9 (18:00-20:00)", &err), string(""));
    CHECK_EQ(e.open("T2 (20:00-18:00)", &err), string(""));
    CHECK_EQ(e.open("", &err), string(""));
    CHECK_FALSE(e.svc.mo_lop("GS999", "Toan", "T2 (18:00-20:00)", 1000));
    CHECK_FALSE(e.svc.mo_lop("GS001", "Toan", "T2 (18:00-20:00)", 0));
    CHECK_FALSE(e.svc.mo_lop("GS001", "", "T2 (18:00-20:00)", 1000));
    CHECK_FALSE(e.svc.mo_lop("GS001", "To|an", "T2 (18:00-20:00)", 1000));
    CHECK_FALSE(e.svc.mo_lop("GS001", "Toan", "T2 (18:00-20:00)", 1000, "99/99/2026", ""));
    CHECK_FALSE(e.svc.mo_lop("GS001", "Toan", "T2 (18:00-20:00)", 1000, "10/02/2027", "01/02/2027"));
    CHECK_EQ(e.svc.ds_lop().size(), (size_t)0);
}

TEST(Svc_ProfileUpdateAndValidation) {
    Env e;
    StudentProfile p;
    p.fullName = "Nguyen Van Minh"; p.phone = "0911222333"; p.email = "minh@gmail.com";
    p.gender = "Nam"; p.dateOfBirth = "01/02/2008"; p.grade = "11"; p.budget = 1000;
    string err;
    CHECK_TRUE(e.svc.sua_ho_so_hv("SV001", p, &err));
    CHECK_EQ(e.svc.tim_hoc_vien("SV001")->getFullName(), string("Nguyen Van Minh"));

    p.phone = "123";
    CHECK_FALSE(e.svc.sua_ho_so_hv("SV001", p, &err));
    CHECK_EQ(e.svc.tim_hoc_vien("SV001")->getPhone(), string("0911222333"));   // giu nguyen
    CHECK_FALSE(e.svc.sua_ho_so_hv("SV999", p, &err));

    TutorProfile t;
    t.fullName = "Le Thi Hoa"; t.phone = "0944555666"; t.email = "hoa@gmail.com";
    t.subjects = "Hoa"; t.achievements = "Giai nhi"; t.expYears = 4;
    CHECK_TRUE(e.svc.sua_ho_so_gs("GS001", t, &err));
    CHECK_EQ(e.svc.tim_gia_su("GS001")->getExpYears(), 4);
    t.expYears = -1;
    CHECK_FALSE(e.svc.sua_ho_so_gs("GS001", t, &err));
}

TEST(Svc_Persistence_SurvivesRestart) {
    string fs = tempPath("students"), ft = tempPath("tutors"), fc = tempPath("classes"), fk = tempPath("contracts");
    {
        StudentRepository s(fs); TutorRepository t(ft); ClassRepository c(fc); ContractRepository k(fk);
        CenterService svc(s, t, c, k);
        CHECK_FALSE(svc.nap_du_lieu());                    // lan dau chua co file
        IdGenerator::reset();
        tao_du_lieu_mau(svc);
        CHECK_TRUE(svc.dang_ky_lop("SV001", "CL001"));
        CHECK_TRUE(svc.duyet_lop("GS001", "CL001"));
    }
    IdGenerator::reset();
    StudentRepository s(fs); TutorRepository t(ft); ClassRepository c(fc); ContractRepository k(fk);
    CenterService svc(s, t, c, k);
    CHECK_TRUE(svc.nap_du_lieu());
    CHECK_EQ(svc.ds_lop().size(), (size_t)4);
    CHECK_EQ(svc.tim_lop("CL001")->getStatus(), string("ACTIVE"));
    CHECK_EQ(svc.ds_hop_dong().size(), (size_t)1);

    // ma moi khong trung ma cu
    string id;
    CHECK_TRUE(svc.mo_lop("GS002", "Ly", "T2 (06:00-07:00)", 1000, "", "", &id));
    CHECK_EQ(id, string("CL005"));
    remove(fs.c_str()); remove(ft.c_str()); remove(fc.c_str()); remove(fk.c_str());
}

TEST(Svc_DemoDataIsIdempotent) {
    Env e;
    IdGenerator::reset();
    StudentRepository s; TutorRepository t; ClassRepository c; ContractRepository k;
    CenterService svc(s, t, c, k);
    tao_du_lieu_mau(svc);
    CHECK_EQ(svc.ds_lop().size(), (size_t)4);
    CHECK_EQ(svc.ds_hoc_vien().size(), (size_t)2);
    CHECK_EQ(svc.ds_gia_su().size(), (size_t)2);
}

// ---------------- AuthService ----------------

namespace {
// dat lai bo dem ID truoc khi AuthService tao admin
struct ResetIds { ResetIds() { IdGenerator::reset(); } };

struct AuthEnv : ResetIds {
    AccountRepository accounts;
    AuthService auth;
    AuthEnv() : auth(accounts) {}
};
}  // namespace

TEST(Auth_DefaultAdminAndLogin) {
    AuthEnv e;
    CHECK_TRUE(e.auth.login("admin", "admin123"));
    CHECK_TRUE(e.auth.la_admin());
    e.auth.logout();
    CHECK_FALSE(e.auth.da_dang_nhap());
    CHECK_FALSE(e.auth.login("admin", "sai"));
    CHECK_FALSE(e.auth.login("khong_co", "admin123"));
}

TEST(Auth_RegisterValidation) {
    AuthEnv e;
    string err;
    CHECK_TRUE(e.auth.registerAccount("nam", "matkhau1", "Student", "Nguyen Hoang Nam", "0911222333", "nam@gmail.com", &err));
    CHECK_FALSE(e.auth.registerAccount("nam", "matkhau1", "Student", "A B", "0911222333", "a@gmail.com", &err));    // trung
    CHECK_FALSE(e.auth.registerAccount("x y", "matkhau1", "Student", "A B", "0911222333", "a@gmail.com", &err));   // co dau cach
    CHECK_FALSE(e.auth.registerAccount("x|y", "matkhau1", "Student", "A B", "0911222333", "a@gmail.com", &err));   // co '|'
    CHECK_FALSE(e.auth.registerAccount("u1", "12345", "Student", "A B", "0911222333", "a@gmail.com", &err));       // mat khau ngan
    CHECK_TRUE(e.auth.registerAccount("u2", "123456", "Student", "A B", "0911222333", "a@gmail.com", &err));      // dung 6 ky tu
    CHECK_FALSE(e.auth.registerAccount("u3", "123456", "Student", "", "0911222333", "a@gmail.com", &err));        // thieu ten
    CHECK_FALSE(e.auth.registerAccount("u4", "123456", "Student", "A B", "091", "a@gmail.com", &err));            // sdt sai
    CHECK_FALSE(e.auth.registerAccount("u5", "123456", "Student", "A B", "0911222333", "", &err));                // thieu email
    CHECK_FALSE(e.auth.registerAccount("u6", "123456", "Student", "A B", "0911222333", "a@@b.com", &err));        // email sai
    CHECK_FALSE(e.auth.registerAccount("u7", "123456", "Boss", "A B", "0911222333", "a@gmail.com", &err));        // vai tro sai
}

TEST(Auth_AssignPersonIdsAndProfileSync) {
    AuthEnv e;
    e.auth.registerAccount("hs", "matkhau1", "Student", "Nguyen Hoang Nam", "0911222333", "nam@gmail.com");
    e.auth.registerAccount("gs", "matkhau2", "Tutor", "Tran Thi Lan", "0944555666", "lan@gmail.com");

    StudentRepository s; TutorRepository t; ClassRepository c; ContractRepository k;
    CenterService svc(s, t, c, k);
    tao_du_lieu_mau(svc);   // SV001, SV002, GS001, GS002

    int failed = -1;
    int created = dong_bo_ho_so(e.auth, svc, &failed);
    CHECK_EQ(created, 2);
    CHECK_EQ(failed, 0);

    // ma moi khong trung du lieu mau
    e.auth.login("hs", "matkhau1");
    CHECK_EQ(e.auth.ma_nguoi_hien_tai(), string("SV003"));
    CHECK_EQ(svc.tim_hoc_vien("SV003")->getFullName(), string("Nguyen Hoang Nam"));
    e.auth.login("gs", "matkhau2");
    CHECK_EQ(e.auth.ma_nguoi_hien_tai(), string("GS003"));

    // goi lai khong tao them
    CHECK_EQ(dong_bo_ho_so(e.auth, svc), 0);
}

TEST(Auth_PersistsAccountsAcrossRestart) {
    string f = tempPath("accounts");
    IdGenerator::reset();
    {
        AccountRepository accounts(f);
        AuthService auth(accounts);
        auth.registerAccount("hs", "matkhau1", "Student", "Nguyen Hoang Nam", "0911222333", "nam@gmail.com");
        auth.cap_ma_con_thieu();
    }
    IdGenerator::reset();
    AccountRepository accounts(f);
    accounts.load();
    AuthService auth(accounts);
    CHECK_TRUE(auth.login("hs", "matkhau1"));
    CHECK_EQ(auth.ma_nguoi_hien_tai(), string("SV001"));
    CHECK_EQ(auth.ten_hien_tai(), string("Nguyen Hoang Nam"));
    CHECK_TRUE(auth.login("admin", "admin123"));
    remove(f.c_str());
}

// ---------------- Xoa lop / rut dang ky ----------------

TEST(Svc_TutorDeleteClass) {
    Env e;
    string a = e.open("T2 (18:00-20:00)");
    string b = e.open("T3 (18:00-20:00)");
    string err;
    CHECK_FALSE(e.svc.xoa_lop_trong("GS999", a, &err));     // khong phai lop cua minh
    CHECK_FALSE(e.svc.xoa_lop_trong("GS001", "CL999", &err));
    CHECK_TRUE(e.svc.xoa_lop_trong("GS001", a, &err));
    CHECK_TRUE(e.svc.tim_lop(a) == nullptr);
    CHECK_EQ(e.svc.ds_lop().size(), (size_t)1);

    // lop da co hoc vien dang ky thi khong xoa duoc
    e.svc.dang_ky_lop("SV001", b);
    CHECK_FALSE(e.svc.xoa_lop_trong("GS001", b, &err));
    CHECK_FALSE(err.empty());
    e.svc.duyet_lop("GS001", b);
    CHECK_FALSE(e.svc.xoa_lop_trong("GS001", b, &err));    // ACTIVE
    CHECK_EQ(e.svc.ds_lop().size(), (size_t)1);

    // xoa xong thi lich duoc giai phong, mo lai cung gio duoc
    CHECK_FALSE(e.open("T2 (18:00-20:00)").empty());
}

TEST(Svc_StudentCancelBooking) {
    Env e;
    string a = e.open("T2 (18:00-20:00)");
    string err;
    CHECK_FALSE(e.svc.rut_dang_ky("SV001", a, &err));   // chua dang ky
    e.svc.dang_ky_lop("SV001", a);
    CHECK_FALSE(e.svc.rut_dang_ky("SV002", a, &err));   // nguoi khac
    CHECK_FALSE(e.svc.rut_dang_ky("SV001", "CL999", &err));
    CHECK_TRUE(e.svc.rut_dang_ky("SV001", a, &err));
    CHECK_EQ(e.svc.tim_lop(a)->getStatus(), string("AVAILABLE"));
    CHECK_EQ(e.svc.tim_lop(a)->getStudentId(), string(""));
    CHECK_TRUE(e.svc.dang_ky_lop("SV002", a));              // nguoi khac dang ky duoc

    e.svc.duyet_lop("GS001", a);
    CHECK_FALSE(e.svc.rut_dang_ky("SV002", a, &err));   // ACTIVE thi khong rut
}

// ---------------- Ket thuc hop dong ----------------

namespace {
// lop CL001 da duoc duyet: hop dong HD001 giua GS001 va SV001
struct ContractEnv : Env {
    string cls, ct;
    ContractEnv() {
        cls = open("T2 (18:00-20:00)");
        svc.dang_ky_lop("SV001", cls);
        svc.duyet_lop("GS001", cls);
        ct = svc.tim_lop(cls)->getContractId();
    }
    string status() { return svc.tim_hop_dong(ct)->getStatus(); }
};
}  // namespace

TEST(Svc_EndContract_RequestThenConfirm) {
    ContractEnv e;
    string err;
    CHECK_TRUE(e.svc.con_hop_dong_mo("SV001"));
    CHECK_TRUE(e.svc.yeu_cau_ket_thuc("GS001", e.ct, "Chuyen cong tac", &err));
    CHECK_EQ(e.status(), string("EndRequested"));
    CHECK_EQ(e.svc.tim_hop_dong(e.ct)->getRequestedBy(), string("GS001"));
    CHECK_TRUE(e.svc.con_hop_dong_mo("SV001"));             // chua ket thuc

    CHECK_FALSE(e.svc.dong_y_ket_thuc("GS001", e.ct, &err));   // tu dong y
    CHECK_TRUE(e.svc.dong_y_ket_thuc("SV001", e.ct, &err));
    CHECK_EQ(e.status(), string("Ended"));
    CHECK_FALSE(e.svc.tim_hop_dong(e.ct)->getEndedOn().empty());
    CHECK_EQ(e.svc.tim_lop(e.cls)->getStatus(), string("ENDED"));
    CHECK_FALSE(e.svc.tim_hoc_vien("SV001")->getHasTutor());
    CHECK_FALSE(e.svc.con_hop_dong_mo("SV001"));
    CHECK_FALSE(e.svc.con_hop_dong_mo("GS001"));

    // lop ENDED khong chiem lich: gia su mo lai cung gio duoc
    CHECK_FALSE(e.open("T2 (18:00-20:00)").empty());
}

TEST(Svc_EndContract_InvalidRequests) {
    ContractEnv e;
    string err;
    CHECK_FALSE(e.svc.yeu_cau_ket_thuc("SV001", e.ct, "   ", &err));      // ly do rong
    CHECK_FALSE(e.svc.yeu_cau_ket_thuc("SV002", e.ct, "Ly do", &err));    // nguoi ngoai
    CHECK_FALSE(e.svc.yeu_cau_ket_thuc("SV001", "HD999", "Ly do", &err));
    CHECK_FALSE(e.svc.yeu_cau_ket_thuc("", e.ct, "Ly do", &err));
    CHECK_FALSE(e.svc.dong_y_ket_thuc("SV001", e.ct, &err));             // chua co yeu cau
    CHECK_FALSE(e.svc.huy_yeu_cau_ket_thuc("SV001", e.ct, &err));
    CHECK_EQ(e.status(), string("Active"));

    CHECK_TRUE(e.svc.yeu_cau_ket_thuc("SV001", e.ct, "Ly do", &err));
    CHECK_FALSE(e.svc.yeu_cau_ket_thuc("GS001", e.ct, "Ly do", &err));    // da co yeu cau
    CHECK_FALSE(e.svc.dong_y_ket_thuc("SV002", e.ct, &err));             // nguoi ngoai
}

TEST(Svc_EndContract_CancelAndReject) {
    ContractEnv e;
    string err;
    // nguoi yeu cau rut lai
    e.svc.yeu_cau_ket_thuc("SV001", e.ct, "Ly do", &err);
    CHECK_TRUE(e.svc.huy_yeu_cau_ket_thuc("SV001", e.ct, &err));
    CHECK_EQ(e.status(), string("Active"));
    CHECK_EQ(e.svc.tim_hop_dong(e.ct)->getRequestedBy(), string(""));
    // ben con lai tu choi
    e.svc.yeu_cau_ket_thuc("SV001", e.ct, "Ly do", &err);
    CHECK_TRUE(e.svc.huy_yeu_cau_ket_thuc("GS001", e.ct, &err));
    CHECK_EQ(e.status(), string("Active"));
    CHECK_EQ(e.svc.tim_lop(e.cls)->getStatus(), string("ACTIVE"));
}

TEST(Svc_EndContract_Admin) {
    ContractEnv e;
    string err;
    CHECK_TRUE(e.svc.admin_ket_thuc(e.ct, "", &err));
    CHECK_EQ(e.status(), string("Ended"));
    CHECK_EQ(e.svc.tim_hop_dong(e.ct)->getRequestedBy(), string("ADMIN"));
    CHECK_FALSE(e.svc.admin_ket_thuc(e.ct, "", &err));    // da ket thuc
    CHECK_FALSE(e.svc.admin_ket_thuc("HD999", "", &err));
}

// ---------------- Admin xoa hoc vien / gia su ----------------

namespace {
// Env + tai khoan cua SV001 (an), GS001 (vu)
struct AdminEnv : Env {
    AccountRepository accounts;
    AuthService auth;
    AdminService admin;
    AdminEnv() : auth(accounts), admin(auth, svc) {
        accounts.add(new Account("AC100", "an", "x", "SV001", "Student"));
        accounts.add(new Account("AC101", "vu", "x", "GS001", "Tutor"));
    }
};
}  // namespace

TEST(Admin_DeleteStudent_ReleasesPendingClass) {
    AdminEnv e;
    string cls = e.open("T2 (18:00-20:00)");
    e.svc.dang_ky_lop("SV001", cls);

    DeleteImpact p = e.admin.xem_truoc_xoa_hoc_vien("SV001");
    CHECK_TRUE(p.allowed);
    CHECK_EQ(p.releasedClasses.size(), (size_t)1);
    CHECK_EQ(p.deletedAccounts.size(), (size_t)1);
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("PENDING"));   // preview khong sua

    string err;
    CHECK_TRUE(e.admin.xoa_hoc_vien("SV001", &err));
    CHECK_TRUE(e.svc.tim_hoc_vien("SV001") == nullptr);
    CHECK_TRUE(e.accounts.findByUsername("an") == nullptr);
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("AVAILABLE"));
    CHECK_EQ(e.svc.tim_lop(cls)->getStudentId(), string(""));
    CHECK_TRUE(e.accounts.findByUsername("vu") != nullptr);   // nguoi khac giu nguyen

    // dang nhap lai khong tao lai ho so rong
    CHECK_EQ(dong_bo_ho_so(e.auth, e.svc), 0);
    CHECK_TRUE(e.svc.tim_hoc_vien("SV001") == nullptr);
}

TEST(Admin_Delete_BlockedByOpenContract) {
    AdminEnv e;
    string cls = e.open("T2 (18:00-20:00)");
    e.svc.dang_ky_lop("SV001", cls);
    e.svc.duyet_lop("GS001", cls);
    string ct = e.svc.tim_lop(cls)->getContractId();

    string err;
    CHECK_FALSE(e.admin.xem_truoc_xoa_hoc_vien("SV001").allowed);
    CHECK_FALSE(e.admin.xoa_hoc_vien("SV001", &err));
    CHECK_FALSE(err.empty());
    CHECK_FALSE(e.admin.xoa_gia_su("GS001", &err));
    // khong co gi thay doi
    CHECK_TRUE(e.svc.tim_hoc_vien("SV001") != nullptr);
    CHECK_TRUE(e.accounts.findByUsername("an") != nullptr);
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("ACTIVE"));

    // yeu cau ket thuc chua duoc dong y van chan
    e.svc.yeu_cau_ket_thuc("SV001", ct, "Ly do");
    CHECK_FALSE(e.admin.xoa_hoc_vien("SV001", &err));

    // ket thuc xong thi xoa duoc, lich su giu lai
    e.svc.dong_y_ket_thuc("GS001", ct);
    DeleteImpact p = e.admin.xem_truoc_xoa_hoc_vien("SV001");
    CHECK_TRUE(p.allowed);
    CHECK_EQ(p.keptHistory, 1);
    CHECK_TRUE(e.admin.xoa_hoc_vien("SV001", &err));
    CHECK_TRUE(e.svc.tim_hop_dong(ct) != nullptr);
    CHECK_EQ(e.svc.tim_lop(cls)->getStatus(), string("ENDED"));
}

TEST(Admin_DeleteTutor_RemovesOpenClassesKeepsHistory) {
    AdminEnv e;
    string ended = e.open("T2 (18:00-20:00)");
    e.svc.dang_ky_lop("SV001", ended);
    e.svc.duyet_lop("GS001", ended);
    e.svc.admin_ket_thuc(e.svc.tim_lop(ended)->getContractId(), "");
    string avail = e.open("T3 (18:00-20:00)");
    string pending = e.open("T4 (18:00-20:00)");
    e.svc.dang_ky_lop("SV002", pending);

    DeleteImpact p = e.admin.xem_truoc_xoa_gia_su("GS001");
    CHECK_TRUE(p.allowed);
    CHECK_EQ(p.deletedClasses.size(), (size_t)2);

    string err;
    CHECK_TRUE(e.admin.xoa_gia_su("GS001", &err));
    CHECK_TRUE(e.svc.tim_gia_su("GS001") == nullptr);
    CHECK_TRUE(e.accounts.findByUsername("vu") == nullptr);
    CHECK_TRUE(e.svc.tim_lop(avail) == nullptr);
    CHECK_TRUE(e.svc.tim_lop(pending) == nullptr);
    CHECK_TRUE(e.svc.tim_lop(ended) != nullptr);
}

TEST(Admin_Delete_InvalidCases) {
    AdminEnv e;
    string err;
    CHECK_FALSE(e.admin.xoa_hoc_vien("SV999", &err));
    CHECK_FALSE(e.admin.xoa_hoc_vien("", &err));
    CHECK_FALSE(e.admin.xoa_gia_su("SV001", &err));            // ma hoc vien, khong phai gia su
    CHECK_FALSE(e.auth.co_the_xoa_tai_khoan("ADMIN"));

    // khong xoa nguoi dang dang nhap
    e.accounts.findByUsername("an")->setPasswordHash(PasswordHasher::hash("matkhau1"));
    CHECK_TRUE(e.auth.login("an", "matkhau1"));
    CHECK_FALSE(e.admin.xem_truoc_xoa_hoc_vien("SV001").allowed);
    CHECK_FALSE(e.admin.xoa_hoc_vien("SV001", &err));
    CHECK_TRUE(e.svc.tim_hoc_vien("SV001") != nullptr);
    CHECK_EQ(e.svc.ds_hoc_vien().size(), (size_t)2);
}
