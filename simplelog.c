#include "simplelog.h"

size_t __LOGFILESIZE__ = 1000000;
int __NUMLOGFILE__ = 3;
short __LOGTRACELEVEL__ = _LOG_ALL;

const char *__LOGFILENAME__ = NULL;
const char *__LOGFILE__;
int __LOGLINE__;

const char *__LOGLEVEL__[] = {0, "ALERT", "ERROR", 0, "WARNING", 0, 0, 0, "INFO", 0, 0, 0, 0, 0, 0, 0, "DEBUG", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
 * logWrite
 * return value :    void
 *
 */
void logWrite(int level, const char *format, ...) {
    int i;
    time_t ct;
    struct tm tmbuf;
    va_list ap;
    if ((__LOGTRACELEVEL__ & (level & _LOG_ALL)) == 0) {
        return;
    }
    checkFile(__LOGFILENAME__, __LOGFILESIZE__, __NUMLOGFILE__);

    va_start(ap, format);
    ct = time(NULL);
    localtime_r(&ct, &tmbuf);
    ct = time(NULL);
    localtime_r(&ct, &tmbuf);

    FILE *logstream = fopen(__LOGFILENAME__, "a+");
    if (!logstream) {
        perror("fopen");
        return;
    }
    /* print out */
    fprintf(logstream, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d %5.5d %10.10s:%5.5d %8s: ",
            tmbuf.tm_year + 1900,
            tmbuf.tm_mon + 1,
            tmbuf.tm_mday,
            tmbuf.tm_hour,
            tmbuf.tm_min,
            tmbuf.tm_sec,
            getpid(),
            __LOGFILE__, __LOGLINE__, __LOGLEVEL__[level % _LOG_ALL] ? __LOGLEVEL__[level % _LOG_ALL] : "*");
    /* print out remainder of message */
    vfprintf(logstream, format, ap);
    fprintf(logstream, "%s", "\n");
    va_end(ap);

    fclose(logstream);
}

/*
 * checkFile
 * return value :    0    esito positivo
 *                  -1    file non accessibile
 */
int checkFile(const char *filename, size_t maxfilesize, unsigned short numlogfile) {
    int i, ct;
    size_t size;
    struct stat buf;
    char logfilename[PATH_MAX + 1];
    char oldlogfilename[PATH_MAX + 1];
    char newlogfilename[PATH_MAX + 1];

    i = stat(filename, &buf);
    if (i == 0) {
        size = buf.st_size;
        /*fprintf(stdout,"file %s, size %ld\n",filename, size);*/
        if (size >= maxfilesize) {
            /*fprintf(stdout, "file exceeds max size %d\n",maxfilesize);*/

            snprintf(oldlogfilename, sizeof (oldlogfilename), "%s", filename);
            for (ct = numlogfile - 1; ct > 0; ct--) {
                snprintf(oldlogfilename, sizeof (oldlogfilename), "%s.%d", filename, ct - 1);
                snprintf(newlogfilename, sizeof (newlogfilename), "%s.%d", filename, ct);
                if ((i = stat(oldlogfilename, &buf)) == 0) {
                    /*fprintf(stdout,"renaming %s in %s\n",oldlogfilename,newlogfilename);*/
                    rename(oldlogfilename, newlogfilename);
                } /* if */
            } /* for */

            snprintf(newlogfilename, sizeof (newlogfilename), "%s.%d", filename, 0);
            rename(filename, newlogfilename);
        } /* size>=maxfilesize */

        return 0;
    }/* checkfile */
    else {
        return -1;
    }
}
