// bdc 0x08a0006c ScriptOpPlayMovie
#include "bdc.h"

/* Script opcode 0x40 (group 0x40): plays a movie through the movie task 0x2738 (10040,
   `GfxMovieTask`). Operands u32 `wait`, u16 `movieId`. With `wait == 0`: when the task does not
   exist it is created (`CoreTaskCreate``(0x2738, 100)`, only while a decoder output channel
   exists, `SndDecOutExists``(-1)` ≠ 0; otherwise it just keeps returning 2) and the opcode
   returns 2; once it exists `GfxMovieRequestPlay``(movieId)` is retried until accepted (then 0).
   With `wait != 0`: while the task exists it returns 2 and removes the task
   (`CoreTaskRemove``(task, 1)`) as soon as `GfxMovieIsDone`; with no task it returns 0. */

int ScriptOpPlayMovie(Script *script)
{
  u32 wait = ScriptReadU32(script);
  u32 movieId = ScriptReadU16(script);
  CoreTask *task = (CoreTask *)CoreTaskFind(0x2738);
  int ret;

  if (wait == 0) {
    ret = 2;
    if (task == NULL) {
      if (SndDecOutExists(-1) != 0) {
        CoreTaskCreate(0x2738, 100);
      }
    } else if (GfxMovieRequestPlay(movieId)) {
      ret = 0;
    }
  } else {
    ret = 0;
    if (task != NULL) {
      ret = 2;
      if (GfxMovieIsDone()) {
        CoreTaskRemove(task, true);
      }
    }
  }
  return ret;
}
