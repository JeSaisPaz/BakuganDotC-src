// bdc 0x08a1e338 sceGuInit
#include "bdc.h"

/* Statically linked libgu `sceGuInit`: on first call fetches the GE eDRAM base
   (`sceGeEdramGetAddr`), resets the library state (`GfxGuInitContexts`), creates the
   `"SceGuSignal"` event flag (`g_guSignalEventFlag`), registers the GE signal/finish callbacks
   (`sceGeSetCallback`, both with the settings block starting at `g_guSignalCallback` as argument)
   and enqueues `g_guGeInitList`, waiting for it with `sceGeListSync`/`sceGeDrawSync`; then sets
   `g_guInitialized` and returns 0. A repeat call returns `0x80000020`; a failing
   `sceKernelCreateEventFlag`, `sceGeSetCallback` or `sceGeListEnQueue` returns its error code after
   rolling back the event flag and callback (ids reset to -1). */

s32 sceGuInit(void)
{
  s32 ret;
  SceGeCallbackData cb;

  ret = (s32)0x80000020;
  if (g_guInitialized == 0) {
    g_guEdramBase = sceGeEdramGetAddr();
    GfxGuInitContexts();
    ret = sceKernelCreateEventFlag("SceGuSignal", 0x200, 1, NULL);
    if (ret >= 0) {
      cb.finish_func = (SceGeCallback)GfxGuCallbackFin;
      g_guSignalEventFlag = ret;
      cb.signal_func = (SceGeCallback)GfxGuCallbackSig;
      cb.signal_arg = &g_guSignalCallback;
      cb.finish_arg = &g_guSignalCallback;
      ret = sceGeSetCallback(&cb);
      if (ret < 0) {
        sceKernelDeleteEventFlag(g_guSignalEventFlag);
      }
      else {
        g_guGeCallbackId = ret;
        ret = sceGeListEnQueue((void *)g_guGeInitList, NULL, -1, NULL);
        if (ret >= 0) {
          g_guListId = ret;
          sceGeListSync(ret, 0);
          sceGeDrawSync(0);
          g_guInitialized = 1;
          return 0;
        }
        sceKernelDeleteEventFlag(g_guSignalEventFlag);
        sceGeUnsetCallback(g_guGeCallbackId);
        g_guGeCallbackId = -1;
      }
      g_guSignalEventFlag = -1;
    }
  }
  return ret;
}
