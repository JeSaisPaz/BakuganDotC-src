// bdc 0x0880fda8 ScriptOpCameraFocusUnitRef
#include "bdc.h"

/* Camera command like `ScriptOpCameraFocusUnit` but the unit comes from a ref and the x/y/z
   offsets are used unscaled: `cmd` 0 `BtlCameraEnterDefaultMode`, `cmd` 2
   `BtlCameraFocusUnit``(x, y, z, camera, unit, flag != 0, 0, 0)`; other values do nothing.
   Nothing happens unless the camera task exists and the player Bakugan is found; `cmd` 2 also
   needs the ref'd unit to still be in the Bakugan list. Operands: u16 `cmd`, ref, floats x, y, z,
   u16 `flag`, 4 ignored floats. Returns 0. */

int ScriptOpCameraFocusUnitRef(Script *script)

{
  int cmd;
  u32 *ref;
  u32 savedRef;
  float x;
  float y;
  float z;
  u32 flag;
  void *unit;
  void *player;

  cmd = (int)ScriptReadU16(script);
  ref = ScriptReadRef(script, 2);
  savedRef = *ref;
  x = ScriptReadFloat(script);
  y = ScriptReadFloat(script);
  z = ScriptReadFloat(script);
  flag = ScriptReadU16(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  unit = BtlBakuganListFind((BtlBakugan *)PspPtr(savedRef));
  player = BtlGetPlayerBakugan();
  if (BtlCameraTaskExists() != 0 && player != NULL) {
    if (cmd == 0) {
      BtlCameraEnterDefaultMode(BtlGetCameraTask());
    }
    else if (cmd == 2 && unit != NULL) {
      BtlCameraFocusUnit(x, y, z, (BtlMain *)BtlGetCameraTask(), unit, flag != 0, 0, NULL);
    }
  }
  return 0;
}
