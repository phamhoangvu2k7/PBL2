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
string IdGenerator::them_so_0(int number, int width) {
    ostringstream oss;
    oss << setfill('0') << setw(width) << number;
    return oss.str();
}

string IdGenerator::generateStudentId() {
    return "SV" + them_so_0(nextStudentId++, 3);
}

string IdGenerator::generateTutorId() {
    return "GS" + them_so_0(nextTutorId++, 3);
}

string IdGenerator::generateClassId() {
    return "CL" + them_so_0(nextClassId++, 3);
}

string IdGenerator::generateContractId() {
    return "HD" + them_so_0(nextContractId++, 3);
}

string IdGenerator::generateAccountId() {
    return "AC" + them_so_0(nextAccountId++, 3);
}

void IdGenerator::reset() {
    nextStudentId  = 1;
    nextTutorId    = 1;
    nextClassId    = 1;
    nextContractId = 1;
    nextAccountId  = 1;
}

// dong bo bo dem sau khi nap file de khong trung ma
void IdGenerator::dong_bo_ma(const string& existingId) {
    if (existingId.length() < 3) return;

    string prefix = existingId.substr(0, 2);
    string numStr = existingId.substr(2);

    if (numStr.length() > 9) return;
    for (char c : numStr) {
        if (c < '0' || c > '9') return;
    }

    int num = stoi(numStr) + 1; // tang 1 ma

    if (prefix == "SV" && num > nextStudentId)  { nextStudentId  = num; }
    else if (prefix == "GS" && num > nextTutorId)    { nextTutorId    = num; }
    else if (prefix == "CL" && num > nextClassId)    { nextClassId    = num; }
    else if (prefix == "HD" && num > nextContractId) { nextContractId = num; }
    else if (prefix == "AC" && num > nextAccountId)  { nextAccountId  = num; }
}
