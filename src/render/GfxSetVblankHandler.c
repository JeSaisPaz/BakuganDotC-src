// bdc 0x08a0faac GfxSetVblankHandler
#include "bdc.h"

/* Installs (`handler` non-NULL: `sceKernelRegisterSubIntrHandler(PSP_VBLANK_INT 0x1e, id, handler,
   arg)` then `sceKernelEnableSubIntr`) or removes (`sceKernelReleaseSubIntrHandler`) a vblank
   sub-interrupt handler with number `id`. */

void GfxSetVblankHandler(s32 id, void *handler, void *arg)

{
  s32 r;
  if (handler == (void *)0x0) {
    sceKernelReleaseSubIntrHandler(0x1e,id);
  }
  else {
    r = sceKernelRegisterSubIntrHandler(0x1e,id,handler,arg);
    if (-1 < r) {
      sceKernelEnableSubIntr(0x1e,id);
    }
  }
  return;
}

