// bdc 0x089098cc UiScreenCtor
#include "bdc.h"

/* Base constructor of the UI screen task classes: runs `CoreTaskInit`, installs the screen base
   vtable `g_uiScreenVtable`, clears the state words, allocates a 0x80-byte sprite layer from the
   low heap (`GfxSpriteLayerCtor`, marked sorted) into `spriteLayer`, makes sure the UI text
   renderer exists (`UiTextRenderEnsure`) and sets its box's first float to 2500, points `pad` at
   the global pad `g_padState` and disables the pad's d-pad-as-stick emulation. If
   `g_uiKeepSharedBg` is clear it also resets the shared screen globals (`g_uiSharedAnims`, the
   list `g_uiSharedAnimList`, the animation frame/end/done state, `g_uiSharedAnimAux` and the
   0x20-byte `g_uiSharedBlock`); otherwise it only clears the flag. Returns `task`. */

CoreTask *UiScreenCtor(CoreTask *task)

{
  UiScreen *screen = (UiScreen *)task;
  bool fromLow;
  GfxSpriteLayer *mem;
  GfxSpriteLayer *layer;
  PadState *pad;
  float *box;

  CoreTaskInit(task);
  screen->base.vtable = &g_uiScreenVtable;
  screen->phase = 0;
  screen->phaseStep = 0;
  screen->unk30 = 0;
  screen->closeRequested = 0;
  screen->suspendedTask = NULL;
  screen->self = screen;
  screen->paletteAnimA = NULL;
  screen->paletteAnimB = NULL;
  layer = NULL;
  screen->pulseTimer = 0;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 0);
    layer = mem;
  }
  screen->spriteLayer = layer;
  layer->sorted = 1;
  screen->data = NULL;
  UiTextRenderEnsure();
  box = UiTextRenderGetBox();
  *box = 2500.0f;
  pad = g_padState;
  screen->pad = pad;
  pad->dpadEmulatesStick = 0;
  screen->bgData = NULL;
  screen->unk58 = 0;
  screen->bgAnimList = NULL;
  screen->unk5c = 0;
  screen->unk68 = 0;
  screen->unk60 = 0;
  screen->unk64 = 0;
  if (g_uiKeepSharedBg == 0) {
    g_uiSharedAnims = NULL;
    g_uiSharedAnimListTail = NULL;
    g_uiSharedAnimList = NULL;
    g_uiSharedAnimListCount = 0;
    g_uiSharedAnimDone = 0;
    g_uiSharedAnimFrame = 0;
    g_uiSharedAnimEndFrame = 0;
    g_uiSharedAnimAux = 0;
    memset(g_uiSharedBlock, 0, 0x20);
  }
  else {
    g_uiKeepSharedBg = 0;
  }
  return task;
}
