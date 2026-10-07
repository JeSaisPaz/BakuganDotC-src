// bdc 0x08a002f0 ScriptOpBgmPlayer
#include "bdc.h"

/* Script opcode 0x45 (group 0x40): commands one of the two streamed-music players
   (`SndBgmPlayer`, slot `0`/`1` of `g_soundBgmPlayers`, checked with `SndBgmPlayerExists`,
   fetched with `SndBgmPlayerGet`). Operands u32 `slot, cmd, a, b`. Returns 2 (retry next frame)
   if `slot` does not exist, else 0 unless a command below says 2. cmd 0 stops with a 0.5 s fade
   (`SndBgmPlayerStopF`, 2 if refused); 1 waits until stopped (`SndBgmPlayerIsStopped`); 2
   preloads stream file `a` (`SndStreamFilePreload`); 3 waits until the preload is loaded
   (`SndStreamFileIsLoaded``(-1)`); 4 plays the loaded track (`loop = b != 0`,
   `SndBgmPlayerPlay`, 2 if refused); 5 plays track `a` from scratch (`SndBgmPlayerPlayTrack`,
   loop `b != 0`, result ignored); 6 releases the preloaded file (`SndStreamFileRelease`, 2 while
   loading); 7 loads track `a` without playing (`SndBgmPlayerLoadTrack`, 2 if refused); 8 plays
   track `a` on slot 0 without loop, pushing the previous track if `b != 0` (2 if refused); 9 returns
   2 while slot 0 is playing with a push-resume pending (`SndBgmPlayerIsResumePending`). Other
   `cmd` values return 0. */

int ScriptOpBgmPlayer(Script *script)

{
  int result;
  u32 slot;
  u32 cmd;
  u32 a;
  u32 b;

  result = 0;
  slot = ScriptReadU32(script);
  cmd = ScriptReadU32(script);
  a = ScriptReadU32(script);
  b = ScriptReadU32(script);
  if (!SndBgmPlayerExists(slot)) {
    return 2;
  }
  switch (cmd) {
  case 0:
    if (SndBgmPlayerStopF(0.5f, SndBgmPlayerGet(slot), 0) == 0) {
      result = 2;
    }
    break;
  case 1:
    if (SndBgmPlayerIsStopped(SndBgmPlayerGet(slot)) == 0) {
      result = 2;
    }
    break;
  case 2:
    SndStreamFilePreload(a);
    break;
  case 3:
    result = 2;
    if (SndStreamFileIsLoaded(-1)) {
      result = 0;
    }
    break;
  case 4:
    if (SndBgmPlayerPlay(SndBgmPlayerGet(slot), b != 0) == 0) {
      result = 2;
    }
    break;
  case 5:
    SndBgmPlayerPlayTrack(SndBgmPlayerGet(slot), a, b != 0, 0);
    break;
  case 6:
    if (!SndStreamFileRelease(a)) {
      result = 2;
    }
    break;
  case 7:
    if (!SndBgmPlayerLoadTrack(SndBgmPlayerGet(slot), a)) {
      result = 2;
    }
    break;
  case 8:
    if (SndBgmPlayerPlayTrack(SndBgmPlayerGet(0), a, 0, b != 0) == 0) {
      result = 2;
    }
    break;
  case 9:
    if (SndBgmPlayerIsStopped(SndBgmPlayerGet(0)) == 0 &&
        SndBgmPlayerIsResumePending(SndBgmPlayerGet(0)) != 0) {
      result = 2;
    }
    break;
  }
  return result;
}
