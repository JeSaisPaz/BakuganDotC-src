// bdc 0x089bc050 BootMainThread
#include "bdc.h"

/* Entry of the main game thread (slot 0 of `g_threadTable`, "MyThread_Main"): enables VFPU use
   for the thread, masks the FPU exception traps (clears FCSR enable bits 7..11), waits for two
   start-up conditions (`IoDataMngExists`, `IoDecodeMngExists`), sets up the VFPU constant
   registers the rest of the game reads as live-ins (S700 = 2*pi, S701 = 255, S702 = pi,
   S703 = 2/pi, S713 = 0, C720 = 0, C730 = identity row), initialises the subsystems and runs the
   per-frame game loop, finishing each frame with `BootEndOfFrame`(`g_padState`) after
   waiting out `BootIsExitRequested`. The script/sys-util step is skipped on the frame after one
   where `CorePowerCanRunFrame` said no. Tears down and returns 0 when the frame check
   `ScriptMngUpdate` fails. */

int BootMainThread(void)
{
  bool suspended;

  sceKernelChangeCurrentThreadAttr(0, 0x4000);
  PlatformFpuSetControl(PlatformFpuGetControl() & ~0xf80u);
  while (IoDataMngExists() == 0) {
    sceDisplayWaitVblankStartCB();
  }
  while (IoDecodeMngExists() == 0) {
    sceDisplayWaitVblankStartCB();
  }
  /* Here the asm loads the VFPU constant bank (matrix 7: S700 = 2*pi, S701 = 255, S702 = pi,
     S703 = 2/pi, S713 = 0, C720 = 0, C730 = (0, 0, 0, 1)). Every reader lifts those registers to
     literals, so the set-up has no C effect. */
  PadCreate();
  GfxInit();
  ScriptMngCreate();
  CoreTaskManagerCreate();
  SysUtilCreate();
  GfxInitBootResources();
  CoreRandLoadVfpuState();
  CoreRandResetSeed();
  suspended = false;
  for (;;) {
    if (!suspended) {
      if (ScriptMngUpdate(ScriptMngGet()) == 0) {
        SysUtilDestroy();
        CoreTaskManagerShutdown();
        ScriptMngDestroy();
        PadDestroy();
        return 0;
      }
      if (SysUtilIsInit()) {
        SysUtilPoll(SysUtilGetCell());
      }
    }
    if (NetPlayHasManager()) {
      NetPlayUpdate((NetPlay *)NetPlayGetManager());
    }
    /* asm loads a0 = g_taskManager here; the callee ignores it */
    CoreTaskManagerUpdate();
    if (NetPlayHasManager()) {
      NetPlayLateUpdate((NetPlay *)NetPlayGetManager());
    }
    suspended = false;
    if (CorePowerIsInitialized() != 0) {
      if (CorePowerCanRunFrame(CorePowerGet()) == 0) {
        suspended = true;
      }
    }
    /* asm loads a0 = g_taskManager here; the callee ignores it */
    CoreTaskManagerDraw();
    GfxEndFrame();
    while (BootIsExitRequested() != 0) {
      sceDisplayWaitVblankStartCB();
    }
    BootEndOfFrame(g_padState);
  }
}
