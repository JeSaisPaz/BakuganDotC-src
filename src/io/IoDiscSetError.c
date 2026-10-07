// bdc 0x089fa17c IoDiscSetError
#include "bdc.h"

/* Stores error code `err` in `error` of the `CODiscSimple` disc reader (`g_discSimple`),
   remapping two memory-stick errors when no card is inserted (`sceIoDevctl("fatms0:", 0x02425823)`
   fails or reports != 1): `0x80010013` becomes 2 and `0x80020321` becomes 1. */

void IoDiscSetError(IoDiscSimple *self, int err)
{
  int ret;
  int msInsertedA;
  int msInsertedB;

  if (err <= (int)0x80010013) {
    if (err > (int)0x80010012) {
      ret = sceIoDevctl("fatms0:", 0x02425823, NULL, 0, &msInsertedB, 4);
      if (ret >= 0) {
        ret = (msInsertedB == 1);
      }
      if (ret <= 0) {
        err = 2;
      }
    }
  } else if (err == (int)0x80020321) {
    ret = sceIoDevctl("fatms0:", 0x02425823, NULL, 0, &msInsertedA, 4);
    if (ret >= 0) {
      ret = (msInsertedA == 1);
    }
    if (ret <= 0) {
      err = 1;
    }
  }
  self->error = err;
}
