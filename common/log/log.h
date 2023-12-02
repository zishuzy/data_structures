#ifndef LOG_LOG
#define LOG_LOG

#include <stdio.h>

#define LOG_INFO(fmt, ...)  printf("[%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) fprintf(stderr, "[%s]: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)

#endif /* LOG_LOG */
