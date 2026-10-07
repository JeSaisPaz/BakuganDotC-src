// bdc 0x0899852c UiWorldMapApplyGlobeRotation
#include "bdc.h"

/* Rebuilds the matrices of both globe nodes of `UiWorldMap` (`globeNodeMatrix[0]`,
   `globeNodeMatrix[1]`) from the current rotation: each is set to an X rotation by `globePitch`,
   then multiplied by a Y rotation by `globeYaw` and a Z rotation by `globeRoll` (matrix = M * R
   per step, rows of 4 floats). The angles are radians (the VFPU scales them by the bank's 2/π
   before its quarter-turn `vrot`). */

/* m = Rx(angle): rows (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
static void GlobeSetRotX(float *m, float angle)
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

/* m = m * Ry(angle), Ry rows (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1) (vmmul.q E200,E100,E000):
   each row r becomes (c*x + s*z, y, -s*x + c*z, w). */
static void GlobeMulRotY(float *m, float angle)
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

/* m = m * Rz(angle), Rz rows (c,s,0,0), (-s,c,0,0), (0,0,1,0), (0,0,0,1): each row r becomes
   (c*x - s*y, s*x + c*y, z, w). */
static void GlobeMulRotZ(float *m, float angle)
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

void UiWorldMapApplyGlobeRotation(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;

  GlobeSetRotX(map->globeNodeMatrix[0], map->globePitch);
  GlobeSetRotX(map->globeNodeMatrix[1], map->globePitch);
  GlobeMulRotY(map->globeNodeMatrix[0], map->globeYaw);
  GlobeMulRotY(map->globeNodeMatrix[1], map->globeYaw);
  GlobeMulRotZ(map->globeNodeMatrix[0], map->globeRoll);
  GlobeMulRotZ(map->globeNodeMatrix[1], map->globeRoll);
}
