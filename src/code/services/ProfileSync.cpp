#include "../../include/services/ProfileSync.h"

int dong_bo_ho_so(AuthService& auth, CenterService& center, int* outFailed) {
    int created = 0;
    int failed = 0;
    for (Account* a : auth.ds_tai_khoan()) {
        const string& role = a->getRole();
        if (role == "Student" || role == "Tutor") {
            const string& id = a->getPersonId();
            bool existed = (role == "Student") ? (center.tim_hoc_vien(id) != nullptr)
                                               : (center.tim_gia_su(id) != nullptr);
            if (!existed) {
                bool ok = (role == "Student")
                    ? center.dam_bao_ho_so_hv(id, a->getDisplayName(), a->getPhone(), a->getEmail())
                    : center.dam_bao_ho_so_gs(id, a->getDisplayName(), a->getPhone(), a->getEmail());
                if (ok) created++;
                else failed++;
            }
        }
    }
    if (outFailed != nullptr) *outFailed = failed;
    return created;
}
