// bdc 0x089c0658 SndListenerDistSq
#include "bdc.h"

/* Squared distance from the listener position `listener->pos` to the point `p` (three floats).
   Returns `g_floatPosInf` (+infinity) instead when any squared axis difference equals
   `g_floatPosInf` or `g_floatNegInf` (an infinite coordinate). */

float SndListenerDistSq(SndListener *listener, const float *p)
{
  float *pos = listener->pos;
  float dx = p[0] - pos[0];
  float dy = p[1] - pos[1];
  float dz = p[2] - pos[2];
  float result;
  float x2 = dx * dx;
  float y2 = dy * dy;
  float z2 = dz * dz;

  result = g_floatPosInf;
  if (x2 != g_floatPosInf && x2 != g_floatNegInf && y2 != g_floatPosInf && y2 != g_floatNegInf &&
      z2 != g_floatPosInf && z2 != g_floatNegInf) {
    result = x2 + y2 + z2;
  }
  return result;
}
