// bdc 0x0890a3e4 UiSharedAnimStart
#include "bdc.h"

/* Starts a shared UI animation: when the array `g_uiSharedAnims` exists and `slot` is free, creates the
   player (`UiSharedAnimCreate`), sets its loop flag `loop` and its depth `depth` (f12), and its
   range `start`/`end` (f13/f14, `UiSharedAnimSetRange`). */

void UiSharedAnimStart(float depth, float start, float end, void *owner, void *data, int slot, u8 loop)

{
  if ((g_uiSharedAnims != (GfxFab **)0) && (g_uiSharedAnims[slot] == (GfxFab *)0)) {
    UiSharedAnimCreate(owner, data, slot);
    g_uiSharedAnims[slot]->loop = loop;
    g_uiSharedAnims[slot]->depth = depth;
    UiSharedAnimSetRange(start, end, owner, slot);
  }
}
