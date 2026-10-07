// bdc 0x08945e98 UiStaffCreditSpinEmblem
#include "bdc.h"

/* Rotates two emblem sprites of `UiStaffCredit` (objects at `data+0x18` and
   `data+0x1c`) in opposite directions every frame, multiplying their matrix (`+0x20`) by a Z
   rotation. The angle (radians) is the cached `g_staffCreditEmblemSin` = sin(1 degree), computed
   once on the first call; the first object turns by its negation, the second by it. */

typedef struct StaffCreditEmblemObj {
  u8 unk00[0x20];
  float matrix[16]; /* 0x20 */
} StaffCreditEmblemObj;

typedef struct StaffCreditEmblemData {
  u8 unk00[0x18];
  StaffCreditEmblemObj *a; /* 0x18 */
  StaffCreditEmblemObj *b; /* 0x1c */
} StaffCreditEmblemData;

/* m = m * Rz(angle) (vmmul.q E200,E100,E000): each row r becomes
   (c*m[r][0] - s*m[r][1], s*m[r][0] + c*m[r][1], m[r][2], m[r][3]). */
static void StaffCreditRotateMatrix(float *m, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);
  float x, y;
  int r;

  for (r = 0; r < 4; r++) {
    x = m[r * 4 + 0];
    y = m[r * 4 + 1];
    m[r * 4 + 0] = c * x - s * y;
    m[r * 4 + 1] = s * x + c * y;
  }
}

void UiStaffCreditSpinEmblem(UiScreen *screen)

{
  StaffCreditEmblemObj *obj;

  if (g_staffCreditEmblemInit == 0) {
    g_staffCreditEmblemInit = 1;
    g_staffCreditEmblemSin = __builtin_sinf(0.0174532924f); /* 0x3c8efa35: 1 degree */
  }
  obj = ((StaffCreditEmblemData *)screen->data)->a;
  StaffCreditRotateMatrix(obj->matrix, -g_staffCreditEmblemSin);
  obj = ((StaffCreditEmblemData *)screen->data)->b;
  StaffCreditRotateMatrix(obj->matrix, g_staffCreditEmblemSin);
}
