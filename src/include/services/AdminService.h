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

    DeleteImpact preview(const string& id, bool isTutor) const;
    bool deletePerson(const string& id, bool isTutor, string* outError);

public:
    AdminService(AuthService& auth, CenterService& center);

    DeleteImpact previewDeleteStudent(const string& id) const { return preview(id, false); }
    DeleteImpact previewDeleteTutor(const string& id) const { return preview(id, true); }
    bool deleteStudent(const string& id, string* outError = nullptr) { return deletePerson(id, false, outError); }
    bool deleteTutor(const string& id, string* outError = nullptr) { return deletePerson(id, true, outError); }
};

#endif
