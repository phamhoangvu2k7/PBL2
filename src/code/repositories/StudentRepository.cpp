#include "../../include/repositories/StudentRepository.h"
#include "../../include/helper/IdGenerator.h"
#include "../../include/helper/StringUtils.h"

string StudentRepository::idOf(const Student& s) const { return s.getId(); }

vector<string> StudentRepository::toRecord(const Student& s) const {
    return {s.getId(), s.getFullName(), s.getGrade(), s.getPhone(), s.getEmail(), s.getAddress(),
            s.getGender(), s.getDateOfBirth(), s.getSubjects(), s.getSchedule(),
            to_string(s.getBudget()), s.getHasTutor() ? "1" : "0", s.getRegisteredClasses()};
}

Student* StudentRepository::fromRecord(const vector<string>& r) const {
    if (r.size() < 13 || r[0].empty()) return nullptr;
    IdGenerator::syncFromExistingId(r[0]);
    return new Student(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], r[9],
                       StringUtils::toDouble(r[10]), r[11] == "1", r[12]);
}
