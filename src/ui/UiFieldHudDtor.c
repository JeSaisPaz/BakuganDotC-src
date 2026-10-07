// bdc 0x088ceabc UiFieldHudDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the field HUD (task id 3001): destroys the attached task at `+0x74`
   if it still exists (`CoreTaskIsAlive`, `ActorPlayerCancelPowers`) and releases the helper at
   `+0x6c` (`UiFieldHudFaderDtor(.., 2)`). Then `UiScreenDtor``(screen, 0)`; frees the object when
   `flags & 1`. */

void UiFieldHudDtor(UiFieldHud *self, u32 flags)

{
  CoreTask *task;
  UiTextPrinter *printer;
  const VtblEntry *vt;
  bool alive;

  if (self != (UiFieldHud *)0x0) {
    (self->base).base.vtable = g_uiFieldHudVtbl;
    task = CoreTaskFind(500);
    if (task == (CoreTask *)0x0) {
      alive = false;
    }
    else {
      alive = CoreTaskIsAlive(task) != 0;
    }
    if (alive && self->player != (ActorPlayer *)0x0) {
      ActorPlayerCancelPowers(self->player);
    }
    printer = self->hintPrinter;
    if (printer != (UiTextPrinter *)0x0) {
      vt = (printer->layer).vtbl;
      ((void (*)(void *, s32))vt[1].fn)((u8 *)printer + vt[1].delta, 3);
      self->hintPrinter = (UiTextPrinter *)0x0;
    }
    UiFieldHudFaderDtor(&self->fader,2);
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

