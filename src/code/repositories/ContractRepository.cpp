#include "../../include/repositories/ContractRepository.h"
#include "../../include/helper/IdGenerator.h"

string ContractRepository::idOf(const Contract& c) const { return c.getContractId(); }

vector<string> ContractRepository::toRecord(const Contract& c) const {
    return {c.getContractId(), c.getTutorId(), c.getStudentId(), c.getStartDate(), c.getEndDate(),
            c.getSubjects(), c.getStatus(), c.getRequestedBy(), c.getEndReason(), c.getEndedOn()};
}

// file cu 7 cot (chua co thong tin ket thuc) van doc duoc
Contract* ContractRepository::fromRecord(const vector<string>& r) const {
    if (r.size() < 7 || r[0].empty()) return nullptr;
    IdGenerator::syncFromExistingId(r[0]);
    // ma nguoi trong lich su khong duoc cap lai du nguoi do da bi xoa
    IdGenerator::syncFromExistingId(r[1]);
    IdGenerator::syncFromExistingId(r[2]);
    Contract* c = new Contract(r[0], r[1], r[2], r[3], r[4], r[5], r[6]);
    if (r.size() >= 10) {
        c->setRequestedBy(r[7]);
        c->setEndReason(r[8]);
        c->setEndedOn(r[9]);
    }
    return c;
}
