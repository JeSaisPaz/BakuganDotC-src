// bdc 0x08974eb8 UiCollectionMenuUpdateItemBoxTween
#include "bdc.h"

/* Advances one frame of the open/close tween of the item-box model (`itemBox`) of the collection
   top menu (task 311, `maybe_UiScreen311Ctor`): `spinT += 1 / slideDuration`. Opening
   (`out` false) uses the ease-out e = 1 - (spinT - 1)^2: `itemBox->ambient[3] = spinScale + e`,
   rootMatrix = rotY(spinAngle - e/2), `pos[0] = itemBoxSlideStart - e * itemBoxSlideDelta`; once
   `!(spinT < 1)` it snaps to `ambient[3] = 1`, rotY(0), `pos[0] = itemBoxSlideEnd`. Closing
   (`out` true) uses the ease-in q = spinT^2: `ambient[3] = spinScale - q`, rotY(spinAngle + q/2),
   `pos[0] = itemBoxSlideStart + q * itemBoxSlideDelta` (no snap). Both then multiply rootMatrix by
   a fixed X tilt rotX(0.5 rad), scale rows 0..2 (all four lanes) by 1.2, copy `pos` into row 3 and
   set its w to 1. Returns true when `!(spinT < 1)`. */

/* rootMatrix = VFPU vrot Y rotation of angle * 2/pi (S703) quarter turns: rows (c, 0, -s, 0),
   (0, 1, 0, 0), (s, 0, c, 0), (0, 0, 0, 1). */
static void ItemBoxSetRotY(float *m, float angle)
{
  float c;
  float s;

  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  m[0] = c;
  m[1] = 0.0f;
  m[2] = -s;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 1.0f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s;
  m[9] = 0.0f;
  m[10] = c;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
}

/* Shared tail. Each row *= X rotation of 0.5 rad (0.5 * S703 quarter turns): R0 = (1, 0, 0, 0),
   R1 = (0, c, s, 0), R2 = (0, -s, c, 0), R3 = (0, 0, 0, 1) (vmmul.q E200, E100, E000); rows 0..2
   *= 1.2 (vscl.q); row 3 = pos; row 3 w = 1. */
static void ItemBoxFinish(UiCollectionMenu *self)
{
  float *m;
  float *pos;
  float c;
  float s;
  float y;
  float z;
  int i;

  m = self->itemBox->data->rootMatrix;
  c = __builtin_cosf(0.5f);
  s = __builtin_sinf(0.5f);
  for (i = 0; i < 4; i++) {
    y = m[i * 4 + 1];
    z = m[i * 4 + 2];
    m[i * 4 + 1] = y * c - z * s;
    m[i * 4 + 2] = y * s + z * c;
  }
  m = self->itemBox->data->rootMatrix;
  for (i = 0; i < 12; i++) {
    m[i] = m[i] * 1.20000005f; /* 0x3f99999a */
  }
  pos = self->itemBox->pos;
  m = self->itemBox->data->rootMatrix;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = pos[3];
  self->itemBox->data->rootMatrix[15] = 1.0f;
}

bool UiCollectionMenuUpdateItemBoxTween(UiCollectionMenu *self, bool out)
{
  float *m;
  float a;
  float t;
  float start;
  bool done;

  done = 0;
  if (!out) {
    t = self->spinT + 1.0f / self->slideDuration;
    self->spinT = t;
    t = t - 1.0f;
    self->itemBox->ambient[3] = self->spinScale + (1.0f - t * t);
    m = self->itemBox->data->rootMatrix;
    a = self->spinAngle;
    t = self->spinT - 1.0f;
    ItemBoxSetRotY(m, a - (1.0f - t * t) * 0.5f);
    start = (float)self->itemBoxSlideStart;
    t = self->spinT - 1.0f;
    self->itemBox->pos[0] = start - (1.0f - t * t) * (float)self->itemBoxSlideDelta;
    if (!(self->spinT < 1.0f)) {
      self->itemBox->ambient[3] = 1.0f;
      ItemBoxSetRotY(self->itemBox->data->rootMatrix, 0.0f);
      self->itemBox->pos[0] = (float)self->itemBoxSlideEnd;
      done = 1;
    }
  } else {
    t = self->spinT + 1.0f / self->slideDuration;
    self->spinT = t;
    self->itemBox->ambient[3] = self->spinScale - t * t;
    m = self->itemBox->data->rootMatrix;
    a = self->spinAngle;
    t = self->spinT;
    ItemBoxSetRotY(m, a + t * t * 0.5f);
    start = (float)self->itemBoxSlideStart;
    t = self->spinT;
    self->itemBox->pos[0] = start + t * t * (float)self->itemBoxSlideDelta;
    if (!(self->spinT < 1.0f)) {
      done = 1;
    }
  }
  ItemBoxFinish(self);
  return done;
}
