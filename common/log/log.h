/**
 * @file log.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Simple logger
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef LOG_LOG
#define LOG_LOG

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define LOG_INFO(fmt, ...)                                                                  \
    do {                                                                                    \
        struct timeval tv;                                                                  \
        gettimeofday(&tv, NULL);                                                            \
        struct tm now_tm;                                                                   \
        char time_str_1[32];                                                                \
        memset(time_str_1, 0, 32);                                                          \
        strftime(time_str_1, 32, "%Y-%m-%d %H:%M:%S",                                       \
                 localtime_r((const time_t *)&tv.tv_sec, &now_tm));                         \
        char time_str_2[64];                                                                \
        memset(time_str_2, 0, 64);                                                          \
        snprintf(time_str_2, 64, "%s.%.06ld", time_str_1, (long)tv.tv_usec);                \
        printf("%s: [%s:%d] " fmt "\n", time_str_2, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0);

#ifdef DEBUG
    #define LOG_DEBUG(fmt, ...) LOG_INFO(fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(fmt, ...)
#endif
#define LOG_ERROR(fmt, ...) LOG_INFO(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG_INFO(fmt, ##__VA_ARGS__)

#define BOOL_STR(v) v ? "true" : "false"

#endif /* LOG_LOG */
