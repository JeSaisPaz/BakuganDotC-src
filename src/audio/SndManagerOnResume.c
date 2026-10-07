// bdc 0x089c6bbc SndManagerOnResume
#include "bdc.h"

/* Power-resume callback of the sound manager (registered by `SndManagerStateInitAudio` with
   `CorePowerAddResumeCallback`): if `volatileBlock` is NULL it allocates two 0x1a0000-byte
   (1.625 MiB) buffers from the PSP-2000 volatile memory (`CorePowerVolatileAlloc`, source tags
   `"c:/bullets/bkn2pspsys/src/pspsys/sys/Sound/COSoundSndp.cpp"` lines `0x8f9` and `0x8fa`),
   stores the first in `volatileBlock`, hands buffer 0 to BGM player 0 and buffer 1 to BGM player 1
   (`SndBgmPlayerSetBuffer`) and asks each existing player to resume
   (`SndBgmPlayerRequestResume`). If both allocations succeeded it sets `buffersReady = 1` and
   `suspended = 0`. Returns nothing; does nothing when `volatileBlock` is already set. */

void SndManagerOnResume(void)
{
  SndManager *mgr;
  void *buf0;
  void *buf1;

  mgr = SndGetManager();
  if (mgr->volatileBlock != NULL) {
    return;
  }
  buf0 = CorePowerVolatileAlloc(CorePowerGet(), 0x1a0000,
                                "c:/bullets/bkn2pspsys/src/pspsys/sys/Sound/COSoundSndp.cpp", 0x8f9);
  buf1 = CorePowerVolatileAlloc(CorePowerGet(), 0x1a0000,
                                "c:/bullets/bkn2pspsys/src/pspsys/sys/Sound/COSoundSndp.cpp", 0x8fa);
  mgr->volatileBlock = buf0;
  if (SndBgmPlayerExists(0)) {
    SndBgmPlayerSetBuffer(SndBgmPlayerGet(0), buf0);
    SndBgmPlayerRequestResume(SndBgmPlayerGet(0));
  }
  if (SndBgmPlayerExists(1)) {
    SndBgmPlayerSetBuffer(SndBgmPlayerGet(1), buf1);
    SndBgmPlayerRequestResume(SndBgmPlayerGet(1));
  }
  if (buf0 != NULL && buf1 != NULL) {
    mgr->buffersReady = 1;
    mgr->suspended = 0;
  }
}
