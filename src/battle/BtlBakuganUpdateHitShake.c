// bdc 0x08865184 BtlBakuganUpdateHitShake
#include "bdc.h"

/* Per-frame model shake after a hit: while `hitShake` is positive, optionally rebuilds the model
   matrix (`BtlBakuganUpdateModelMatrix`) when `rebuildMatrix` is nonzero, then lowers `hitShake`
   by `hitShakeDecay`. If it is still positive, the root matrix translation x/z (elements 12/14)
   move by `hitShakeDir{X,Z} × amplitude × hitShake` and the amplitude's sign flips; otherwise
   `hitShake` is set to 0. */
void BtlBakuganUpdateHitShake(BtlBakugan *self, char rebuildMatrix)
{
  float shake;
  float *root;

  if (self->hitShake <= 0.0f) {
    return;
  }
  if (rebuildMatrix != 0) {
    BtlBakuganUpdateModelMatrix(self);
  }
  shake = self->hitShake - self->hitShakeDecay;
  self->hitShake = shake;
  if (shake <= 0.0f) {
    self->hitShake = 0.0f;
    return;
  }
  /* The binary also clamps the amplitude with vmin.s/vmax.s against live-in VFPU registers
     S733/S713, but the clamped value is overwritten before use, so it has no effect. */
  root = self->base.data->rootMatrix;
  root[12] = root[12] + self->hitShakeDirX * self->hitShakeAmplitude * shake;
  root[14] = root[14] + self->hitShakeDirZ * self->hitShakeAmplitude * self->hitShake;
  self->hitShakeAmplitude = -self->hitShakeAmplitude;
}
