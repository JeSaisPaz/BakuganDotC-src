// bdc 0x08a25ddc GmoImageGetBpp
#include "bdc.h"

/* Returns the bits per pixel of an image record (`+0x17`), 0 for NULL. */

u8 GmoImageGetBpp(const GmoImage *self)
{
  u8 bpp = 0;

  if (self != NULL) {
    bpp = self->bpp;
  }
  return bpp;
}
