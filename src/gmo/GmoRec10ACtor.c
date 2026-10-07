// bdc 0x08a14308 GmoRec10ACtor
#include "bdc.h"

/* In-place constructor of the 0x10-byte records carved by `GmoPlanTakeRec10A`: sets the reference
   count `+0x0` to 1 and clears the rest. Returns `rec` (NULL-safe). */

GmoRec10A *GmoRec10ACtor(GmoRec10A *rec)

{
  if (rec != (void *)0x0) {
    rec->refCount = 1;
    rec->f02 = 0;
    rec->f04 = 0;
    rec->f08 = 0;
    rec->f0a = 0;
    rec->f0c = 0;
    rec->f0d = 0;
    rec->f0e = 0;
  }
  return rec;
}

