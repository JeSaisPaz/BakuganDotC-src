// bdc 0x08810f5c ScriptOpChangeBgm
#include "bdc.h"

/* Script opcode: queues a background-music change. Mode 0 plays BGM track `param` (-1 = 0x11):
   through the battle main task (`BtlMainQueueBgm`, stop then play) when it exists, else
   `SndBgmQueueStop` followed by `SndBgmQueuePlay`; mode 1 only queues a stop (fade-out). Always
   returns 0. */

int ScriptOpChangeBgm(Script *script)
{
  u32 mode;
  u32 bgmId;

  mode = ScriptReadU32(script);
  bgmId = ScriptReadU32(script);
  if (bgmId == 0xffffffff) {
    bgmId = 0x11;
  }
  if ((int)mode < 1) {
    if (-1 < (int)mode) {
      if (BtlCameraTaskExists() == 0) {
        SndBgmQueueStop(1.0f, 0);
        SndBgmQueuePlay(0, bgmId, 1, 0);
      } else {
        BtlMainQueueBgm((BtlMain *)BtlGetCameraTask(), bgmId, 1.5f);
      }
    }
  } else if ((int)mode < 2) {
    SndBgmQueueStop(1.0f, 0);
  }
  return 0;
}
