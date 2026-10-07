// bdc 0x08a108d8 GmoTextureIsDynamic
#include "bdc.h"

/* Returns the dynamic bit (0x10 of the flags half-word `+2`) of a GMO texture record: 1 for a
   texture that must be deep-copied rather than shared when a model is instanced, else 0. */

u32 GmoTextureIsDynamic(GmoTexture *tex)
{
  u32 dynamic = 0;
  if (tex != NULL) {
    dynamic = (tex->flags >> 4) & 1;
  }
  return dynamic;
}
