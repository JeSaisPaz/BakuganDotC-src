// bdc 0x089bc53c SndMainThread
#include "bdc.h"

/* Entry of the sound main thread, "MyThread-Sound-Main" (slot 5 of `g_threadTable`, priority 18,
   32 KiB stack). It builds the whole sound runtime: `SndInit` (the `SndManager`), then
   `SndEmitterSystemCreate` (listener, emitter list/pool), `SndObjectMgrCreate` (sound objects)
   and `SndGroupLoaderCreate` (group loader). It then runs the manager step
   `SndManagerStep``(mgr)` back to back while the byte at `SndManager + 0x8be4` is 0; once that
   byte is non-zero it leaves the loop if it has stepped at least once, and otherwise sleeps 200 us
   (`sceKernelDelayThread`) and retries. After the loop it runs `SndShutdown` and returns 0. */

int SndMainThread(void)

{
  bool stepped;
  SndManager *mgr;

  stepped = false;
  SndInit();
  SndEmitterSystemCreate();
  SndObjectMgrCreate();
  SndGroupLoaderCreate();
  while (true) {
    while (mgr = SndGetManager(), mgr->stopFlag == '\0') {
      stepped = true;
      mgr = SndGetManager();
      SndManagerStep(mgr);
    }
    if (stepped) break;
    sceKernelDelayThread(200);
  }
  SndShutdown();
  return 0;
}
