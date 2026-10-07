// bdc 0x08996e74 UiWorldMapDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiWorldMap screen (task id 310): reinstalls vtable
   `g_uiWorldMapVtbl`, waits for the GE, resets the frame mode, restores the pad's stick-as-d-pad
   byte (the saved value in rank mode, 0 otherwise), deletes the globe and jet models and the three
   text-slot printers through their virtual destructors, runs `UiHelpLineDestroy`, stores its task id
   in `g_lastScreenTaskId` (last closed screen), destroys the embedded globe camera; then
   `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiWorldMapDtor(UiScreen *screen, u32 flags)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  const VtblEntry *entry;
  int i;

  if (screen != (UiScreen *)0x0) {
    screen->base.vtable = g_uiWorldMapVtbl;
    GfxWaitGeIdle();
    UiScreenSetFrameMode(&screen->base, 1);
    if (UiWorldMapIsRankMode(screen) == 1) {
      screen->pad->stickEmulatesDpad = map->savedStickEmulatesDpad;
    } else {
      screen->pad->stickEmulatesDpad = 0;
    }
    if (map->mapModel != (GfxModel *)0x0) {
      entry = (const VtblEntry *)map->mapModel->base.vtable + 1;
      ((void (*)(void *, int))entry->fn)((char *)map->mapModel + entry->delta, 3);
      map->mapModel = (GfxModel *)0x0;
    }
    if (map->jetModel != (GfxModel *)0x0) {
      entry = (const VtblEntry *)map->jetModel->base.vtable + 1;
      ((void (*)(void *, int))entry->fn)((char *)map->jetModel + entry->delta, 3);
      map->jetModel = (GfxModel *)0x0;
    }
    UiHelpLineDestroy();
    for (i = 0; i < 3; i++) {
      UiTextPrinter *printer = map->textSlot[i].printer;

      if (printer != (UiTextPrinter *)0x0) {
        entry = printer->layer.vtbl + 1;
        ((void (*)(void *, int))entry->fn)((char *)printer + entry->delta, 3);
        map->textSlot[i].printer = (UiTextPrinter *)0x0;
      }
    }
    g_lastScreenTaskId = screen->base.id;
    GfxCameraDtor(&map->camera.base, 2);
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
