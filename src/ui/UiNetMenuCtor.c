// bdc 0x0894db40 UiNetMenuCtor
#include "bdc.h"

/* Constructor of the multiplayer (ad-hoc) top menu, task id 1999 (0x7cf) (base `UiScreenCtor`,
   vtable `g_uiNetMenuVtable`). Object size 0x714. Allocates a 0x3c-byte sprite table, primes the
   fader, initialises its fields (`UiNetMenuCreateHelpText`) and sets the shared-background flag
   `g_uiKeepSharedBg`. Returns `screen`. Its main phase creates the lobby screen (task 2000,
   `UiNetLobbyCtor`), which returns to it. */

UiScreen *UiNetMenuCtor(UiScreen *screen)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  bool fromLow;
  void *data;
  GfxFader *fader;

  UiScreenCtor(&screen->base);
  screen->base.vtable = g_uiNetMenuVtable;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x3c, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  screen->data = data;
  UiScreenSetFrameMode(&screen->base, 1);
  menu->unk6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
  }
  menu->unk70 = 0;
  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  screen->pad->stickEmulatesDpad = 1;
  menu->cancelled = 0xff;
  UiNetMenuCreateHelpText(screen);
  memset(&menu->cursorMove, 0, sizeof(menu->cursorMove));
  g_uiKeepSharedBg = 1;
  return screen;
}
