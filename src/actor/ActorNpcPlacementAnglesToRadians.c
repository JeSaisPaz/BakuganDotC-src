// bdc 0x088e58d4 ActorNpcPlacementAnglesToRadians
#include "bdc.h"

/* Byte-identical copy of `ActorPlacementAnglesToRadians` used by the NPC code
   (`ActorNpcPhase00ApplyPlacement`): converts three s16 angles (1/65536 turns) to radians as
   `pi/2 - a`, each wrapped to (-pi, pi], into a vec4 with w = 0. */

void ActorNpcPlacementAnglesToRadians(float *out, const s16 *angles)

{
  float rad[3];
  float a;
  s32 i;

  for (i = 0; i < 3; i++) {
    a = -((float)angles[i] * 6.2831855f * 1.5259022e-05f - 1.5707964f);
    if (!(a <= 3.1415927f)) {
      a = a - 6.2831855f;
    }
    else if (a <= -3.1415927f) {
      a = a + 6.2831855f;
    }
    rad[i] = a;
  }
  out[0] = rad[0];
  out[1] = rad[1];
  out[2] = rad[2];
  out[3] = 0.0f;
}
