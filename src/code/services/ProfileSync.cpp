#include "../../include/services/ProfileSync.h"

int syncProfilesFromAccounts(AuthService& auth, CenterService& center, int* outFailed) {
    auth.assignMissingPersonIds();

    int created = 0;
    int failed = 0;
    for (Account* a : auth.getAccounts()) {
        const string& role = a->getRole();
        if (role == "Student" || role == "Tutor") {
            const string& id = a->getPersonId();
            bool existed = (role == "Student") ? (center.findStudentById(id) != nullptr)
                                               : (center.findTutorById(id) != nullptr);
            if (!existed) {
                bool ok = (role == "Student")
                    ? center.ensureStudentProfile(id, a->getDisplayName(), a->getPhone(), a->getEmail())
                    : center.ensureTutorProfile(id, a->getDisplayName(), a->getPhone(), a->getEmail());
                if (ok) created++;
                else failed++;
            }
        }
    }
    if (outFailed != nullptr) *outFailed = failed;
    return created;
}
