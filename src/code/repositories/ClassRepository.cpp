#include "../../include/repositories/ClassRepository.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

string ClassRepository::idOf(const Class& c) const { return c.getClassId(); }

vector<string> ClassRepository::toRecord(const Class& c) const {
    return {c.getClassId(), c.getContractId(), c.getStudentId(), c.getTutorId(), c.getSubject(),
            c.getSchedule().chuoi_luu(), c.getStartDate(), c.getEndDate(), c.getStatus(),
            to_string(c.getTuitionFee())};
}

Class* ClassRepository::fromRecord(const vector<string>& r) const {
    if (r.size() < 10 || r[0].empty()) return nullptr;
    // lich sai dinh dang -> bo dong nay
    Schedule schedule;
    if (!Schedule::doc_lich(r[5], schedule)) return nullptr;
    IdGenerator::dong_bo_ma(r[0]);
    // ma nguoi trong lich su khong duoc cap lai du nguoi do da bi xoa
    IdGenerator::dong_bo_ma(r[2]);
    IdGenerator::dong_bo_ma(r[3]);
    return new Class(r[0], r[1], r[2], r[3], r[4], schedule, r[6], r[7], r[8], StringUtils::toDouble(r[9]));
}
