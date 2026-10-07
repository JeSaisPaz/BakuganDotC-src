// bdc 0x089a4d88 UiWrapAngle
#include "bdc.h"

/* Wraps an angle into `[0, 6.28)` by adding or subtracting 6.28 once. */

float UiWrapAngle(float angle)

{
  if (angle < 6.28f) {
    if (angle < 0.0f) {
      angle = angle + 6.28f;
    }
    return angle;
  }
  return angle - 6.28f;
}
