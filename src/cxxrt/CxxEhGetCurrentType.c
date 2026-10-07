// bdc 0x08a03ec0 CxxEhGetCurrentType
#include "bdc.h"

/* Returns the current exception's type (`+4`), qualifier flags (`+0xc`) and extra word (`+0x10`)
   through the three out pointers. */

void CxxEhGetCurrentType(void **type, u8 *flags, u32 *extra)

{
  *type = ((CxxEhRecord *)g_cxxEhCurrentException)->type;
  *flags = ((CxxEhRecord *)g_cxxEhCurrentException)->flags;
  *extra = ((CxxEhRecord *)g_cxxEhCurrentException)->extra;
}
