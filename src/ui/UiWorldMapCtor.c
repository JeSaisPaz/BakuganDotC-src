// bdc 0x08996cec UiWorldMapCtor
#include "bdc.h"

/* Constructor of UiWorldMap, the story-mode world map (`menu_worldmap.gmo`, `fz_maruchojet_02.gmo`,
   country flags `hata_*`, stages `stage_*`, `DWWorldName` area names); task id 310 (0x136), object
   size 0x2380. Runs `UiScreenCtor`, installs `g_uiWorldMapVtbl`, constructs the globe camera
   `camera` (`GfxCameraCtor`), saves the pad's `stickEmulatesDpad` byte and forces it to 1, runs
   `UiWorldMapInitState`, allocates (low heap) the 0x178-byte per-screen `data` block and the
   zeroed 8-byte `g_uiSharedAnims` table, sets frame mode 0 (`UiScreenSetFrameMode`), creates
   the fader system if needed (sort key 20000), sets the clear colour to opaque black, refreshes
   profile flag 0 (`SaveRefreshProfileFlag0`), creates the text slots
   (`UiWorldMapCreateTextSlots`), keeps the shared background (`UiScreenKeepSharedBg`) and
   creates the help line (`UiHelpLineCreate`). Returns `screen`. */

UiScreen *UiWorldMapCtor(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  bool fromLow;
  void *block;
  PadState *pad;

  UiScreenCtor(&screen->base);
  screen->base.vtable = g_uiWorldMapVtbl;
  GfxCameraCtor(&map->camera.base);
  pad = screen->pad;
  map->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  UiWorldMapInitState(screen);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(0x178, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  screen->data = block;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(8, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = block;
  memset(block, 0, 8);

  UiScreenSetFrameMode(&screen->base, 0);
  map->unk6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    GfxGetActiveFader()->sortKey = 20000.0f;
  }
  map->unk70 = 0;

  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  SaveRefreshProfileFlag0();
  screen->unk24 = 0;
  UiWorldMapCreateTextSlots(screen);
  UiScreenKeepSharedBg(screen);
  UiHelpLineCreate();
  return screen;
}
