#ifndef PROFILE_SYNC_H
#define PROFILE_SYNC_H

#include "AuthService.h"
#include "CenterService.h"

// tao ho so Student/Tutor tu tai khoan:
// 1. cap ma SV/GS cho tai khoan moi
// 2. tai khoan chua co ho so thi tao tu ten / sdt / email
// goi sau moi lan dang nhap, tra ve so ho so vua tao,
// outFailed nhan so tai khoan khong tao duoc
int syncProfilesFromAccounts(AuthService& auth, CenterService& center, int* outFailed = nullptr);

#endif
