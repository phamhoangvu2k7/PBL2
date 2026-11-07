#include "../../include/services/AdminService.h"
#include "../../include/helper/StringUtils.h"

AdminService::AdminService(AuthService& auth, CenterService& center) : auth(auth), center(center) {}

DeleteImpact AdminService::preview(const string& id, bool isTutor) const {
    DeleteImpact impact = isTutor ? center.previewRemoveTutor(id) : center.previewRemoveStudent(id);
    if (!impact.allowed) return impact;

    string error;
    if (!auth.canRemoveAccountsOfPerson(id, &error)) {
        impact.allowed = false;
        impact.reason = error;
        return impact;
    }
    impact.deletedAccounts = auth.accountsOfPerson(id);
    return impact;
}

bool AdminService::deletePerson(const string& id, bool isTutor, string* outError) {
    // kiem tra lai truoc khi sua, bi chan thi chua thay doi gi
    DeleteImpact impact = preview(id, isTutor);
    if (!impact.allowed) {
        StringUtils::assign(outError, impact.reason);
        return false;
    }

    // 1. lop
    if (isTutor) center.removeTutorOpenClasses(id);
    else center.releaseStudentClasses(id);
    // 2. tai khoan truoc ho so: neu dung giua chung, ProfileSync khong tao lai ho so rong
    if (!auth.removeAccountsOfPerson(id, outError)) return false;
    // 3. ho so
    bool removed = isTutor ? center.removeTutorProfile(id) : center.removeStudentProfile(id);
    if (!removed) StringUtils::assign(outError, "Khong xoa duoc ho so " + id + "!");
    return removed;
}
