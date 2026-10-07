// bdc 0x08a14108 GmoRec30BCtor
#include "bdc.h"

/* In-place constructor of the 0x30-byte records carved by `GmoPlanTakeRec30B`: sets the reference
   count `+0x0` to 1, `+0x20` to -1 and `+0x2c` to `0xffff0001`, clears the rest. Returns `rec`
   (NULL-safe). */

void *GmoRec30BCtor(void *rec)

{
  GmoRec30B *r = (GmoRec30B *)rec;
  if (r != NULL) {
    r->refCount = 1;
    r->f20 = -1;
    r->f2c = 1;
    r->f2e = -1;
    r->f02 = 0;
    r->f04 = 0;
    r->f08 = 0;
    r->f0c = 0;
    r->f10 = 0;
    r->f14 = 0;
    r->f16 = 0;
    r->f18 = 0;
    r->f24 = 0;
    r->f28 = 0;
  }
  return rec;
}
