#include "../../include/helper/IdGenerator.h"
#include <sstream>
#include <iomanip>

// bat dau tu 1
int IdGenerator::nextStudentId  = 1;
int IdGenerator::nextTutorId    = 1;
int IdGenerator::nextClassId    = 1;
int IdGenerator::nextContractId = 1;
int IdGenerator::nextAccountId  = 1;

// them so 0 ben trai
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

void IdGenerator::reset() {
    nextStudentId  = 1;
    nextTutorId    = 1;
    nextClassId    = 1;
    nextContractId = 1;
    nextAccountId  = 1;
}

// dong bo bo dem sau khi nap file de khong trung ma
// tien to: SV, GS, CL, HD, AC
void IdGenerator::syncFromExistingId(const string& existingId) {
    if (existingId.length() < 3) return;

    string prefix = existingId.substr(0, 2);
    string numStr = existingId.substr(2);

    // phan so toi da 9 chu so de stoi khong tran
    if (numStr.length() > 9) return;
    for (char c : numStr) {
        if (c < '0' || c > '9') return;
    }

    int num = stoi(numStr) + 1;  // bo dem moi phai lon hon ma da co

    if (prefix == "SV" && num > nextStudentId)  { nextStudentId  = num; }
    else if (prefix == "GS" && num > nextTutorId)    { nextTutorId    = num; }
    else if (prefix == "CL" && num > nextClassId)    { nextClassId    = num; }
    else if (prefix == "HD" && num > nextContractId) { nextContractId = num; }
    else if (prefix == "AC" && num > nextAccountId)  { nextAccountId  = num; }
}
