/**
 * @file utils_string.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Utilities for string
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "utils_string.h"

void split_string(char *str, char delim, int (*cb)(const char *data, void *ctx), void *ctx)
{
    if (!str || !cb) {
        return;
    }
    int abort = 0;
    char *p, *b;

    for (p = str, b = str; !abort && *p != '\0'; p++) {
        if (*p == delim) {
            *p = '\0';
            if (cb(b, ctx)) {
                abort = 1;
            }
            *p = delim;
            b = p + 1;
        }
    }
    if (abort && *b != '\0') {
        cb(b, ctx);
    }
}
