// bdc 0x0893d6c0 UiPasscodeDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the passcode (symbol sequence) puzzle screen (task id 374): stores
   its id in the last-screen word `g_lastScreenTaskId`, resumes field task 500 and restores the
   pad's stick emulation. Then `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. */

void UiPasscodeDtor(UiScreen *screen, u32 flags)

{
  UiPasscode *self = (UiPasscode *)screen;
  CoreTask *task;

  if (screen != (UiScreen *)0x0) {
    (screen->base).vtable = &g_uiPasscodeVtbl;
    GfxWaitGeIdle();
    task = CoreTaskFind(500);
    if (task != (CoreTask *)0x0) {
      CoreTaskClearFlags(task, 1);
    }
    UiScreenSetFrameMode(&screen->base, 1);
    screen->pad->stickEmulatesDpad = self->savedStickEmu;
    g_lastScreenTaskId = (screen->base).id;
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
