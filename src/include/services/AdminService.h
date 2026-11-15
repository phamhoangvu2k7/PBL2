#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <string>
#include "AuthService.h"
#include "CenterService.h"
using namespace std;

// admin xoa hoc vien / gia su: dieu phoi AuthService (tai khoan) va CenterService (lop, ho so)
// xem truoc bang preview, roi xoa theo thu tu an toan: lop -> tai khoan -> ho so
class AdminService {
private:
    AuthService& auth;
    CenterService& center;

    DeleteImpact xem_truoc(const string& id, bool isTutor) const;
    bool xoa_nguoi(const string& id, bool isTutor, string* outError);

public:
    AdminService(AuthService& auth, CenterService& center);

    DeleteImpact xem_truoc_xoa_hoc_vien(const string& id) const { return xem_truoc(id, false); }
    DeleteImpact xem_truoc_xoa_gia_su(const string& id) const { return xem_truoc(id, true); }
    bool xoa_hoc_vien(const string& id, string* outError = nullptr) { return xoa_nguoi(id, false, outError); }
    bool xoa_gia_su(const string& id, string* outError = nullptr) { return xoa_nguoi(id, true, outError); }
};

#endif
