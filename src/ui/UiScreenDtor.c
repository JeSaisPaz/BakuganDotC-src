// bdc 0x08909a80 UiScreenDtor
#include "bdc.h"

/* Destructor of the `UiScreen` base class (vtable `g_uiScreenVtable` slot 1), called at the end
   of every derived screen destructor: reinstalls the base vtable, waits for the GE, deletes the
   background animation list `bgAnimList` (+0x54) and frees the block `bgData` (+0x50). Unless
   `g_uiKeepSharedBg` is set, it also closes the shared background (`UiSharedBgClose`), deletes
   `g_uiSharedAnimList` and frees `g_uiSharedAnims`. Then it releases the two palette blenders
   (`paletteAnimB`, `paletteAnimA`), the per-class data block `data` (+0x1c) and the sprite layer
   (+0x18, virtual dtor), re-enables the suspended task `+0x10` (`CoreTaskClearFlags(t, 3)`), runs
   `CoreTaskDestroy` and frees the object when `flags & 1`. Does nothing for a NULL `screen`. */

void UiScreenDtor(UiScreen *screen, u32 flags)
{
  void *block;
  GfxSpriteLayer *layer;
  CoreTask *task;

  if (screen == NULL) {
    return;
  }
  screen->base.vtable = &g_uiScreenVtable;
  GfxWaitGeIdle();
  CoreObjectListDeleteAll((CoreObjectList *)&screen->bgAnimList);
  block = screen->bgData;
  if (block != NULL) {
    MemLock();
    MemFree(block, NULL, 0);
    MemUnlock();
    screen->bgData = NULL;
  }
  screen->bgData = NULL;
  if (g_uiKeepSharedBg == 0) {
    UiSharedBgClose();
    CoreObjectListDeleteAll((CoreObjectList *)&g_uiSharedAnimList);
    if (g_uiSharedAnims != NULL) {
      MemLock();
      MemFree(g_uiSharedAnims, NULL, 0);
      MemUnlock();
      g_uiSharedAnims = NULL;
    }
    g_uiSharedAnims = NULL;
  }
  if (screen->paletteAnimB != NULL) {
    GfxPaletteBlendDtor(screen->paletteAnimB, 3);
    screen->paletteAnimB = NULL;
  }
  if (screen->paletteAnimA != NULL) {
    GfxPaletteBlendDtor(screen->paletteAnimA, 3);
    screen->paletteAnimA = NULL;
  }
  block = screen->data;
  if (block != NULL) {
    MemLock();
    MemFree(block, NULL, 0);
    MemUnlock();
    screen->data = NULL;
  }
  layer = screen->spriteLayer;
  if (layer != NULL) {
    const VtblEntry *dtor = &layer->vtbl[1];
    ((void (*)(void *, int))dtor->fn)((u8 *)layer + dtor->delta, 3);
    screen->spriteLayer = NULL;
  }
  task = screen->suspendedTask;
  if (task != NULL) {
    CoreTaskClearFlags(task, 3);
  }
  CoreTaskDestroy(&screen->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(screen, NULL, 0);
    MemUnlock();
  }
}
