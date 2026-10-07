// bdc 0x089f8850 GfxFabGetFrame
#include "bdc.h"

/* Returns the current frame (`+0x34`, low 16 bits) of the fab's first clip (`+0x88`), or 0 when it
   has none. */

u32 GfxFabGetFrame(GfxFab *fab)

{
  GfxFabClip *clip;

  clip = fab->clips;
  if (clip == NULL) {
    return 0;
  }
  return clip->frame & 0xffff;
}
