#include "test_framework.h"
#include "repositories/StudentRepository.h"
#include "repositories/TutorRepository.h"
#include "repositories/ClassRepository.h"
#include "repositories/ContractRepository.h"
#include "repositories/AccountRepository.h"
#include "helper/IdGenerator.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>

namespace {

// file tam trong thu muc TEMP, xoa khi het test
struct TempFile {
    string path;
    explicit TempFile(const string& name) {
        const char* t = getenv("TEMP");
        path = string(t ? t : ".") + "/pbl2_repo_" + name + ".txt";
        remove(path.c_str());
    }
    ~TempFile() { remove(path.c_str()); }
};

Student* mkStudent(const string& id) {
    return new Student(id, "Nguyen Van An", "12", "0901234567", "an@gmail.com", "addr", "Nam",
                       "15/03/2007", "Toan", "T2", 100000);
}

}  // namespace

TEST(Repo_Student_AddFindAndDuplicate) {
    StudentRepository repo;
    Student* a = mkStudent("SV001");
    CHECK_TRUE(repo.add(a));
    CHECK_TRUE(repo.findById("SV001") == a);
    CHECK_EQ(repo.size(), 1);

    Student* dup = mkStudent("SV001");
    CHECK_FALSE(repo.add(dup));   // trung ID, repo khong nhan con tro
    delete dup;

    Student* noId = mkStudent("");
    CHECK_FALSE(repo.add(noId));
    delete noId;
    CHECK_FALSE(repo.add(nullptr));
    CHECK_TRUE(repo.findById("SV999") == nullptr);
    CHECK_EQ(repo.size(), 1);
}

TEST(Repo_ToVectorKeepsInsertOrder) {
    StudentRepository repo;
    repo.add(mkStudent("SV003"));
    repo.add(mkStudent("SV001"));
    repo.add(mkStudent("SV002"));
    vector<Student*> v = repo.toVector();
    CHECK_EQ(v.size(), (size_t)3);
    CHECK_EQ(v[0]->getId(), string("SV003"));
    CHECK_EQ(v[2]->getId(), string("SV002"));
}

TEST(Repo_NoFile_LoadReturnsFalseAndSaveDoesNothing) {
    StudentRepository repo;   // khong co file
    CHECK_FALSE(repo.load());
    CHECK_FALSE(repo.save());

    TempFile f("missing");
    StudentRepository repo2(f.path);
    CHECK_FALSE(repo2.load());   // file chua ton tai
}

TEST(Repo_Student_SaveThenLoadRoundTrip) {
    TempFile f("student");
    {
        StudentRepository repo(f.path);
        Student* s = new Student("SV001", "Nguyen Van An", "12", "0901234567", "an@gmail.com",
                                 "12 Le Loi | Q1\\ghi chu", "Nam", "15/03/2007", "Toan,Ly", "T2,T4",
                                 120000.0, true, "CL001");
        CHECK_TRUE(repo.add(s));
        s->setFullName("Nguyen Van Minh");
        CHECK_TRUE(repo.update(s));
    }
    StudentRepository again(f.path);
    CHECK_TRUE(again.load());
    Student* s = again.findById("SV001");
    CHECK_TRUE(s != nullptr);
    if (s) {
        CHECK_EQ(s->getFullName(), string("Nguyen Van Minh"));
        CHECK_EQ(s->getAddress(), string("12 Le Loi | Q1\\ghi chu"));
        CHECK_EQ(s->getBudget(), 120000.0);
        CHECK_TRUE(s->getHasTutor());
        CHECK_EQ(s->getRegisteredClasses(), string("CL001"));
    }
}

TEST(Repo_Tutor_LoadsOldFileWithoutAchievements) {
    TempFile f("tutor");
    {
        ofstream out(f.path);
        // file cu: 14 cot, chua co thanh tich
        out << "GS001|Pham Hoang Vu|0934567890|vu@gmail.com|addr|Nam|25/12/1998|Toan|DHBK|3|0|T2|1|\n";
    }
    TutorRepository repo(f.path);
    CHECK_TRUE(repo.load());
    Tutor* t = repo.findById("GS001");
    CHECK_TRUE(t != nullptr);
    if (t) {
        CHECK_EQ(t->getExpYears(), 3);
        CHECK_EQ(t->getAchievements(), string(""));
    }
}

