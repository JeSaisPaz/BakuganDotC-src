// bdc 0x08950914 UiTitleCtor
#include "bdc.h"

/* Constructor of UiTitle, the title screen (`title.fab`, `title_logo.fab`, `title_haikei.fab`);
   task id 200 (0xc8), object size 0xac. Runs `UiScreenCtor`, installs `g_uiTitleVtable`,
   allocates the 0x20-byte per-screen data block (`data`) and a zeroed 0x10-byte `bgData` from the
   low heap, sets frame mode 0 (`UiScreenSetFrameMode`), creates the fader system and the shared
   text box (packet depth 2000.0) if missing, sets the clear colour to opaque black, makes the stick
   emulate the d-pad, clears the script globals and the title state, and when a player profile
   exists wipes it back to defaults. Returns `screen`. */

UiScreen *UiTitleCtor(UiScreen *screen)

{
  UiTitle *title = (UiTitle *)screen;
  bool wasLow;
  void *block;
  UiTextBox *box;
  int i;

  UiScreenCtor(&screen->base);
  screen->base.vtable = &g_uiTitleVtable;
  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(0x20, NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  screen->data = block;
  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(0x10, NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  screen->bgData = block;
  memset(block, 0, 0x10);
  UiScreenSetFrameMode(&screen->base, 0);
  title->unk6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
  }
  if (!UiTextRenderExists()) {
    UiTextRenderEnsure();
    box = (UiTextBox *)UiTextRenderGetBox();
    box->packetDepth = 2000.0f;
  }
  title->unk70 = 0;
  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  title->animAngle = 0.0f;
  screen->unk30 = 0;
  title->idleFrames = 0;
  screen->pad->stickEmulatesDpad = 1;
  title->sprite = NULL;
  title->choiceIndex = 0;
  ScriptVarsClearGlobals();
  for (i = 0; i < 3; i++) {
    title->unk7c[i] = 0.0f;
  }
  if (SaveHasProfile()) {
    memset(&SaveGetProfile()->data->stageProfileByte, 0, 0xb6);
    SaveProfileReset(SaveGetProfile());
    SaveProfileResetWordTable(SaveGetProfile(), true);
  }
  title->timer = 0;
  title->skipFrameA = 0;
  title->skipFrameB = 0;
  title->repertFrame = 0;
  title->skipFrameC = 0;
  return screen;
}
