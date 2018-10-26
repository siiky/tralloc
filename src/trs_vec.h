/*
 * "trs.h"
 *  _trs_free()
 *  struct trs
 *  trs_eq()
 */
#include "trs.h"

#define VEC_CFG_DATA_TYPE     struct trs
#define VEC_CFG_DATA_TYPE_CMP trs_cmp
#define VEC_CFG_DTOR          _trs_free
#define VEC_CFG_PREFIX        trs_
#define VEC_CFG_VEC           trs_vec
#include <utils/vec.h>
