#include "login.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cJSON.h>

// Kiểm tra tài khoản và mật khẩu đăng nhập
bool loginAccount(const char *username, const char *password) {
    FILE *file = fopen("user.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        cJSON *user = cJSON_Parse(line);
        if (user) {
            cJSON *username_json = cJSON_GetObjectItem(user, "username");
            cJSON *password_json = cJSON_GetObjectItem(user, "password");
            if (username_json && password_json &&
                strcmp(username, username_json->valuestring) == 0 &&
                strcmp(password, password_json->valuestring) == 0) {
                cJSON_Delete(user);
                fclose(file);
                return true; // Đăng nhập thành công
            }
            cJSON_Delete(user);
        }
    }

    fclose(file);
    return false; // Tài khoản hoặc mật khẩu không đúng
}
