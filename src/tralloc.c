/* strdup() is not in C11 */
#define _XOPEN_SOURCE 500
/*
 * <string.h>
 *  strdup()
 */
#include <string.h>

#include <utils/ifnotnull.h>

/*
 * "trs.h"
 *  _trs_free()
 *  _trs_new()
 *  struct trs
 *
 * "trs_vec.h"
 *  struct trs_vec
 *  trs_find()
 *  trs_get_nth()
 *  trs_is_empty()
 *  trs_iter()
 *  trs_iter_idx()
 *  trs_iter_next()
 *  trs_itering()
 *  trs_len()
 *  trs_push()
 *  trs_set_nth()
 *  trs_swap_remove()
 */
#include "trs.h"
#include "trs_vec.h"

#include "tralloc.h"

static struct trs_vec * trvec = NULL;

static void * _tradd_new_entry (size_t size, void * ptr, const char * file, const char * func, unsigned short line)
{
    if (ptr != NULL) {
        struct trs trs = _trs_new(size, ptr, file, func, line);
        trs_push(trvec, trs);
    }

    return ptr;
}

void * _trcalloc (size_t nmemb, size_t size, const char * file, const char * func, unsigned short line)
{
    return _tradd_new_entry(size * nmemb,
            calloc(nmemb, size),
            file,
            func,
            line);
}

void _trfree (void * ptr)
{
    if (ptr != NULL) {
        struct trs tmp = _trs_new(0, ptr, NULL, NULL, 0);
        size_t idx = trs_find(trvec, tmp);

        if (idx < trs_len(trvec)) /* ptr was found */
            _trs_free(trs_swap_remove(trvec, idx));
    }
}

void * _trmalloc (size_t size, const char * file, const char * func, unsigned short line)
{
    return _tradd_new_entry(size,
            malloc(size),
            file,
            func,
            line);
}

#define _trupdate_field(trs, field)                    \
    do {                                               \
        /* new is empty, free old */                   \
        if (field == NULL) {                           \
            ifnotnull(trs.field, free);                \
            trs.field = NULL;                          \
            break;                                     \
        }                                              \
        /* old is empty, dup new */                    \
        if (trs.field == NULL) {                       \
            trs.field = strdup(field);                 \
            break;                                     \
        }                                              \
        /*                                             \
         * old is not empty and is different from new, \
         * free old and dup new                        \
         */                                            \
        if (strcmp(field, trs.field) != 0) {           \
            free(trs.field);                           \
            trs.field = strdup(field);                 \
        }                                              \
    } while (0)

void * _trrealloc (void * ptr, size_t size, const char * file, const char * func, unsigned short line)
{
    /*
     * calling realloc(ptr, size) where ptr == NULL is equivalent
     * to calling malloc(size)
     */
    if (ptr == NULL)
        return _trmalloc(size, file, func, line);

    /*
     * calling realloc(ptr, size) where ptr != NULL and size == 0 is
     * equivalent to calling free(ptr) and it may return either NULL
     * or a pointer passable to free()
     */
    if (size == 0) {
        _trfree(ptr);
        return NULL;
    }

    void * ret = NULL;
    size_t idx = 0;
    bool elem = false;

    /* ptr should already be in trvec */
    {
        struct trs tmp = _trs_new(0, ptr, NULL, NULL, 0);
        idx = trs_find(trvec, tmp);
        elem = idx < trs_len(trvec);
    }

    ret = realloc(ptr, size);

    if (elem) { /* ptr is in trvec */
        /*
         * get the entry for ptr, update it and put it back in trvec
         */

        struct trs trs = trs_get_nth(trvec, idx);

        /* update the file and func fields if they differ from the old ones */
        _trupdate_field(trs, file);
        _trupdate_field(trs, func);

        trs.line = line;

        if (ret != NULL) { /* allocation was successful */
            trs.ptr  = ret;
            trs.size = size;
        }

        trs_set_nth(trvec, idx, trs);
    } else if (ret != NULL) { /* ptr not in trvec but allocation was successful */
        /*
         * create a new entry, similar to _trmalloc()
         */
        _tradd_new_entry(size, ret, file, func, line);
    }

    return ret;
}
#undef _trupdate_field

bool _trfprint (FILE * stream)
{
    bool ret = trs_is_empty(trvec);
    if (stream != NULL && !ret) {
        size_t total = 0;

        for (trs_iter(trvec); trs_itering(trvec); trs_iter_next(trvec)) {
            struct trs tmp = trs_get_nth(trvec, trs_iter_idx(trvec));
            total += tmp.size;
            fprintf(stream, "%s:%s():%hu %p %zu\n", tmp.file, tmp.func, tmp.line, tmp.ptr, tmp.size);
        }

        fprintf(stream, "Total: %zu\n", total);
    }
    return ret;
}

bool _treprint (void)
{
    return _trfprint(stderr);
}

bool _trprint (void)
{
    return _trfprint(stdout);
}

void _trinit (void)
{
    trvec = trs_new();
}

void _trdeinit (void)
{
    trs_free(trvec);
}
