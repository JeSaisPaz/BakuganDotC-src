// bdc 0x0890a188 UiSharedAnimIsDone
#include "bdc.h"

/* Returns the sticky done flag `0x08ac0e6c` of the shared animations; sets it once the first track
   of the animation in `slot` reaches `frame` (the recorded length `0x08ac0e68` when `frame` is -1).
    */

bool UiSharedAnimIsDone(void *owner, u32 frame, int slot)

{
  GfxFabClip *clip;
  u32 limit;
  
  if (g_uiSharedAnimDone != '\0') {
    return true;
  }
  limit = g_uiSharedAnimEndFrame;
  if (frame != 0xffffffff) {
    limit = frame;
  }
  if (((g_uiSharedAnims[slot] != (GfxFab *)0x0) &&
      (clip = GfxFabGetClip(g_uiSharedAnims[slot],0), clip != (GfxFabClip *)0x0)) &&
     (limit <= clip->frame)) {
    g_uiSharedAnimDone = '\x01';
  }
  return (bool)g_uiSharedAnimDone;
}

