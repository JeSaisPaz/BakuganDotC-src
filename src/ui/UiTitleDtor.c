// bdc 0x08950b10 UiTitleDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiTitle screen (task id 200): reinstalls `g_uiTitleVtable`,
   waits for the GE, restores the pad's stick-as-d-pad byte, stores its task id in
   `g_lastScreenTaskId` (last closed screen), deletes its owned sprite object through its virtual
   destructor and clears save-profile word 0x17; then `UiScreenDtor``(screen, 0)` and frees the
   object when `flags & 1`. */

/* Owned object whose class vtable pointer sits at +0x74. */
typedef struct VirtualOwner {
  u8 _unk[0x74];
  const VtblEntry *vtable;
} VirtualOwner;

void UiTitleDtor(UiScreen *screen, u32 flags)

{
  UiTitle *title = (UiTitle *)screen;
  GfxSprite *sprite;

  if (screen != (UiScreen *)0x0) {
    screen->base.vtable = &g_uiTitleVtable;
    GfxWaitGeIdle();
    screen->pad->stickEmulatesDpad = 0;
    g_lastScreenTaskId = screen->base.id;
    sprite = title->sprite;
    if (sprite != (GfxSprite *)0x0) {
      const VtblEntry *entry = ((const VirtualOwner *)sprite)->vtable + 1;

      ((void (*)(void *, int))entry->fn)((char *)sprite + entry->delta, 3);
      title->sprite = (GfxSprite *)0x0;
    }
    SaveProfileSetWord(SaveGetProfile(), 0x17, 0);
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen, (char *)0, 0);
      MemUnlock();
    }
  }
  return;
}
