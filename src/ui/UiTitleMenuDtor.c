// bdc 0x089501ac UiTitleMenuDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the title/system menu screen (task id 1000): stores its id in
   `g_lastScreenTaskId` and deletes its package (`+0x6c`, `IoLzsPackage`) through its virtual destructor. Then
   `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. */

/* Owned object whose class vtable pointer sits at +0x20. */
typedef struct VirtualOwner {
  u8 _unk[0x20];
  const VtblEntry *vtable;
} VirtualOwner;

void UiTitleMenuDtor(UiScreen *screen, u32 flags)

{
  UiTitleMenu *menu = (UiTitleMenu *)screen;
  IoLzsPackage *package;

  if (screen != (UiScreen *)0x0) {
    screen->base.vtable = &g_uiTitleMenuVtable;
    GfxWaitGeIdle();
    package = menu->package;
    if (package != (IoLzsPackage *)0x0) {
      const VtblEntry *entry = ((const VirtualOwner *)package)->vtable + 1;

      ((void (*)(void *, int))entry->fn)((char *)package + entry->delta, 3);
      menu->package = (IoLzsPackage *)0x0;
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
  return;
}
