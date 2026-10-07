// bdc 0x089ce55c PadDtor
#include "bdc.h"

/* Destructor of the main controller object: runs `PadBaseDtor`(pad, 2) and, when bit 0 of `flags`
   is set, frees the object under `MemLock`. No-op for NULL. */

void PadDtor(PadState *pad, u32 flags)
{
  if (pad != NULL) {
    PadBaseDtor(pad, 2);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(pad, NULL, 0);
      MemUnlock();
    }
  }
}
