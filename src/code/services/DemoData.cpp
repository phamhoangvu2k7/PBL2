#include "../../include/services/DemoData.h"

namespace {
// addStudent/addTutor khong giai phong con tro khi loi nen tu xoa
void addStudentOrFree(CenterService& sv, Student* s) { if (!sv.addStudent(s)) delete s; }
void addTutorOrFree(CenterService& sv, Tutor* t)     { if (!sv.addTutor(t)) delete t; }
}

void seedDemoData(CenterService& service) {
    addStudentOrFree(service, new Student("SV001", "Nguyen Van An", "12", "0901234567",
                                          "an@gmail.com", "123 Le Loi, Da Nang", "Nam",
                                          "15/03/2007", "Toan,Ly", "T2,T4,T6", 150000));
    addStudentOrFree(service, new Student("SV002", "Tran Thi Binh", "11", "0909876543",
                                          "binh@gmail.com", "456 Tran Phu, Da Nang", "Nu",
                                          "20/08/2008", "Toan,Hoa", "T3,T5,T7", 120000));

    addTutorOrFree(service, new Tutor("GS001", "Pham Hoang Vu", "0934567890", "vu@gmail.com",
                                      "789 Nguyen Luong Bang", "Nam", "25/12/1998",
                                      "Toan,Ly", "Dai hoc Bach Khoa", 3, 150000, "T2,T4,T6"));
    addTutorOrFree(service, new Tutor("GS002", "Le Thi Mai", "0912345678", "mai@gmail.com",
                                      "102 Dien Bien Phu", "Nu", "10/05/1999",
                                      "Toan,Hoa", "Dai hoc Su Pham", 2, 130000, "T3,T5,T7"));

    // lich nhieu khung: "T2,T4 (18:00-20:00); T6 (...)"
    service.tutorOpenClass("GS001", "Toan 12", "T2,T4 (18:00-20:00)", 150000, "01/11/2026", "31/01/2027");
    service.tutorOpenClass("GS001", "Toan 12", "T3,T5 (19:00-21:00)", 150000, "01/11/2026", "31/01/2027");
    service.tutorOpenClass("GS002", "Toan 11", "T7,CN (08:00-10:00)", 130000, "05/11/2026", "05/02/2027");
    service.tutorOpenClass("GS001", "Vat Ly 12", "T6 (18:00-20:00); CN (08:00-10:00)", 140000,
                           "01/11/2026", "31/01/2027");
}
