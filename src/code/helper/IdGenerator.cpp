#include "../../include/helper/IdGenerator.h"
#include <sstream>
#include <iomanip>

// Khởi tạo biến static - bắt đầu từ 1
int IdGenerator::nextStudentId  = 1;
int IdGenerator::nextTutorId    = 1;
int IdGenerator::nextClassId    = 1;
int IdGenerator::nextContractId = 1;
int IdGenerator::nextAccountId  = 1;

// Phụ trợ: đệm số 0 bên trái
string IdGenerator::padLeft(int number, int width) {
    ostringstream oss;
    oss << setfill('0') << setw(width) << number;
    return oss.str();
}

string IdGenerator::generateStudentId() {
    return "SV" + padLeft(nextStudentId++, 3);
}

string IdGenerator::generateTutorId() {
    return "GS" + padLeft(nextTutorId++, 3);
}

string IdGenerator::generateClassId() {
    return "CL" + padLeft(nextClassId++, 3);
}

string IdGenerator::generateContractId() {
    return "HD" + padLeft(nextContractId++, 3);
}

string IdGenerator::generateAccountId() {
    return "AC" + padLeft(nextAccountId++, 3);
}

// Khi load dữ liệu từ file, đồng bộ bộ đếm để tránh sinh mã trùng
// Hỗ trợ tiền tố: SV, GS, CL, HD, AC
void IdGenerator::syncFromExistingId(const string& existingId) {
    if (existingId.length() < 3) return;

    string prefix = existingId.substr(0, 2);
    string numStr = existingId.substr(2);

    // Kiểm tra phần số
    for (char c : numStr) {
        if (c < '0' || c > '9') return;
    }

    int num = stoi(numStr) + 1; // Bộ đếm kế tiếp phải lớn hơn mã đã có

    if (prefix == "SV" && num > nextStudentId)  { nextStudentId  = num; }
    else if (prefix == "GS" && num > nextTutorId)    { nextTutorId    = num; }
    else if (prefix == "CL" && num > nextClassId)    { nextClassId    = num; }
    else if (prefix == "HD" && num > nextContractId) { nextContractId = num; }
    else if (prefix == "AC" && num > nextAccountId)  { nextAccountId  = num; }
}
