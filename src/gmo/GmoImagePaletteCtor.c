// bdc 0x08a1267c GmoImagePaletteCtor
#include "bdc.h"

/* In-place constructor of the 0x30-byte image palette/track records carved by
   `GmoImagePlanTakePalettes`: sets the reference count `+0x0` to 1, bytes `+0x28` = 1 and `+0x29`
   = 3, clears all other fields. Returns `rec` (NULL-safe). */

GmoImage *GmoImagePaletteCtor(GmoImage *self)

{
  if (self != (GmoImage *)0x0) {
    self->mipmapMode = '\x01';
    self->kind29 = '\x03';
    self->refCount = 1;
    self->auxFlags = 0;
    self->next = (GmoImage *)0x0;
    self->id = 0;
    self->format = 0;
    self->width = 0;
    self->height = 0;
    self->flags16 = '\0';
    self->bpp = '\0';
    self->widthAlign = '\0';
    self->heightAlign = '\0';
    self->flags1a = '\0';
    self->frameMask = 0;
    self->levels = (void **)0x0;
    self->levelCount = 0;
    self->frameCount = 0;
    self->aux2a = 0;
    self->userData = (void *)0x0;
  }
  return self;
}

