#ifndef REGISTER_H
#define REGISTER_H

#include <stdbool.h>

// Kiểm tra tài khoản có tồn tại hay không
bool checkUsernameExist(const char *username);

// Hàm đăng ký tài khoản
bool registerAccount(const char *username, const char *password);

#endif
