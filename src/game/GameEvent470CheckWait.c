// bdc 0x088f2138 GameEvent470CheckWait
#include "bdc.h"

/* Vtable slot 12 of the task-470 event (custom waits, from `GameEventCheckWaitDone`): wait types
   5/6 end when task 0x177 is gone; type 7 ends when task 0x176 is gone, then skips to label opcode
   0xa6 or 0xa7 depending on script global 3 (`g_scriptGlobalVars[3]` = 0/1) and resets the camera.
   Other types end at once; type 7 with global 3 outside 0..1 keeps waiting. */

s32 GameEvent470CheckWait(GameEvent470 *self)
{
  u8 type = self->base.waitType;
  s32 done = 0;
  s32 v;

  if (type < 7) {
    if (type < 5) {
      done = 1;
    } else if (CoreTaskExists(0x177) == 0) {
      done = 1;
    }
  } else if (type < 8) {
    if (CoreTaskExists(0x176) == 0) {
      v = g_scriptGlobalVars[3];
      if (v < 1) {
        if (v >= 0) {
          GameEventSkipToOpcode(&self->base, 0xa6);
          GameFieldCameraReset((GameFieldCamera *)g_gfxActiveCamera, 0, 0);
          done = 1;
        }
      } else if (v < 2) {
        GameEventSkipToOpcode(&self->base, 0xa7);
        GameFieldCameraReset((GameFieldCamera *)g_gfxActiveCamera, 0, 0);
        done = 1;
      }
    }
  } else {
    done = 1;
  }
  return done;
}
