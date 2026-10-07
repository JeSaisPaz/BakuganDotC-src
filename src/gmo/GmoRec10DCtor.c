// bdc 0x08a140e4 GmoRec10DCtor
#include "bdc.h"

/* In-place constructor of the 0x10-byte records carved by `GmoPlanTakeRec10D`: sets the reference
   count `+0x0` to 1 and clears `+0x2`, `+0x4`, `+0x8`. Returns `rec` (NULL-safe). */

GmoRec10D *GmoRec10DCtor(GmoRec10D *rec)

{
  if (rec != (void *)0x0) {
    rec->refCount = 1;
    rec->f02 = 0;
    rec->f04 = 0;
    rec->f08 = 0;
  }
  return rec;
}

