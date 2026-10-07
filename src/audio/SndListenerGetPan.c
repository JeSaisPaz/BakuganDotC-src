// bdc 0x089c0738 SndListenerGetPan
#include "bdc.h"

/* Stereo pan for a sound at `p`: `-sin(atan2f(target.z - p.z, target.x - p.x) + listener->heading)`,
   i.e. -1..1 depending on which side of the listener's heading the source lies. The VFPU sine takes
   quarter turns and the asm scales by the bank constant 2/pi (S703), so the two cancel. */

float SndListenerGetPan(SndListener *listener, const float *p)
{
  float angle = atan2f(listener->target[2] - p[2], listener->target[0] - p[0]);

  return -__builtin_sinf(angle + listener->heading);
}
