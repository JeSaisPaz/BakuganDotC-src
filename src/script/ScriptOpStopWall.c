// bdc 0x088109f4 ScriptOpStopWall
#include "bdc.h"

/* Script opcode that drives the stop walls (`g_stopWallList`, `StopWallCreate`). Operands: u32
   `mode`, floats `on`, `id`, `x`, `y`. Mode 0: `on <= 0` dismisses the wall tagged `id`
   (`StopWallDismiss`), otherwise creates it (`StopWallCreate`). Mode 1: `on <= 0` returns the
   camera (task 100, `BtlGetCameraTask`) to its default mode (`BtlCameraEnterDefaultMode`);
   otherwise, if any wall exists, points the battle camera at the wall with that id
   (`StopWallFocusCamera``(wall, x, y, 40.0)`); when no wall matches the search stops on the last
   wall of the list, which is then used. Other modes do nothing. Returns 0. */

int ScriptOpStopWall(Script *script)
{
  s32 mode;
  float on;
  float idF;
  float x;
  float y;
  s32 id;
  StopWall *wall;
  StopWall *next;

  mode = (s32)ScriptReadU32(script);
  on = ScriptReadFloat(script);
  idF = ScriptReadFloat(script);
  x = ScriptReadFloat(script);
  y = ScriptReadFloat(script);
  id = (s32)idF;
  wall = NULL;
  if (g_stopWallList != NULL) {
    wall = (StopWall *)g_stopWallList->head;
    if (wall != NULL) {
      next = (StopWall *)wall->base.next;
      while (wall->id != id && next != NULL) {
        wall = next;
        next = (StopWall *)wall->base.next;
      }
    }
  }
  if (mode == 0) {
    if (on <= 0.0f) {
      StopWallDismiss(id);
    } else {
      StopWallCreate(id);
    }
  } else if (mode == 1) {
    if (on <= 0.0f) {
      BtlCameraEnterDefaultMode(BtlGetCameraTask());
    } else if (wall != NULL) {
      StopWallFocusCamera(wall, x, y, 40.0f);
    }
  }
  return 0;
}
