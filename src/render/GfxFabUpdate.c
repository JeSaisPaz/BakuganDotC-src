// bdc 0x089f86b4 GfxFabUpdate
#include "bdc.h"

/* Advances a `.fab` animation by one frame: runs the clip update `GfxFabClipUpdate` on every clip
   of its clip list (`+0x88`). */

void GfxFabUpdate(GfxFab *fab)

{
  GfxFabClip *clip;

  for (clip = fab->clips; clip != NULL; clip = clip->next) {
    GfxFabClipUpdate(clip);
  }
}
