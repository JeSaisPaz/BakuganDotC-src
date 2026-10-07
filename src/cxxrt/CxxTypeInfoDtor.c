// bdc 0x08a04350 CxxTypeInfoDtor
#include "bdc.h"

/* Destructor of `std::type_info` (vtable `g_cxxTypeInfoVtable`): restores the vtable and frees the object
   when `flags & 1`. A NULL `self` is ignored. */

void CxxTypeInfoDtor(void *self, u32 flags)
{
  if (self != (void *)0x0) {
    *(void **)self = g_cxxTypeInfoVtable;
    if ((flags & 1) != 0) {
      CxxOperatorDelete(self);
    }
  }
}
