// bdc 0x089a4afc UiAbsDiff
#include "bdc.h"

/* Returns `|a - b|`. Used by many UI tweens to compute slide distances. */

float UiAbsDiff(float a, float b)
{
  float d;

  d = a - b;
  if (d < 0.0f) {
    d = -d;
  }
  return d;
}
