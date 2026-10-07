// bdc 0x088115d8 ScriptOpControlMovie
#include "bdc.h"

/* Script opcode driving the PSMF movie player task 0x2738. Operands: u32 `mode`, u16 `movieId`.
   Returns 0 at once when bit 0x20 of the global script bitset is set or the battle main task
   (task 100) exists with `rematch` set. Mode 0 (start/update): without the task, creates it
   (`CoreTaskCreate``(0x2738, 100)`) when a sound decoder is live and returns 2 (wait); with the
   task, returns 0 once `GfxMovieRequestPlay``(movieId)` succeeds, else 2. Other modes (stop):
   returns 0 when the task is gone; otherwise returns 2 and, once `GfxMovieIsDone`, removes and
   destroys the task (`CoreTaskRemove`). */

int ScriptOpControlMovie(Script *script)

{
  u32 mode;
  u32 movieId;
  CoreTask *task;
  int result;

  mode = ScriptReadU32(script);
  movieId = ScriptReadU16(script);
  task = (CoreTask *)CoreTaskFind(0x2738);
  if (CoreBitsetTest(0x20, g_scriptGlobalBits)) {
    return 0;
  }
  if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->rematch != 0) {
    return 0;
  }
  if (mode == 0) {
    result = 2;
    if (task == NULL) {
      if (SndDecOutExists(-1) != 0) {
        CoreTaskCreate(0x2738, 100);
      }
    }
    else if (GfxMovieRequestPlay(movieId)) {
      result = 0;
    }
  }
  else {
    result = 0;
    if (task != NULL) {
      result = 2;
      if (GfxMovieIsDone()) {
        CoreTaskRemove(task, true);
      }
    }
  }
  return result;
}
