#include "test_framework.h"
#include "services/CenterService.h"
#include "services/AuthService.h"
#include "services/ProfileSync.h"
#include "services/DemoData.h"
#include "helper/IdGenerator.h"
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
        svc.addTutor(new Tutor("GS001", "Pham Hoang Vu", "0934567890", "vu@gmail.com", "", "Nam", "",
                               "Toan", "DHBK", 3, 0, ""));
        svc.addStudent(new Student("SV001", "Nguyen Van An", "12", "0901234567", "an@gmail.com", "",
                                   "Nam", "", "Toan", "", 100000));
        svc.addStudent(new Student("SV002", "Tran Thi Binh", "11", "0909876543", "binh@gmail.com", "",
                                   "Nu", "", "Toan", "", 100000));
    }
    string open(const string& schedule, string* err = nullptr) {
        string id, e;
        bool ok = svc.tutorOpenClass("GS001", "Toan", schedule, 100000, "", "", &id, &e);
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
    CHECK_EQ(e.svc.findClassById(cls)->getSchedule(), string("T2,T4 (18:00-20:00)"));

    string err;
    CHECK_TRUE(e.svc.studentBookClass("SV001", cls, &err));
    CHECK_EQ(e.svc.findClassById(cls)->getStatus(), string("PENDING"));
    CHECK_FALSE(e.svc.studentBookClass("SV002", cls, &err));   // da co nguoi dat

    CHECK_FALSE(e.svc.tutorApproveClass("GS999", cls));        // sai gia su
    CHECK_TRUE(e.svc.tutorApproveClass("GS001", cls));
    CHECK_EQ(e.svc.findClassById(cls)->getStatus(), string("ACTIVE"));
    CHECK_EQ(e.svc.getContracts().size(), (size_t)1);
    CHECK_TRUE(e.svc.findStudentById("SV001")->getHasTutor());
}

TEST(Svc_RejectReopensClass) {
    Env e;
    string cls = e.open("T3 (18:00-20:00)");
    e.svc.studentBookClass("SV001", cls);
    CHECK_TRUE(e.svc.tutorRejectClass("GS001", cls));
    CHECK_EQ(e.svc.findClassById(cls)->getStatus(), string("AVAILABLE"));
    CHECK_EQ(e.svc.findClassById(cls)->getStudentId(), string(""));
    CHECK_TRUE(e.svc.studentBookClass("SV002", cls));
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
    e.svc.addTutor(new Tutor("GS002", "Le Thi Mai", "0912345678", "mai@gmail.com", "", "Nu", "", "Toan",
                             "DHSP", 2, 0, ""));
    string b;
    e.svc.tutorOpenClass("GS002", "Ly", "T2 (19:00-21:00)", 90000, "", "", &b);
    CHECK_TRUE(e.svc.studentBookClass("SV001", a));
    string err;
    CHECK_FALSE(e.svc.studentBookClass("SV001", b, &err));
    CHECK_FALSE(err.empty());
    CHECK_TRUE(e.svc.studentBookClass("SV002", b));                // hoc vien khac thi duoc
}

TEST(Svc_OpenClassValidation) {
    Env e;
    string err;
    CHECK_EQ(e.open("T9 (18:00-20:00)", &err), string(""));
    CHECK_EQ(e.open("T2 (20:00-18:00)", &err), string(""));
    CHECK_EQ(e.open("", &err), string(""));
    CHECK_FALSE(e.svc.tutorOpenClass("GS999", "Toan", "T2 (18:00-20:00)", 1000));
    CHECK_FALSE(e.svc.tutorOpenClass("GS001", "Toan", "T2 (18:00-20:00)", 0));
    CHECK_FALSE(e.svc.tutorOpenClass("GS001", "", "T2 (18:00-20:00)", 1000));
    CHECK_FALSE(e.svc.tutorOpenClass("GS001", "To|an", "T2 (18:00-20:00)", 1000));
    CHECK_FALSE(e.svc.tutorOpenClass("GS001", "Toan", "T2 (18:00-20:00)", 1000, "99/99/2026", ""));
    CHECK_FALSE(e.svc.tutorOpenClass("GS001", "Toan", "T2 (18:00-20:00)", 1000, "10/02/2027", "01/02/2027"));
    CHECK_EQ(e.svc.getClasses().size(), (size_t)0);
}

