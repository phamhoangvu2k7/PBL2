#include <iostream>
#include "../include/entities/Student.h"
#include "../include/entities/Tutor.h"
#include "../include/services/CenterService.h"

using namespace std;

int main() {
    cout << "==============================================================\n";
    cout << "       HE THONG QUAN LY TRUNG TAM GIA SU (PBL2) - DEMO        \n";
    cout << "==============================================================\n";

    CenterService service;

    // 1. THEM HOC VIEN VA GIA SU
    cout << "\n>>> [1] THEM HOC VIEN VA GIA SU VAO HE THONG:\n";
    Student* s1 = new Student("SV001", "Nguyen Van An", "12", "0901234567",
                              "an@gmail.com", "123 Le Loi, Da Nang", "Nam",
                              "15/03/2007", "Toan,Ly", "T2,T4,T6", 150000);
    Student* s2 = new Student("SV002", "Tran Thi Binh", "11", "0909876543",
                              "binh@gmail.com", "456 Tran Phu, Da Nang", "Nu",
                              "20/08/2008", "Toan,Hoa", "T3,T5,T7", 120000);
    service.addStudent(s1);
    service.addStudent(s2);

    Tutor* t1 = new Tutor("GS001", "Pham Hoang Vu", "0934567890", "vu@gmail.com",
                          "789 Nguyen Luong Bang", "Nam", "25/12/1998",
                          "Toan,Ly", "Dai hoc Bach Khoa", 3, 150000, "T2,T4,T6");
    Tutor* t2 = new Tutor("GS002", "Le Thi Mai", "0912345678", "mai@gmail.com",
                          "102 Dien Bien Phu", "Nu", "10/05/1999",
                          "Toan,Hoa", "Dai hoc Su Pham", 2, 130000, "T3,T5,T7");
    service.addTutor(t1);
    service.addTutor(t2);

    service.displayAllStudents();
    service.displayAllTutors();

    // 2. GIA SU DUA RA CAC KHUNG GIO HOC THEO TUNG MON (MO CA HOC)
    cout << "\n>>> [2] GIA SU DANG KY MO CAC KHUNG GIO DAY (CA HOC):\n";
    service.tutorOpenClass("GS001", "CL001", "Toan 12", "T2-T4 (18h-20h)", 150000, "01/11/2026", "31/01/2027");
    service.tutorOpenClass("GS001", "CL002", "Toan 12", "T3-T5 (19h-21h)", 150000, "01/11/2026", "31/01/2027");
    service.tutorOpenClass("GS002", "CL003", "Toan 11", "T7-CN (8h-10h)", 130000, "05/11/2026", "05/02/2027");
    service.tutorOpenClass("GS001", "CL004", "Vat Ly 12", "T6-CN (18h-20h)", 140000, "01/11/2026", "31/01/2027");

    // 3. HOC VIEN TIM KIEM KHUNG GIO HOC THEO MON
    cout << "\n>>> [3] HOC VIEN SV001 TIM KIEM MON \"Toan\":\n";
    service.displayAvailableClassesBySubject("Toan");

    // 4. HOC VIEN CHON KHUNG GIO -> KHUNG GIO BI AN KHOI NGUOI KHAC
    cout << "\n>>> [4] HOC VIEN SV001 DANG KY CA HOC CL001 (Toan 12 - T2-T4):\n";
    service.studentBookClass("SV001", "CL001");

    cout << "\n>>> [4.1] HOC VIEN SV002 TIM KIEM LAI MON \"Toan\":\n";
    cout << "   (Lop CL001 da duoc SV001 chon nen se KHONG con hien thi cho SV002!)\n";
    service.displayAvailableClassesBySubject("Toan");

    // 5. GIA SU XEM DANH SACH CHO DUYET
    cout << "\n>>> [5] GIA SU GS001 KIEM TRA CAC YEU CAU DANG CHO DUYET:\n";
    service.displayPendingRequestsByTutor("GS001");

    // 6. GIA SU PHÊ DUYET -> TAO LOP ACTIVE VA SINH HOP DONG
    cout << "\n>>> [6] GIA SU GS001 XAC NHAN (APPROVE) LOP CL001:\n";
    service.tutorApproveClass("GS001", "CL001");

    // 7. THU NGHIEM TU CHOI LOP (REJECT -> MO LAI KHUNG GIO)
    cout << "\n>>> [7] THU NGHIEM DANG KY & TU CHOI (CL002):\n";
    service.studentBookClass("SV002", "CL002");
    cout << "   Gia su GS001 tu choi lop CL002:\n";
    service.tutorRejectClass("GS001", "CL002");
    cout << "   Kiem tra lai: Ca CL002 (mon Toan) da duoc mo lai cho moi nguoi:\n";
    service.displayAvailableClassesBySubject("Toan");

    // 8. XEM DANH SACH LOP CUA MINH (MY CLASSES)
    cout << "\n>>> [8] XEM DANH SACH LOP CUA TUNG NGUOI:\n";
    service.displayClassesByStudent("SV001");
    service.displayClassesByStudent("SV002");
    service.displayClassesByTutor("GS001");

    // 9. XEM DANH SACH HOP DONG HE THONG
    cout << "\n>>> [9] DANH SACH HOP DONG DA DUOC KICH HOAT:\n";
    service.displayAllContracts();

    cout << "\n==============================================================\n";
    cout << "              CHUONG TRINH DEMO HOAN TAT MY MAN!              \n";
    cout << "==============================================================\n";
    return 0;
}
