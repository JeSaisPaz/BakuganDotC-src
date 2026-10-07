// bdc 0x088ea5e0 GameEventPropSetRot
#include "bdc.h"

/* Sets the prop model's rotation `+0x30` from three s16 angles (1/65535 turns), each wrapped
   once into (-pi, pi]; w is 0. Does nothing when the prop has no model. */

typedef struct {
  u8 pad[0x30];
  float rot[4];
} GameEventPropRotModel;

static inline float GameEventPropAngleToRad(s16 angle)
{
  float a = (float)angle * 6.2831855f * 1.5259022e-05f;

  if (!(a <= 3.1415927f)) {
    a = a - 6.2831855f;
  } else if (a <= -3.1415927f) {
    a = a + 6.2831855f;
  }
  return a;
}

void GameEventPropSetRot(void *prop, const s16 *angles)

{
  GameEventProp *p = (GameEventProp *)prop;
  float *dst;

  if (p->model == NULL) {
    return;
  }
  dst = ((GameEventPropRotModel *)p->model)->rot;
  dst[0] = GameEventPropAngleToRad(angles[0]);
  dst[1] = GameEventPropAngleToRad(angles[1]);
  dst[2] = GameEventPropAngleToRad(angles[2]);
  dst[3] = 0.0f;
}
