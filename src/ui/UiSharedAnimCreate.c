// bdc 0x0890a048 UiSharedAnimCreate
#include "bdc.h"

/* Creates a `.fab` animation player (0xb0-byte `GfxFab`, allocated from the low end of the heap,
   constructor `GfxFabCtorFromPack` with parent `g_uiSharedAnimList`) for `data` in slot `slot` of
   `g_uiSharedAnims` (NULL when the allocation fails). If the fab has a first clip, records its frame
   in `g_uiSharedAnimFrame`; if that clip also has a definition, records its last frame in
   `g_uiSharedAnimEndFrame` and rewinds the fab to frame 0 (`GfxFabSeek`). `owner` is unused. */

void UiSharedAnimCreate(void *owner, void *data, int slot)

{
  bool fromLow;
  GfxFab *fab;
  GfxFab *made;
  GfxFabClip *clip;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  fab = MemAlloc(sizeof(GfxFab), (char *)0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  made = (GfxFab *)0;
  if (fab != (GfxFab *)0) {
    GfxFabCtorFromPack(fab, data, &g_uiSharedAnimList);
    made = fab;
  }
  g_uiSharedAnims[slot] = made;
  clip = GfxFabGetClip(g_uiSharedAnims[slot], 0);
  if (clip != (GfxFabClip *)0) {
    g_uiSharedAnimFrame = clip->frame;
    if (clip->def != (struct GfxFabClipDef *)0) {
      g_uiSharedAnimEndFrame = clip->def->lastFrame;
      GfxFabSeek(g_uiSharedAnims[slot], 0);
    }
  }
}
