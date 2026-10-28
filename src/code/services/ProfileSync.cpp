#include "../../include/services/ProfileSync.h"

int syncProfilesFromAccounts(AuthService& auth, CenterService& center, int* outFailed) {
    auth.assignMissingPersonIds();

    int created = 0;
    int failed = 0;
    Node<Account*>* cur = auth.getAccountList().getHead();
    while (cur != nullptr) {
        Account* a = cur->data;
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
        cur = cur->next;
    }
    if (outFailed != nullptr) *outFailed = failed;
    return created;
}
