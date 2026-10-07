// bdc 0x0892bff4 UiBakuganRotateModel
#include "bdc.h"

/* One field of `rot * m`: r.x * rot0 + r.y * rot1 + r.z * rot2 + r.w * rot3 lane by lane, summed
   left to right, with rot's columns rot0 = (c, 0, -s, 0), rot1 = (0, 1, 0, 0), rot2 = (s, 0, c, 0),
   rot3 = (0, 0, 0, 1). */
static void UiBakuganRotateField(float *d, const float *r, float c, float s)
{
  float x = r[0];
  float y = r[1];
  float z = r[2];
  float w = r[3];

  d[0] = x * c + y * 0.0f + z * s + w * 0.0f;
  d[1] = x * 0.0f + y * 1.0f + z * 0.0f + w * 0.0f;
  d[2] = x * -s + y * 0.0f + z * c + w * 0.0f;
  d[3] = x * 0.0f + y * 0.0f + z * 0.0f + w * 1.0f;
}

/* Rotates a menu model's root matrix (`model->data->rootMatrix`) about Y by the display angle of
   Bakugan `id`, form `form` (`g_bakuganModelYaw`, 2 floats per id, copied to the stack first):
   rootMatrix = rotY(angle) * rootMatrix in the engine's column-major layout (VFPU `vmmul.q E200,
   E100, E000`: each field of the matrix is transformed by the rotation). */
void UiBakuganRotateModel(void *model, u32 id, u32 form)
{
  float table[44];
  float *m;
  float angle;
  float c;
  float s;
  int i;

  memcpy(table, g_bakuganModelYaw, 0xb0);
  m = ((GfxModel *)model)->data->rootMatrix;
  angle = table[(id & 0xff) * 2 + (form & 0xff)];
  /* vmul.s by S703 (2/π) then vrot in quarter turns: cos/sin of the angle in radians */
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  for (i = 0; i < 4; i++) {
    float r[4];

    r[0] = m[i * 4 + 0];
    r[1] = m[i * 4 + 1];
    r[2] = m[i * 4 + 2];
    r[3] = m[i * 4 + 3];
    UiBakuganRotateField(&m[i * 4], r, c, s);
  }
}
