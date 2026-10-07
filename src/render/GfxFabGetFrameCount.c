// bdc 0x089f886c GfxFabGetFrameCount
#include "bdc.h"

/* Returns the frame count (u16 at `+0xc` of the clip chunk) of the fab's first clip, or 0. */

u16 GfxFabGetFrameCount(GfxFab *fab)

{
  GfxFabClip *clip;

  clip = fab->clips;
  if (clip != NULL && clip->def != NULL) {
    return clip->def->lastFrame;
  }
  return 0;
}
