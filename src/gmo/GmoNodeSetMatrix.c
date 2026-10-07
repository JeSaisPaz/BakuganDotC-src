// bdc 0x08a15b80 GmoNodeSetMatrix
#include "bdc.h"

/* Sets the explicit 4x4 matrix of a 0xc0-byte GMO node record: copies `m` into a 0x40-byte pool-0
   block at `+0x30` (allocated on demand, `GmoHeapAlloc`) and sets flag 0x10 in `+0x42`; NULL
   releases it. */

void GmoNodeSetMatrix(GmoNode *self, const float *m)
{
  float *dst;
  const float *end;

  if (self != (GmoNode *)0x0) {
    if (m == (const float *)0x0) {
      GmoHeapReleaseThunk(0, self->matrix);
      self->matrix = (float *)0x0;
      self->flags42 = self->flags42 & 0xffef;
    }
    else {
      dst = self->matrix;
      if (dst == (float *)0x0) {
        dst = GmoHeapAlloc(0, 0x40, 0x40);
        self->matrix = dst;
        if (dst == (float *)0x0) {
          return;
        }
        self->flags42 = self->flags42 | 0x10;
      }
      end = m + 0x10;
      do {
        float a = m[1];
        float b = m[2];
        float c = m[3];
        dst[0] = m[0];
        m = m + 4;
        dst[1] = a;
        dst[2] = b;
        dst[3] = c;
        dst = dst + 4;
      } while (m != end);
    }
  }
}
