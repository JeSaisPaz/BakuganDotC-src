// bdc 0x0899c24c UiWorldMapStartJetFlyIn
#include "bdc.h"

/* Prepares Marucho's jet animation of `UiWorldMap`: always clears the 0x60-byte
   fly record (`jetFly`, `+0x22b0`); in rank mode, or going out (`out` != 0), that is all. Coming in
   (`out` = 0) it also resets `jetFlyT`, sets `jetFlyScale` to 1.2, gives the jet model full alpha
   (`ambient[3]` = 1) and places it at (−4, −50), records that start position, the target (−4, −20)
   and their difference (`jetFlyDelta`), then rebuilds the model's GMO root matrix: X/Y/Z rotations
   for flight direction 2 (`UiWorldMapGetJetAngle`, radians; the VFPU scales them by the bank's
   2/π before its quarter-turn `vrot`) with the 3×3 part scaled by 1.2, translation = the jet
   position, w = 1. Advanced by `UiWorldMapJetFlyDone`. */

/* m = Rx(angle): rows (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
static void JetFlySetRotX(float *m, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);

  m[0] = 1.0f;
  m[1] = 0.0f;
  m[2] = 0.0f;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = c;
  m[6] = s;
  m[7] = 0.0f;
  m[8] = 0.0f;
  m[9] = -s;
  m[10] = c;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
}

/* Rows 0..2 (all four lanes) scaled by `k` (vscl.q). */
static void JetFlyScaleRows(float *m, float k)
{
  int i;

  for (i = 0; i < 12; i++) {
    m[i] = m[i] * k;
  }
}

/* m = m * Ry(angle), Ry rows (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1) (vmmul.q E200,E100,E000):
   each row becomes (c*x + s*z, y, -s*x + c*z, w). */
static void JetFlyMulRotY(float *m, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);
  float x, z;
  int r;

  for (r = 0; r < 4; r++) {
    x = m[r * 4 + 0];
    z = m[r * 4 + 2];
    m[r * 4 + 0] = x * c + z * s;
    m[r * 4 + 2] = x * -s + z * c;
  }
}

/* m = m * Rz(angle), Rz rows (c,s,0,0), (-s,c,0,0), (0,0,1,0), (0,0,0,1): each row becomes
   (c*x - s*y, s*x + c*y, z, w). */
static void JetFlyMulRotZ(float *m, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);
  float x, y;
  int r;

  for (r = 0; r < 4; r++) {
    x = m[r * 4 + 0];
    y = m[r * 4 + 1];
    m[r * 4 + 0] = x * c + y * -s;
    m[r * 4 + 1] = x * s + y * c;
  }
}

void UiWorldMapStartJetFlyIn(UiScreen *screen, u8 out)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  float *m;
  float *pos;
  float angle;

  if (UiWorldMapIsRankMode(screen) == 1) {
    memset(map->jetFly, 0, 0x60);
    return;
  }
  if (out != 0) {
    memset(map->jetFly, 0, 0x60);
    return;
  }

  memset(map->jetFly, 0, 0x60);
  map->jetFlyT = 0.0f;
  map->jetFlyScale = 1.2f;
  map->jetModel->ambient[3] = 1.0f;
  map->jetModel->pos[0] = -4.0f;
  map->jetModel->pos[1] = -50.0f;
  map->jetFlyStart[0] = map->jetModel->pos[0];
  map->jetFlyStart[1] = map->jetModel->pos[1];
  map->jetFlyTarget[0] = -4.0f;
  map->jetFlyTarget[1] = -20.0f;
  map->jetFlyDelta[0] = -4.0f - map->jetFlyStart[0];
  map->jetFlyDelta[1] = -20.0f - map->jetFlyStart[1];

  /* X rotation (matrix pointer read before the call, as in the asm) */
  m = map->jetModel->data->rootMatrix;
  angle = UiWorldMapGetJetAngle(screen, 2, 0);
  JetFlySetRotX(m, angle);

  /* scale the three axis rows */
  JetFlyScaleRows(map->jetModel->data->rootMatrix, map->jetFlyScale);

  /* Y rotation */
  m = map->jetModel->data->rootMatrix;
  angle = UiWorldMapGetJetAngle(screen, 2, 1);
  JetFlyMulRotY(m, angle);

  /* Z rotation */
  m = map->jetModel->data->rootMatrix;
  angle = UiWorldMapGetJetAngle(screen, 2, 2);
  JetFlyMulRotZ(m, angle);

  /* translation row = the jet position (all four lanes), then w = 1 */
  m = &map->jetModel->data->rootMatrix[12];
  pos = map->jetModel->pos;
  m[0] = pos[0];
  m[1] = pos[1];
  m[2] = pos[2];
  m[3] = pos[3];
  map->jetModel->data->rootMatrix[15] = 1.0f;
}
