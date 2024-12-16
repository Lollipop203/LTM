#include "register.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cJSON.h>

// Kiểm tra xem tên tài khoản có tồn tại trong file user.txt không
bool checkUsernameExist(const char *username) {
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
            if (username_json && strcmp(username, username_json->valuestring) == 0) {
                cJSON_Delete(user);
                fclose(file);
                return true; // Tên tài khoản đã tồn tại
            }
            cJSON_Delete(user);
        }
    }

    fclose(file);
    return false; // Tên tài khoản không tồn tại
}

// Hàm đăng ký tài khoản mới
bool registerAccount(const char *username, const char *password) {
    if (checkUsernameExist(username)) {
        return false; // Tên tài khoản đã tồn tại
    }

    // Mở file để ghi tài khoản mới
    FILE *file = fopen("user.txt", "a");
    if (file == NULL) {
        printf("Error opening file\n");
        return false;
    }

    // Tạo JSON cho tài khoản mới
    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "username", username);
    cJSON_AddStringToObject(user, "password", password);

    // Ghi thông tin tài khoản vào file
    char *json_str = cJSON_Print(user);
    fprintf(file, "%s\n", json_str);

    // Dọn dẹp và đóng file
    cJSON_Delete(user);
    free(json_str);
    fclose(file);

    return true; // Đăng ký thành công
}
