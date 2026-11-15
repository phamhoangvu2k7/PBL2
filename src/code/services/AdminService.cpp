#include "../../include/services/AdminService.h"
#include "../../include/helper/StringUtils.h"

AdminService::AdminService(AuthService& auth, CenterService& center) : auth(auth), center(center) {}

DeleteImpact AdminService::xem_truoc(const string& id, bool isTutor) const {
    DeleteImpact impact = isTutor ? center.xem_truoc_xoa_gs(id) : center.xem_truoc_xoa_hv(id);
    if (!impact.allowed) return impact;

    string error;
    if (!auth.co_the_xoa_tai_khoan(id, &error)) {
        impact.allowed = false;
        impact.reason = error;
        return impact;
    }
    impact.deletedAccounts = auth.tai_khoan_cua(id);
    return impact;
}

bool AdminService::xoa_nguoi(const string& id, bool isTutor, string* outError) {
    // kiem tra lai truoc khi sua, bi chan thi chua thay doi gi
    DeleteImpact impact = xem_truoc(id, isTutor);
    if (!impact.allowed) {
        StringUtils::gan(outError, impact.reason);
        return false;
    }

    // 1. lop
    if (isTutor) center.xoa_lop_mo_cua_gs(id);
    else center.tra_lop_cua_hv(id);
    // 2. tai khoan truoc ho so: neu dung giua chung, ProfileSync khong tao lai ho so rong
    if (!auth.xoa_tai_khoan_cua(id, outError)) return false;
    // 3. ho so
    bool removed = isTutor ? center.xoa_ho_so_gs(id) : center.xoa_ho_so_hv(id);
    if (!removed) StringUtils::gan(outError, "Khong xoa duoc ho so " + id + "!");
    return removed;
}
