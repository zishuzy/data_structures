/**
 * @file utils_string.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Utilities for string
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef UTILS_UTILS_STRING
#define UTILS_UTILS_STRING

#include <stdint.h>

void split_string(char *str, char delim, int (*cb)(const char *data, void *ctx), void *ctx);

#endif /* UTILS_UTILS_STRING */
