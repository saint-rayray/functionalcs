/* Test support code */

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "report.h"

/* Our program needs to use regular malloc/free */
#define INTERNAL 1
#include "harness.h"

/** Special values **/

/* Byte to fill newly malloced space with */
#define FILLCHAR 0x55

/** Data structures used by our code **/

static size_t allocated_count = 0;
/* Percent probability of malloc failure */
int fail_probability = 0;
static bool cautious_mode = true;
static bool noallocate_mode = false;
static volatile bool error_occurred = false;
static char *volatile error_message = "";

static int time_limit = 1;

/*
 * Data for managing exceptions
 */
static jmp_buf env;
static volatile sig_atomic_t jmp_ready = false;
static bool time_limited = false;

/*
  Internal functions
 */
/* Should this allocation fail? */
static bool fail_allocation(void)
{
    double weight = (double)random() / RAND_MAX;
    return (weight < 0.01 * fail_probability);
}

/*
  Implementation of application functions
 */
void *test_malloc(size_t size)
{
    if (noallocate_mode)
    {
        report_event(MSG_FATAL, "Calls to malloc disallowed");
        return NULL;
    }
    if (fail_allocation())
    {
        report_event(MSG_WARN, "Malloc returning NULL");
        return NULL;
    }

    void *p = malloc(size);
    if (p == NULL)
    {
        report_event(MSG_FATAL, "Couldn't allocate any more memory");
        error_occurred = true;
        return p;
    }
    memset(p, FILLCHAR, size);
    allocated_count++;
    return p;
}

void *test_calloc(size_t num, size_t size)
{
    if (num > SIZE_MAX / size)
    {
        return NULL;
    }
    void *mem = test_malloc(num * size);
    if (mem != NULL)
    {
        memset(mem, 0, num * size);
    }
    return mem;
}

void *test_realloc(void *ptr, size_t size)
{
    report_event(MSG_FATAL, "Calls to realloc disallowed");
    return NULL;
}

void test_free(void *p)
{
    if (noallocate_mode)
    {
        report_event(MSG_FATAL, "Calls to free disallowed");
        return;
    }
    if (p == NULL)
    {
        return;
    }

    free(p);
    allocated_count--;
}

size_t allocation_check(void)
{
    return allocated_count;
}

/*
  Implementation of functions for testing
 */

/*
  Set/unset cautious mode.
  In this mode, makes extra sure any block to be freed is currently allocated.
*/
void set_cautious_mode(bool cautious)
{
    cautious_mode = cautious;
}

/*
  Set/unset restricted allocation mode.
  In this mode, calls to malloc and free are disallowed.
 */
void set_noallocate_mode(bool noallocate)
{
    noallocate_mode = noallocate;
}

/*
  Return whether any errors have occurred since last time set error limit
 */
bool error_check(void)
{
    bool e = error_occurred;
    error_occurred = false;
    return e;
}

/*
 * Prepare for a risky operation using setjmp.
 * Function returns true for initial return, false for error return
 */
bool exception_setup(bool limit_time)
{
    if (sigsetjmp(env, 1))
    {
        /* Got here from longjmp */
        jmp_ready = false;
        if (time_limited)
        {
            alarm(0);
            time_limited = false;
        }
        if (error_message)
        {
            report_event(MSG_ERROR, error_message);
        }
        error_message = "";
        return false;
    }
    else
    {
        /* Got here from initial call */
        jmp_ready = true;
        if (limit_time)
        {
            alarm(time_limit);
            time_limited = true;
        }
        return true;
    }
}

/*
 * Call once past risky code
 */
void exception_cancel(void)
{
    if (time_limited)
    {
        alarm(0);
        time_limited = false;
    }
    jmp_ready = false;
    error_message = "";
}

/*
 * Use longjmp to return to most recent exception setup
 */
void trigger_exception(char *msg)
{
    error_occurred = true;
    error_message = msg;
    if (jmp_ready)
        siglongjmp(env, 1);
    else
        exit(1);
}
