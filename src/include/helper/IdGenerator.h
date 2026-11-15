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

    // them so 0 ben trai, them_so_0(5, 3) = 005
    static string them_so_0(int number, int width);

public:
    static string generateStudentId();
    static string generateTutorId();
    static string generateClassId();
    static string generateContractId();
    static string generateAccountId();

    // sau khi nap file, cap nhat bo dem
    // de ma moi khong trung ma cu
    static void dong_bo_ma(const string& existingId);

    // dat bo dem ve 1
    static void reset();

};

#endif
