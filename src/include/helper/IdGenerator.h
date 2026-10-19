#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include <string>
using namespace std;

class IdGenerator {
private:
    static int nextStudentId;
    static int nextTutorId;
    static int nextClassId;
    static int nextContractId;
    static int nextAccountId;

    // Phụ trợ: đệm số 0 bên trái, VD: padLeft(5, 3) -> "005"
    static string padLeft(int number, int width);

public:
    static string generateStudentId();    // "SV001", "SV002", ...
    static string generateTutorId();      // "GS001", "GS002", ...
    static string generateClassId();      // "CL001", "CL002", ...
    static string generateContractId();   // "HD001", "HD002", ...
    static string generateAccountId();    // "AC001", "AC002", ...

    // Khi load dữ liệu từ file, cần cập nhật lại bộ đếm
    // để đảm bảo mã mới không trùng với mã đã tồn tại
    static void syncFromExistingId(const string& existingId);

    // Getters cho debug / display
    static int getNextStudentNum() { return nextStudentId; }
    static int getNextTutorNum() { return nextTutorId; }
    static int getNextClassNum() { return nextClassId; }
    static int getNextContractNum() { return nextContractId; }
};

#endif
