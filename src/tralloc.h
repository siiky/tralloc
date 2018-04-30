#ifndef _TRALLOC_H
#define _TRALLOC_H

/*
 * <stdbool.h>
 *  bool
 *  false
 *  true
 *
 * <stdio.h>
 *  FILE
 *  fprintf()
 *  stderr
 *  stdout
 *
 * <stdlib.h>
 *  calloc()
 *  free()
 *  malloc()
 *  realloc()
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Define TR_TRACE to use tralloc
 */
#ifndef TR_TRACE

#    define trinit()   do {} while (0)
#    define trdeinit() do {} while (0)

#    define trcalloc  calloc
#    define trfree    free
#    define trmalloc  malloc
#    define trrealloc realloc

/* if trace is not enabled, the tr*print() macros work as if there were no leaks */
#    define treprint()     true
#    define trfprint(OUTF) true
#    define trprint()      true

#else

#    define trinit                _trinit
#    define trdeinit              _trdeinit

#    define trcalloc(nmemb, size) _trcalloc((nmemb), (size), __FILE__, __func__, __LINE__)
#    define trfree(ptr)           _trfree((ptr))
#    define trmalloc(size)        _trmalloc((size), __FILE__, __func__, __LINE__)
#    define trrealloc(ptr, size)  _trrealloc((ptr), (size), __FILE__, __func__, __LINE__)

#    define treprint()            _treprint()
#    define trfprint(OUTF)        _trfprint((OUTF))
#    define trprint()             _trprint()

#endif /* TR_TRACE */

/**
 * @brief If there is unfreed memory, print (to stderr) each block
 *        in the format "file:function:line address size"
 * @returns `true` if there's no unfreed memory, `false` otherwise
 */
bool _treprint (void);

/**
 * @brief If there is unfreed memory, and @a stream is not NULL,
 *        print (to @a stream) each block in the format
 *        "file:function:line address size".
 * @param stream Where to write
 * @returns `true` if there's no unfreed memory, `false` otherwise
 */
bool _trfprint (FILE * stream);

/**
 * @brief If there is unfreed memory, print (to stdout) each block
 *        in the format "file:function:line address size"
 * @returns `true` if there's no unfreed memory, `false` otherwise
 */
bool _trprint (void);

/**
 * @brief Remove @a ptr from the list of unfreed memory and free it
 * @param ptr A pointer to dynamically allocated memory
 *
 * If @a ptr is not in the list, it works similarly to `stdlib.h`'s `free()`
 */
void _trfree (void * ptr);

/**
 * @brief Wrapper for `stdlib.h`'s `calloc()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a nmemb * @a size)
 *        and the address returned by `calloc()`
 * @param nmemb Number of members
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `calloc()`
 */
void * _trcalloc (size_t nmemb, size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Wrapper for `stdlib.h`'s `malloc()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a size)
 *        and the address returned by `malloc()`
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `malloc()`
 */
void * _trmalloc (size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Wrapper for `stdlib.h`'s `realloc()`. Record where the allocation
 *        occurred (@a file, @a func, @a line), its size (@a size)
 *        and the new address returned by `realloc()`. If @a ptr is
 *        already in the list, it is updated (if needed). If @a ptr
 *        is NULL, _trmalloc() is called instead. If @a size is 0,
 *        _trfree() is called instead, and NULL is returned.
 * @param ptr Pointer to some dynamically allocated block of memory
 * @param size Size of each member
 * @param file filename
 * @param func function name
 * @param line Line number
 * @returns Same as `realloc()`, except if @a ptr is not NULL and 
 *          @a size is 0, then NULL is returned (while `realloc()` may
 *          return either NULL or a pointer passable to `free()`)
 */
void * _trrealloc (void * ptr, size_t size, const char * file, const char * func, unsigned short line);

/**
 * @brief Initialize the trace structure
 */
void _trinit (void);

/**
 * @brief Deinitialize the trace structure (this includes free()ing
 *        unfreed memory)
 */
void _trdeinit (void);

#endif /* _TRALLOC_H */
