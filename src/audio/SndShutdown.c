// bdc 0x089c582c SndShutdown
#include "bdc.h"

/* Shuts the sound engine down. If `g_soundManager` exists: state 1 is moved straight to 6
   (stopping); states 2, 3 and 5 first run `SndSsTerm` and then move to 6; any other state
   (<= 0, 4, >= 6) is left as is. It then loops `sceKernelDelayThreadCB(100)` + `SndManagerStep`
   until the state reaches 0, destroys the manager (`SndManagerDestroyObj``(mgr, 3)`) and clears
   `g_soundManager`. Afterwards, unconditionally, it releases the 32 CODataMng request handles at
   `dataHandles` and the one at `dataHandle8c` of `g_soundAudioSettings` (dereferencing it without
   a NULL check) and, if the block exists, frees it under `MemLock` and clears the pointer. */

void SndShutdown(void)
{
  s32 i;

  if (g_soundManager != NULL) {
    switch (g_soundManager->state) {
    case 2:
    case 3:
    case 5:
      SndSsTerm();
      /* fallthrough */
    case 1:
      g_soundManager->state = 6;
      break;
    default:
      break;
    }
    while (g_soundManager->state != 0) {
      sceKernelDelayThreadCB(100);
      SndManagerStep(g_soundManager);
    }
    if (g_soundManager != NULL) {
      SndManagerDestroyObj(g_soundManager, 3);
      g_soundManager = NULL;
    }
  }
  for (i = 0; i < 32; i++) {
    IoDataMngRelease(IoGetDataMng(), &g_soundAudioSettings->dataHandles[i],
                     g_soundAudioSettings->dataHandles[i]);
  }
  IoDataMngRelease(IoGetDataMng(), &g_soundAudioSettings->dataHandle8c,
                   g_soundAudioSettings->dataHandle8c);
  if (g_soundAudioSettings != NULL) {
    MemLock();
    MemFree(g_soundAudioSettings, NULL, 0);
    MemUnlock();
    g_soundAudioSettings = NULL;
  }
}