TEST(Svc_ProfileUpdateAndValidation) {
    Env e;
    StudentProfile p;
    p.fullName = "Nguyen Van Minh"; p.phone = "0911222333"; p.email = "minh@gmail.com";
    p.gender = "Nam"; p.dateOfBirth = "01/02/2008"; p.grade = "11"; p.budget = 1000;
    string err;
    CHECK_TRUE(e.svc.updateStudentProfile("SV001", p, &err));
    CHECK_EQ(e.svc.findStudentById("SV001")->getFullName(), string("Nguyen Van Minh"));

    p.phone = "123";
    CHECK_FALSE(e.svc.updateStudentProfile("SV001", p, &err));
    CHECK_EQ(e.svc.findStudentById("SV001")->getPhone(), string("0911222333"));   // giu nguyen
    CHECK_FALSE(e.svc.updateStudentProfile("SV999", p, &err));

    TutorProfile t;
    t.fullName = "Le Thi Hoa"; t.phone = "0944555666"; t.email = "hoa@gmail.com";
    t.subjects = "Hoa"; t.achievements = "Giai nhi"; t.expYears = 4;
    CHECK_TRUE(e.svc.updateTutorProfile("GS001", t, &err));
    CHECK_EQ(e.svc.findTutorById("GS001")->getExpYears(), 4);
    t.expYears = -1;
    CHECK_FALSE(e.svc.updateTutorProfile("GS001", t, &err));
}

TEST(Svc_Persistence_SurvivesRestart) {
    string fs = tempPath("students"), ft = tempPath("tutors"), fc = tempPath("classes"), fk = tempPath("contracts");
    {
        StudentRepository s(fs); TutorRepository t(ft); ClassRepository c(fc); ContractRepository k(fk);
        CenterService svc(s, t, c, k);
        CHECK_FALSE(svc.loadData());                    // lan dau chua co file
        IdGenerator::reset();
        seedDemoData(svc);
        CHECK_TRUE(svc.studentBookClass("SV001", "CL001"));
        CHECK_TRUE(svc.tutorApproveClass("GS001", "CL001"));
    }
    IdGenerator::reset();
    StudentRepository s(fs); TutorRepository t(ft); ClassRepository c(fc); ContractRepository k(fk);
    CenterService svc(s, t, c, k);
    CHECK_TRUE(svc.loadData());
    CHECK_EQ(svc.getClasses().size(), (size_t)4);
    CHECK_EQ(svc.findClassById("CL001")->getStatus(), string("ACTIVE"));
    CHECK_EQ(svc.getContracts().size(), (size_t)1);

    // ma moi khong trung ma cu
    string id;
    CHECK_TRUE(svc.tutorOpenClass("GS002", "Ly", "T2 (06:00-07:00)", 1000, "", "", &id));
    CHECK_EQ(id, string("CL005"));
    remove(fs.c_str()); remove(ft.c_str()); remove(fc.c_str()); remove(fk.c_str());
}

TEST(Svc_DemoDataIsIdempotent) {
    Env e;
    IdGenerator::reset();
    StudentRepository s; TutorRepository t; ClassRepository c; ContractRepository k;
    CenterService svc(s, t, c, k);
    seedDemoData(svc);
    CHECK_EQ(svc.getClasses().size(), (size_t)4);
    CHECK_EQ(svc.getStudents().size(), (size_t)2);
    CHECK_EQ(svc.getTutors().size(), (size_t)2);
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
    CHECK_TRUE(e.auth.isAdmin());
    e.auth.logout();
    CHECK_FALSE(e.auth.isLoggedIn());
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
    seedDemoData(svc);   // SV001, SV002, GS001, GS002

    int failed = -1;
    int created = syncProfilesFromAccounts(e.auth, svc, &failed);
    CHECK_EQ(created, 2);
    CHECK_EQ(failed, 0);

    // ma moi khong trung du lieu mau
    e.auth.login("hs", "matkhau1");
    CHECK_EQ(e.auth.getCurrentPersonId(), string("SV003"));
    CHECK_EQ(svc.findStudentById("SV003")->getFullName(), string("Nguyen Hoang Nam"));
    e.auth.login("gs", "matkhau2");
    CHECK_EQ(e.auth.getCurrentPersonId(), string("GS003"));

    // goi lai khong tao them
    CHECK_EQ(syncProfilesFromAccounts(e.auth, svc), 0);
}

TEST(Auth_PersistsAccountsAcrossRestart) {
    string f = tempPath("accounts");
    IdGenerator::reset();
    {
        AccountRepository accounts(f);
        AuthService auth(accounts);
        auth.registerAccount("hs", "matkhau1", "Student", "Nguyen Hoang Nam", "0911222333", "nam@gmail.com");
        auth.assignMissingPersonIds();
    }
    IdGenerator::reset();
    AccountRepository accounts(f);
    accounts.load();
    AuthService auth(accounts);
    CHECK_TRUE(auth.login("hs", "matkhau1"));
    CHECK_EQ(auth.getCurrentPersonId(), string("SV001"));
    CHECK_EQ(auth.getCurrentDisplayName(), string("Nguyen Hoang Nam"));
    CHECK_TRUE(auth.login("admin", "admin123"));
    remove(f.c_str());
}
