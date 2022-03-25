#ifndef MYY_LOG_H
#define MYY_LOG_H 1

#include <string.h>


#include <stdio.h>
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define LOG_ERRNO(...)  fprintf(stderr, "Error : %s\n", strerror(errno)); fprintf(stderr, __VA_ARGS__)

#endif
