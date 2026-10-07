// bdc 0x08a1017c GmoImageAlignClass
#include "bdc.h"

/* Maps an alignment to the allocation-plan class: <= 4 -> 3, <= 0x10 -> 2, <= 0x40 -> 1, larger ->
   0. */

u8 GmoImageAlignClass(u32 align)
{
  u8 cls = 3;

  if ((4 < align) && (cls = 2, 0x10 < align)) {
    cls = align < 0x41;
  }
  return cls;
}
