// bdc 0x0897b8b0 UiCollectionSphereSpinPreviewModel
#include "bdc.h"

/* Per-frame idle spin of a preview model of the sphere (Bakugan figure) collection screen
   (`UiCollectionSphere`, task 312); does nothing while `glowOn` is clear
   or the chosen model is NULL. The model is `models[6]` (with `cellActive[6]`, `modelScale[6]`)
   when `UiCollectionSphereGetPageKind` of the current page is 1 or 2, else (kind 0 or 0xff) the
   cursor cell's `models[cursor]`. Multiplies its `rootMatrix` rows by a Y rotation of 0.04 rad
   (0.02 rad when the cell's `cellActive` flag is set), re-orthonormalises the 3x3 part
   (row2 = row0 x row1, row0 = row1 x row2, rows 0..2 normalised, w of rows 0 and 2 zeroed) and
   scales rows 0..2 (all four lanes) by the cell's `modelScale`. */

/* VFPU vrot of angle * 2/pi (S703) in quarter turns: rows R0 = (c, 0, -s, 0), R1 = (0, 1, 0, 0),
   R2 = (s, 0, c, 0), R3 = (0, 0, 0, 1); each matrix row becomes row * R (vmmul.q E200, E100, E000). */
static void SpherePreviewRotate(float *m, float angle)
{
  float c;
  float s;
  float x;
  float y;
  float z;
  float w;
  int i;

  c = VfCosQuarter(VfMul(angle, 0.636619747f));
  s = VfSinQuarter(VfMul(angle, 0.636619747f));
  for (i = 0; i < 4; i++) {
    x = m[i * 4 + 0];
    y = m[i * 4 + 1];
    z = m[i * 4 + 2];
    w = m[i * 4 + 3];
    m[i * 4 + 0] = VfDot4(x, y, z, w, c, 0.0f, s, 0.0f);
    m[i * 4 + 1] = VfDot4(x, y, z, w, 0.0f, 1.0f, 0.0f, 0.0f);
    m[i * 4 + 2] = VfDot4(x, y, z, w, -s, 0.0f, c, 0.0f);
    m[i * 4 + 3] = VfDot4(x, y, z, w, 0.0f, 0.0f, 0.0f, 1.0f);
  }
}

/* Re-orthonormalise (vcrsp.t / vdot.t / vrsq.t / vscl.t); row 3 is stored back unchanged. */
static void SpherePreviewOrthonormalize(float *m)
{
  float r0x, r0y, r0z;
  float r1x, r1y, r1z, r1w;
  float r2x, r2y, r2z;
  float inv0, inv1, inv2;

  r1x = m[4];
  r1y = m[5];
  r1z = m[6];
  r1w = m[7];
  r2x = VfDot2(m[1], m[2], r1z, -r1y);
  r2y = VfDot2(m[2], m[0], r1x, -r1z);
  r2z = VfDot2(m[0], m[1], r1y, -r1x);
  r0x = VfDot2(r1y, r1z, r2z, -r2y);
  r0y = VfDot2(r1z, r1x, r2x, -r2z);
  r0z = VfDot2(r1x, r1y, r2y, -r2x);
  inv0 = VfRsq(VfDot3(r0x, r0y, r0z, r0x, r0y, r0z));
  inv1 = VfRsq(VfDot3(r1x, r1y, r1z, r1x, r1y, r1z));
  inv2 = VfRsq(VfDot3(r2x, r2y, r2z, r2x, r2y, r2z));
  m[0] = VfMul(r0x, inv0);
  m[1] = VfMul(r0y, inv0);
  m[2] = VfMul(r0z, inv0);
  m[3] = 0.0f;
  m[4] = VfMul(r1x, inv1);
  m[5] = VfMul(r1y, inv1);
  m[6] = VfMul(r1z, inv1);
  m[7] = r1w;
  m[8] = VfMul(r2x, inv2);
  m[9] = VfMul(r2y, inv2);
  m[10] = VfMul(r2z, inv2);
  m[11] = 0.0f;
}

/* Scale rows 0..2 (all four lanes) by `scale` (vscl.q). */
static void SpherePreviewScale(float *m, float scale)
{
  int i;

  for (i = 0; i < 12; i++) {
    m[i] = VfMul(m[i], scale);
  }
}

void UiCollectionSphereSpinPreviewModel(UiCollectionSphere *self)
{
  u8 kind;

  kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
  if (self->glowOn == 0) {
    return;
  }
  if (kind != 0xff && kind != 0) {
    if (self->models[6] == NULL) {
      return;
    }
    if (self->cellActive[6] == 0) {
      SpherePreviewRotate(self->models[6]->data->rootMatrix, 0.04f); /* 0x3d23d70a */
    } else {
      SpherePreviewRotate(self->models[6]->data->rootMatrix, 0.02f); /* 0x3ca3d70a */
    }
    SpherePreviewOrthonormalize(self->models[6]->data->rootMatrix);
    SpherePreviewScale(self->models[6]->data->rootMatrix, self->modelScale[6]);
    return;
  }
  if (self->models[self->cursor] == NULL) {
    return;
  }
  if (self->cellActive[self->cursor] == 0) {
    SpherePreviewRotate(self->models[self->cursor]->data->rootMatrix, 0.04f); /* 0x3d23d70a */
  } else {
    SpherePreviewRotate(self->models[self->cursor]->data->rootMatrix, 0.02f); /* 0x3ca3d70a */
  }
  SpherePreviewOrthonormalize(self->models[self->cursor]->data->rootMatrix);
  SpherePreviewScale(self->models[self->cursor]->data->rootMatrix, self->modelScale[self->cursor]);
}
