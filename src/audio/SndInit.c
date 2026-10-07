// bdc 0x089c5740 SndInit
#include "bdc.h"

/* Boots the sound engine: allocates and zeroes the 0x21c-byte audio-settings block
   `g_soundAudioSettings` (heap bottom), allocates the 0x8be8-byte `SndManager` (heap bottom),
   constructs it with `SndManagerInit` and stores it in `g_soundManager` (NULL if the
   allocation failed), then creates the two streaming players with `SndBgmPlayerCreate(0)` and
   `SndBgmPlayerCreate(1)` (each starts its own `MyThread-Sound-Bgm*` thread). */

void SndInit(void)
{
  bool prevFromLow;
  SndAudioSettings *settings;
  SndManager *mgr;
  SndManager *result;

  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  settings = MemAlloc(sizeof(SndAudioSettings), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  g_soundAudioSettings = settings;
  memset(settings, 0, sizeof(SndAudioSettings));

  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mgr = MemAlloc(sizeof(SndManager), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  result = NULL;
  if (mgr != NULL) {
    SndManagerInit(mgr);
    result = mgr;
  }
  g_soundManager = result;

  SndBgmPlayerCreate(0);
  SndBgmPlayerCreate(1);
}
