#ifndef _TRS_H
#define _TRS_H

/*
 * <stddef.h>
 *  size_t
 */
#include <stddef.h>

struct trs {
    /* file where this block of memory was allocated */
    char * file;
    /* function where this block of memory was allocated */
    char * func;
    /* line where this block of memory was allocated */
    unsigned short line;

    /* size of this block of memory */
    size_t size;
    /* pointer to the allocated block of memory */
    void * ptr;
};

int        trs_eq    (struct trs a, struct trs b);
struct trs _trs_free (struct trs self);
struct trs _trs_new  (size_t size, void * ptr, const char * file, const char * func, unsigned short line);

#endif /* _TRS_H */
