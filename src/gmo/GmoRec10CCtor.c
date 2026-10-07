// bdc 0x08a14198 GmoRec10CCtor
#include "bdc.h"

/* In-place constructor of the 0x10-byte records carved by `GmoPlanTakeRec10C`: sets the reference
   count `+0x0` to 1 and clears the rest. Returns `rec` (NULL-safe). */

GmoRec10C *GmoRec10CCtor(GmoRec10C *rec)

{
  if (rec != (void *)0x0) {
    rec->refCount = 1;
    rec->f02 = 0;
    rec->f04 = 0;
    rec->f08 = 0;
    rec->f0c = 0;
  }
  return rec;
}

