// bdc 0x089bb41c main
#include "bdc.h"

/* The C `main` of the game, called from `BootUserMainThread` (the `user_main` thread, whose entry
   the crt0 `BootEntry` creates) as `main(argc, argv)`: installs the exit/power callbacks
   (`BootRegisterCallbacks`), creates the heap (`MemInit`) and the power, stopwatch and
   memory-stick services, starts the seven initial threads (`BootStartInitialThreads`), then runs
   a vblank-synchronised service loop (power and memory-stick updates) until every boot thread has
   ended, or until an exit is requested while the power state is suspended (then it raises the
   sound manager's `stopFlag`). Finally waits for the sound manager to go away and calls
   `sceKernelExitGame`. Returns 0 (only reached when callback registration fails; `argc`/`argv`
   are unused). */

int main(int argc, char **argv)

{
  CxxInit();
  if (BootRegisterCallbacks() != 0) {
    MemInit(g_sceNewlibHeapKbSize * 0x400 - 0x400);
    MemWalkBlocks();
    CorePowerInit();
    CoreStopwatchInit(8);
    CoreMsInit();
    if (BootStartInitialThreads() != 0) {
      while (BootAllThreadsFinished() == 0) {
        sceDisplayWaitVblankStartCB();
        CorePowerUpdate(CorePowerGet());
        if (CorePowerIsRunning(CorePowerGet()) != 0) {
          CoreMsUpdate(CoreMsGet());
        }
        if (BootIsExitRequested() != 0 && CorePowerIsInitialized() != 0 &&
            CorePowerIsSuspended(CorePowerGet()) != 0) {
          SndGetManager()->stopFlag = 1;
          break;
        }
      }
    }
    while (SndHasManager()) {
      sceDisplayWaitVblankStartCB();
    }
    sceKernelExitGame();
  }
  return 0;
}
