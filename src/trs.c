/* strdup() is not in C11 */
#define _XOPEN_SOURCE 500
/*
 * <stdlib.h>
 *  free()
 *
 * <string.h>
 *  memset()
 *  strdup()
 */
#include <stdlib.h>
#include <string.h>

#include <utils/ifnotnull.h>

#include "trs.h"

int trs_eq (struct trs a, struct trs b)
{
    return a.ptr == b.ptr;
}

struct trs _trs_free (struct trs self)
{
    ifnotnull(self.file, free);
    ifnotnull(self.func, free);
    ifnotnull(self.ptr, free);

    memset(&self, 0, sizeof(struct trs));
    return self;
}

struct trs _trs_new (size_t size, void * ptr, const char * file, const char * func, unsigned short line)
{
    return (struct trs) {
        .file = (file != NULL) ? strdup(file) : NULL,
        .func = (func != NULL) ? strdup(func) : NULL,
        .line = line,
        .size = size,
        .ptr = ptr,
    };
}
