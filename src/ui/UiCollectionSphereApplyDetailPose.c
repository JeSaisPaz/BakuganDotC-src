// bdc 0x0897e480 UiCollectionSphereApplyDetailPose
#include "bdc.h"

/* One field of `rotX * m`: r.x * rot0 + r.y * rot1 + r.z * rot2 + r.w * rot3 lane by lane, summed
   left to right, with rot's columns rot0 = (1, 0, 0, 0), rot1 = (0, c, s, 0), rot2 = (0, -s, c, 0),
   rot3 = (0, 0, 0, 1). */
static void UiCollectionSphereRotXField(float *d, const float *r, float c, float s)
{
  float x = r[0];
  float y = r[1];
  float z = r[2];
  float w = r[3];

  d[0] = VfDot4(x, y, z, w, 1.0f, 0.0f, 0.0f, 0.0f);
  d[1] = VfDot4(x, y, z, w, 0.0f, c, -s, 0.0f);
  d[2] = VfDot4(x, y, z, w, 0.0f, s, c, 0.0f);
  d[3] = VfDot4(x, y, z, w, 0.0f, 0.0f, 0.0f, 1.0f);
}

/* Applies the motion-view pose of the detail model (`models[detailCell]`) of
   `UiCollectionSphere`: root matrix = the Y rotation by `yaw`, then each
   field transformed by the X rotation by `tilt` (VFPU `vrot`/`vmmul`), then rows 0-2 scaled
   uniformly by `UiCollectionSphereGetDetailScale` × the second value from
   `UiCollectionSphereGetDetailOffset` × `zoom`; that scale is stored in `modelScale[detailCell]`. */
void UiCollectionSphereApplyDetailPose(UiCollectionSphere *self)
{
  float offset[2];
  float scale;
  float *m;
  float c;
  float s;
  int i;

  /* vmul.s by S703 (2/π) then vrot in quarter turns: cos/sin of the angle in radians */
  m = self->models[self->detailCell]->data->rootMatrix;
  c = VfCosQuarter(VfMul(self->yaw, 0.636619747f));
  s = VfSinQuarter(VfMul(self->yaw, 0.636619747f));
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

  m = self->models[self->detailCell]->data->rootMatrix;
  c = VfCosQuarter(VfMul(self->tilt, 0.636619747f));
  s = VfSinQuarter(VfMul(self->tilt, 0.636619747f));
  for (i = 0; i < 4; i++) {
    float r[4];

    r[0] = m[i * 4 + 0];
    r[1] = m[i * 4 + 1];
    r[2] = m[i * 4 + 2];
    r[3] = m[i * 4 + 3];
    UiCollectionSphereRotXField(&m[i * 4], r, c, s);
  }

  UiCollectionSphereGetDetailOffset(offset, &self->base);
  scale = UiCollectionSphereGetDetailScale(self, self->detailCell) * offset[1] * self->zoom;
  self->modelScale[self->detailCell] = scale;
  m = self->models[self->detailCell]->data->rootMatrix;
  for (i = 0; i < 3; i++) {
    m[i * 4 + 0] = VfMul(m[i * 4 + 0], scale);
    m[i * 4 + 1] = VfMul(m[i * 4 + 1], scale);
    m[i * 4 + 2] = VfMul(m[i * 4 + 2], scale);
    m[i * 4 + 3] = VfMul(m[i * 4 + 3], scale);
  }
}
