// bdc 0x0894dc64 UiNetMenuDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the multiplayer (ad-hoc) top menu (task id 1999): stores its id in
   the last-screen word `0x08ac0e78` and frees its sprites. Then `UiScreenDtor``(screen, 0)`;
   frees the object when `flags & 1`. */

void UiNetMenuDtor(UiScreen *screen, u32 flags)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  UiTextPrinter *printer;

  if (screen != (UiScreen *)0x0) {
    screen->base.vtable = &g_uiNetMenuVtable;
    GfxWaitGeIdle();
    printer = menu->helpPrinter;
    if (printer != (UiTextPrinter *)0x0) {
      const VtblEntry *vtbl = printer->layer.vtbl;
      ((void (*)(void *, int))vtbl[1].fn)((u8 *)printer + vtbl[1].delta, 3);
      menu->helpPrinter = (UiTextPrinter *)0x0;
    }
    screen->pad->stickEmulatesDpad = 0;
    g_lastScreenTaskId = screen->base.id;
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen, (char *)0, 0);
      MemUnlock();
    }
  }
}
