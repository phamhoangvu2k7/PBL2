#include <iostream>

#include "../include/entities/Class.h"
#include "../include/entities/Student.h"
#include "../include/entities/Tutor.h"

using namespace std;

int main() {
  Student s("SV001", "Nguyen Van A", "12", "0901234567", "a@gmail.com",
            "123 Le Loi", "Nam", "15/03/2007", "Toan,Ly", "T2,T4,T6", 60000);
  s.display();

  Tutor t("GS001", "Pham Thi M", "0934567890", "m@gmail.com", "321 Vo Van Tan",
          "Nu", "25/12/1998", "Toan,Ly,Hoa", "DH Su Pham", 3, 60000,
          "T2,T4,T6");
  t.display();

  Class c("CL001", "CTC001", s.getId(), t.getId(), "Toan", "T2,T4,T6",
          "15/01/2025", "15/04/2025", "Active", 60000);
  c.display();

  cout << "\nKhoi tao va ke thua OOP thanh cong!" << endl;
  return 0;
}
