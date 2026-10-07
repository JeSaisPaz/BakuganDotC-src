// bdc 0x088b3ea8 StopWallFocusCamera
#include "bdc.h"

/* If the battle camera task exists (`BtlCameraTaskExists`) and bit 5 of the global bitset
   `g_scriptGlobalBits` is set (`CoreBitsetTest`), raises the stop wall by 300.0 (`wall + 0x34 +=
   300.0`), points the battle camera at it with `BtlCameraFocusUnit``(x, y, z, cameraTask,
   wall + 0x30, 0, 2, wall + 0x40)`, sets the byte `wall + 0x88 = 1` and the counter `wall + 0x80 = 100`.
    */

void StopWallFocusCamera(StopWall *self, float x, float y, float z)

{
  void *cameraTask;

  if (BtlCameraTaskExists() != 0 && CoreBitsetTest(5, g_scriptGlobalBits)) {
    self->centre[1] = self->centre[1] + 300.0f;
    cameraTask = BtlGetCameraTask();
    BtlCameraFocusUnit(x, y, z, cameraTask, self->centre, 0, 2, self->rot);
    self->shown = 1;
    self->state = 100;
  }
}
