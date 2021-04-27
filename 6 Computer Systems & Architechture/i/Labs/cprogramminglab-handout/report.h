#include <stdbool.h>
#include <stdarg.h>

/* Default reporting level.  Must recompile when change */
#ifndef RPT
#define RPT 2
#endif

/* Ways to report interesting behavior and errors */

/* Things to report */
typedef enum { MSG_WARN, MSG_ERROR, MSG_FATAL } message_t;

/* Buffer sizes */
#define MAX_CHAR 512

void init_files(FILE *errfile, FILE *verbfile);

bool set_logfile(char *file_name);

extern int verblevel;
void set_verblevel(int level);

/* Optional function to call when fatal error encountered */
extern void (*fatal_fun)();

/* Error messages */
void report_event(message_t msg, char *fmt, ...);

/* Report useful information */
void report(int verblevel, char *fmt, ...);

/* Like report, but without return character */
void report_noreturn(int verblevel, char *fmt, ...);

/* Simple failure report.  Works even when malloc returns NULL */
void fail_fun(char *format, char *msg);

/* Signal safe reporting function */
void safe_report(int verblevel, char *msg);

/* Attempt to call malloc.  Fail when returns NULL */
void *malloc_or_fail(size_t bytes, char *fun_name);

/* Attempt to call calloc.  Fail when returns NULL */
void *calloc_or_fail(size_t cnt, size_t bytes, char *fun_name);

/* Attempt to call realloc.  Fail when returns NULL */
void *realloc_or_fail(void *old, size_t old_bytes, size_t new_bytes,
                      char *fun_name);

/* Attempt to save string.  Fail when malloc returns NULL */
char *strsave_or_fail(char *s, char *fun_name);

/** Time measurement.  **/

/* Time counted as fp number in seconds */
void init_time(double *timep);

/* Compute time since last call with this timer
   and reset timer */
double delta_time(double *timep);

/** Memory usage **/

/* Convert bytes to gigabytes */
double gigabytes(size_t bytes);
