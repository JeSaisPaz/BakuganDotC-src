// bdc 0x0880f960 ScriptOpCameraFocusUnit
#include "bdc.h"

/* Camera command on the battle camera/main task (id 100,
   `BtlCameraTaskExists`/`BtlGetCameraTask`) for the `idx`-th battle unit
   (`BtlGetNthCrystal`). Operands: u16 `cmd`, u16 `idx`, floats x, y, z, u16 `flag`, 4 ignored
   floats. `cmd` 0: `BtlCameraEnterDefaultMode`; `cmd` 2: `BtlCameraFocusUnit``(x*s, y*s, z,
   camera, unit, flag != 0, 0, 0)` where `s` is the unit's scale (`+0x40`) clamped to 0.8..1.2; on
   stage 10 (script global 1) it also sets camera `+0x414 = pi/2` and `+0x41c = -200.0`. Requires
   the camera task and the player unit. Returns 0. */

int ScriptOpCameraFocusUnit(Script *script)
{
  int cmd;
  short idx;
  float x;
  float y;
  float z;
  u32 flag;
  BtlBakugan *unit;
  void *player;
  BtlMain *cameraTask;
  float scale;

  cmd = (int)ScriptReadU16(script);
  idx = (short)ScriptReadU16(script);
  x = ScriptReadFloat(script);
  y = ScriptReadFloat(script);
  z = ScriptReadFloat(script);
  flag = ScriptReadU16(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  unit = (BtlBakugan *)BtlBakuganListFind((BtlBakugan *)BtlGetNthCrystal(idx));
  player = BtlGetPlayerBakugan();
  if (BtlCameraTaskExists() != 0 && player != NULL) {
    if (cmd == 0) {
      BtlCameraEnterDefaultMode(BtlGetCameraTask());
    }
    else if (cmd == 2 && unit != NULL) {
      scale = unit->base.scale[0];
      if (scale < 0.8f) {
        scale = 0.8f;
      }
      else if (!(scale <= 1.2f)) {
        scale = 1.2f;
      }
      cameraTask = (BtlMain *)BtlGetCameraTask();
      BtlCameraFocusUnit(x * scale, y * scale, z, cameraTask, unit, flag != 0, 0, NULL);
      cameraTask = (BtlMain *)BtlGetCameraTask();
      if (g_scriptGlobalVars[1] == 10) {
        cameraTask->camera.followOffset3f4 = 1.5707964f;
        cameraTask->camera.followDistanceOffset = -200.0f;
      }
    }
  }
  return 0;
}
