// bdc 0x0890f5ec UiRepairDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the card repair screen (task id 430): restores the fader and calls
   `GameStageSpawnFieldPoints`, destroys its text boxes. Then `UiScreenDtor``(screen, 0)`; frees the object
   when `flags & 1`. */

void UiRepairDtor(UiScreen *screen, u32 flags)

{
  UiRepair *repair = (UiRepair *)screen;
  GfxFader *fader;
  UiTextPrinter *obj;
  const VtblEntry *vt;

  if (screen != (UiScreen *)0x0) {
    repair->base.base.vtable = &g_uiRepairVtable;
    repair->base.pad->stickEmulatesDpad = repair->savedStickEmulatesDpad;
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
    GameStageSpawnFieldPoints();
    obj = (UiTextPrinter *)repair->textBox;
    if (obj != (UiTextPrinter *)0x0) {
      vt = &obj->layer.vtbl[1];
      ((void (*)(void *, s32))vt->fn)((u8 *)obj + vt->delta, 3);
      repair->textBox = (void *)0x0;
    }
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen, (const char *)0, 0);
      MemUnlock();
    }
  }
  return;
}
