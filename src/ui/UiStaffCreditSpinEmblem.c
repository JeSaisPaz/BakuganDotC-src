// bdc 0x08945e98 UiStaffCreditSpinEmblem
#include "bdc.h"

/* Rotates two emblem sprites of `UiStaffCredit` (objects at `data+0x18` and
   `data+0x1c`) in opposite directions every frame, multiplying their matrix (`+0x20`) by a Z
   rotation. The angle (radians) is the cached `g_staffCreditEmblemSin` = sin(1 degree), computed
   once on the first call; the first object turns by its negation, the second by it. */

/* m = m * Rz(angle) (vmmul.q E200,E100,E000): each row r becomes
   (c*m[r][0] - s*m[r][1], s*m[r][0] + c*m[r][1], m[r][2], m[r][3]). */
static void StaffCreditRotateMatrix(float *m, float angle)
{
  float c = VfCosQuarter(VfMul(angle, 0.636619747f));
  float s = VfSinQuarter(VfMul(angle, 0.636619747f));
  float x, y, z, w;
  int r;

  for (r = 0; r < 4; r++) {
    x = m[r * 4 + 0];
    y = m[r * 4 + 1];
    z = m[r * 4 + 2];
    w = m[r * 4 + 3];
    m[r * 4 + 0] = VfDot4(x, y, z, w, c, -s, 0.0f, 0.0f);
    m[r * 4 + 1] = VfDot4(x, y, z, w, s, c, 0.0f, 0.0f);
    m[r * 4 + 2] = VfDot4(x, y, z, w, 0.0f, 0.0f, 1.0f, 0.0f);
    m[r * 4 + 3] = VfDot4(x, y, z, w, 0.0f, 0.0f, 0.0f, 1.0f);
  }
}

void UiStaffCreditSpinEmblem(UiScreen *screen)

{
  GfxSprite *obj;

  if (g_staffCreditEmblemInit == 0) {
    g_staffCreditEmblemInit = 1;
    g_staffCreditEmblemSin = VfSinQuarter(0.0174532924f * 0.636619747f); /* 0x3c8efa35: 1 degree */
  }
  obj = ((GfxSprite **)screen->data)[6];
  StaffCreditRotateMatrix(obj->matrix, -g_staffCreditEmblemSin);
  obj = ((GfxSprite **)screen->data)[7];
  StaffCreditRotateMatrix(obj->matrix, g_staffCreditEmblemSin);
}