TEST(Repo_Tutor_AchievementsRoundTrip) {
    TempFile f("tutor2");
    {
        TutorRepository repo(f.path);
        repo.add(new Tutor("GS001", "Le Thi Hoa", "0944555666", "hoa@gmail.com", "", "Nu", "", "Hoa",
                           "DHSP", 4, 0, "", true, "", "Giai nhi HSG\nIELTS 8.0"));
    }
    TutorRepository again(f.path);
    again.load();
    CHECK_EQ(again.findById("GS001")->getAchievements(), string("Giai nhi HSG\nIELTS 8.0"));
}

TEST(Repo_Load_SkipsBrokenLinesAndDuplicates) {
    TempFile f("class");
    {
        ofstream out(f.path);
        out << "rac\n";
        out << "\n";
        out << "CL001|||GS001|Toan|T2 (18:00-20:00)|||AVAILABLE|abc\n";   // hoc phi hong -> 0
        out << "CL001|||GS001|Ly|T3 (18:00-20:00)|||AVAILABLE|1000\n";    // trung ID
    }
    ClassRepository repo(f.path);
    CHECK_TRUE(repo.load());
    CHECK_EQ(repo.size(), 1);
    CHECK_EQ(repo.findById("CL001")->getSubject(), string("Toan"));
    CHECK_EQ(repo.findById("CL001")->getTuitionFee(), 0.0);
}

TEST(Repo_Load_SyncsIdGenerator) {
    IdGenerator::reset();
    TempFile f("class2");
    {
        ofstream out(f.path);
        out << "CL010|||GS001|Toan|T2 (18:00-20:00)|||AVAILABLE|1000\n";
    }
    ClassRepository repo(f.path);
    repo.load();
    CHECK_EQ(IdGenerator::generateClassId(), string("CL011"));
}

TEST(Repo_Contract_RoundTrip) {
    TempFile f("contract");
    {
        ContractRepository repo(f.path);
        repo.add(new Contract("HD001", "GS001", "SV001", "01/11/2026", "31/01/2027", "Toan", "Active"));
    }
    ContractRepository again(f.path);
    CHECK_TRUE(again.load());
    CHECK_EQ(again.findById("HD001")->getTutorId(), string("GS001"));
    CHECK_EQ(again.findById("HD001")->getStatus(), string("Active"));
}

TEST(Repo_Account_FindByUsername) {
    AccountRepository repo;
    CHECK_TRUE(repo.add(new Account("AC001", "admin", "123", "ADMIN", "Admin")));
    CHECK_TRUE(repo.add(new Account("AC002", "lan", "456", "", "Tutor")));
    CHECK_EQ(repo.findByUsername("lan")->getAccountId(), string("AC002"));
    CHECK_TRUE(repo.findByUsername("nobody") == nullptr);

    Account* dup = new Account("AC003", "admin", "789", "", "Student");   // trung username
    CHECK_FALSE(repo.add(dup));
    delete dup;
    CHECK_EQ(repo.size(), 2);
}

TEST(Repo_Account_LoadsOldFormatCRLFAndSkipsBadRole) {
    TempFile f("account");
    {
        ofstream out(f.path, ios::binary);
        out << "AC001|admin|407908580|ADMIN|Admin\r\n";                  // 5 cot + CRLF
        out << "AC002|student|2110877786|SV001|Student|Nguyen Van An|0901234567|an@gmail.com\r\n";
        out << "AC003|hacker|111|X|SuperUser\r\n";                       // vai tro la
        out << "AC004||111|X|Student\r\n";                               // thieu username
    }
    AccountRepository repo(f.path);
    CHECK_TRUE(repo.load());
    CHECK_EQ(repo.size(), 2);
    CHECK_EQ(repo.findByUsername("admin")->getRole(), string("Admin"));   // khong dinh \r
    CHECK_EQ(repo.findByUsername("student")->getEmail(), string("an@gmail.com"));
    CHECK_TRUE(repo.findByUsername("hacker") == nullptr);
}

TEST(Repo_Account_SaveThenLoadKeepsProfile) {
    TempFile f("account2");
    {
        AccountRepository repo(f.path);
        Account* a = new Account("AC001", "lan", "999", "", "Tutor", "Tran Thi Lan", "0944555666", "lan@gmail.com");
        repo.add(a);
        a->setPersonId("GS003");
        repo.update(a);
    }
    AccountRepository again(f.path);
    again.load();
    Account* a = again.findByUsername("lan");
    CHECK_TRUE(a != nullptr);
    if (a) {
        CHECK_EQ(a->getPersonId(), string("GS003"));
        CHECK_EQ(a->getFullName(), string("Tran Thi Lan"));
    }
}
