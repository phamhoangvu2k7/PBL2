#include "../../include/repositories/TutorRepository.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

string TutorRepository::idOf(const Tutor& t) const { return t.getId(); }

vector<string> TutorRepository::toRecord(const Tutor& t) const {
    return {t.getId(), t.getFullName(), t.getPhone(), t.getEmail(), t.getAddress(), t.getGender(),
            t.getDateOfBirth(), t.getSubjects(), t.getQualification(), to_string(t.getExpYears()),
            to_string(t.getHourlyRate()), t.getSchedule(), t.getIsAvailable() ? "1" : "0",
            t.getRegisteredClasses(), t.getAchievements()};
}

// file cu khong co cot thanh tich (cot thu 15) van doc duoc
Tutor* TutorRepository::fromRecord(const vector<string>& r) const {
    if (r.size() < 14 || r[0].empty()) return nullptr;
    IdGenerator::syncFromExistingId(r[0]);
    return new Tutor(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8],
                     StringUtils::toInt(r[9]), StringUtils::toDouble(r[10]), r[11], r[12] == "1", r[13],
                     r.size() > 14 ? r[14] : "");
}
