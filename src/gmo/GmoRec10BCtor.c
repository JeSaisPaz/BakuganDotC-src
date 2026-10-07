// bdc 0x08a14230 GmoRec10BCtor
#include "bdc.h"

/* In-place constructor of the 0x10-byte records carved by `GmoPlanTakeRec10B`: sets the reference
   count `+0x0` to 1 and clears `+0x2` and `+0x4`. Returns `rec` (NULL-safe). */

GmoRec10B *GmoRec10BCtor(GmoRec10B *rec)

{
  if (rec != (void *)0x0) {
    rec->refCount = 1;
    rec->f02 = 0;
    rec->f04 = 0;
  }
  return rec;
}

