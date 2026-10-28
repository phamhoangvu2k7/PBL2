#include "../../include/repositories/ContractRepository.h"
#include "../../include/helper/IdGenerator.h"

string ContractRepository::idOf(const Contract& c) const { return c.getContractId(); }

vector<string> ContractRepository::toRecord(const Contract& c) const {
    return {c.getContractId(), c.getTutorId(), c.getStudentId(), c.getStartDate(), c.getEndDate(),
            c.getSubjects(), c.getStatus()};
}

Contract* ContractRepository::fromRecord(const vector<string>& r) const {
    if (r.size() < 7 || r[0].empty()) return nullptr;
    IdGenerator::syncFromExistingId(r[0]);
    return new Contract(r[0], r[1], r[2], r[3], r[4], r[5], r[6]);
}
