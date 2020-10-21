#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <stdlib.h>

#define OPTSTR ":vsc:h"
#define USAGE_FMT "Usage: %s [-v] [-s] [-c code] [-h]\n"

extern char* optarg;
extern int opterr, optind, optopt;

static const char* BIN_NAME = "orb";
static const char* PKG_NAME = "orbitals";
static const char* PKG_VERSION = "1.0.0";

void xlog(FILE* f, int type, const char* format, ...);
void log(FILE* f, const char* format, va_list args);
void usage(const char* progname);

int main(int argc, char **argv) {

    FILE* logf = NULL;
    logf = fopen ("oribital.log", "a+");
    if (logf) {
        xlog(logf, 3, "logger file created: [%d]", argc);
    }
    if (argc == 1) {
        usage(basename(argv[0]));
        fclose(logf);
        return 0;
    }
    if (!system(NULL)) {
        xlog(logf, 1, "Command processor is not available.");
        fclose(logf);
        return 0;
    }
    int opt = 0;
    opterr = 0;
    optopt = 0;
    optind = 0;
    optarg = NULL;
    
    while ((opt = getopt(argc, argv, OPTSTR)) != EOF) {
       switch(opt) {
           case 'v': {
                xlog(logf, 1, "%s by Tajuddin Khandaker, version %s", PKG_NAME, PKG_VERSION);
                break;
           }
           case 'c': {
                xlog(logf, 1, "code: %s", optarg);
                break;
           }
           case 's': {
                system("php artisan serve");
                break;
           }
           case ':': {
                xlog(logf, 0, "Option: -%c without argument", optopt);
                usage(basename(argv[0]));
                break;
           }
           case '?': {
                xlog(logf, 0, "Unrecognized option: -%c", optopt);
                usage(basename(argv[0]));
                break;
           }
           case 'h':
           default: {
                usage(basename(argv[0]));
                /* NOTREACHED */
                break;
           }
       }
    }
    fclose(logf);
    return 0;
}

void xlog(FILE* f, int type, const char* format, ...) {
    va_list args;
    va_start (args, format);

    char log_type = 0;
    switch (type) {
        case 0: log_type = 'E'; break;
        case 1: log_type = 'I'; break;
        case 2: log_type = 'W'; break;
        default: log_type = 'D';
    }
    time_t t;
    time(&t);
    char* time_log = ctime(&t);
    time_log[strlen(time_log)-1] = '\0';

    fprintf(f, "\n[%s][%c] *** [%s][%s(%d)] *** ", time_log, log_type, __FILE__, __FUNCTION__, __LINE__);

    log(f, format, args);
    va_end (args);
}

void log(FILE* f, const char* format, va_list args) {
    vfprintf (f, format, args);
}

void usage(const char* progname) {
   xlog(stderr, 1, USAGE_FMT, progname ? progname : PKG_NAME);
}