#ifndef PROFILE_SYNC_H
#define PROFILE_SYNC_H

#include "AuthService.h"
#include "CenterService.h"

int dong_bo_ho_so(AuthService& auth, CenterService& center, int* outFailed = nullptr);

#endif
