#ifndef SIMPLELOG_H_
#define SIMPLELOG_H_

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define _LOG_ALERT 0x01
#define _LOG_ERROR 0x02
#define _LOG_WARNING 0x04
#define _LOG_INFO 0x08
#define _LOG_DEBUG 0x10
#define _LOG_ALL (_LOG_ALERT | _LOG_ERROR | _LOG_WARNING | _LOG_INFO | _LOG_DEBUG)

#define LOGWRITE(X, Y) \
  do { \
    if(X != NULL) { __LOGFILENAME__ = X; } \
    __LOGFILE__ = __FILE__; \
    __LOGLINE__ = __LINE__; \
    logWrite Y; \
  } \
  while(0)

#define LOGINIT(X, Y, Z, K) \
  do { \
    __LOGFILENAME__ = X; \
    __LOGFILESIZE__ = Y; \
    __NUMLOGFILE__ = Z; \
    __LOGTRACELEVEL__ = K; \
    LOGWRITE(NULL, (_LOG_INFO, "=========================================================")); \
  } \
  while(0)

    extern const char *__LOGFILENAME__;
    extern const char *__LOGFILE__;
    extern int __LOGLINE__;
    extern size_t __LOGFILESIZE__;
    extern int __NUMLOGFILE__;
    extern short __LOGTRACELEVEL__;

    void logWrite(int level, const char *format, ...);
    int checkFile(const char *filename, size_t maxfilesize, unsigned short numlogfile);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SIMPLELOG_H_ */
