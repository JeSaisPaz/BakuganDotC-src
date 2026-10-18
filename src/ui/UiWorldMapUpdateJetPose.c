// bdc 0x0899a768 UiWorldMapUpdateJetPose
#include "bdc.h"

/* Rebuilds the root matrix of the jet model of `UiWorldMap`
   (`jetModel->data->rootMatrix`). Outside main-phase step 0x10 it does nothing unless `jetActive`;
   when active it first runs `UiWorldMapJetWobbleStep` and reloads `jetPitch`/`jetYaw`/`jetRoll`
   for direction `spinForward` from `UiWorldMapGetJetAngle`. In both cases the matrix becomes
   rotX(jetPitch + jetWobble), its first three rows scaled by `jetScale`, then multiplied by
   rotY(jetYaw + jetYawOffset) and rotZ(jetRoll + jetRollOffset). The angles are radians (the VFPU
   scales them by the bank's 2/π before its quarter-turn `vrot`). When active,
   `UiWorldMapJetHoverStep` runs last. */

/* m = Rx(angle): rows (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
static void JetPoseSetRotX(float *m, float angle)
{
  float c = VfCosQuarter(VfMul(angle, 0.636619747f));
  float s = VfSinQuarter(VfMul(angle, 0.636619747f));

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
static void JetPoseScaleRows(float *m, float k)
{
  int i;

  for (i = 0; i < 12; i++) {
    m[i] = VfMul(m[i], k);
  }
}

/* m = m * Ry(angle), Ry rows (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1) (vmmul.q E200,E100,E000):
   each row becomes (c*x + s*z, y, -s*x + c*z, w). */
static void JetPoseMulRotY(float *m, float angle)
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
    m[r * 4 + 0] = VfDot4(x, y, z, w, c, 0.0f, s, 0.0f);
    m[r * 4 + 1] = VfDot4(x, y, z, w, 0.0f, 1.0f, 0.0f, 0.0f);
    m[r * 4 + 2] = VfDot4(x, y, z, w, -s, 0.0f, c, 0.0f);
    m[r * 4 + 3] = VfDot4(x, y, z, w, 0.0f, 0.0f, 0.0f, 1.0f);
  }
}

/* m = m * Rz(angle), Rz rows (c,s,0,0), (-s,c,0,0), (0,0,1,0), (0,0,0,1): each row becomes
   (c*x - s*y, s*x + c*y, z, w). */
static void JetPoseMulRotZ(float *m, float angle)
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

void UiWorldMapUpdateJetPose(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  bool animate = screen->phaseStep != 0x10;

  if (animate) {
    if (map->jetActive == 0) {
      return;
    }
    UiWorldMapJetWobbleStep(screen);
    map->jetPitch = UiWorldMapGetJetAngle(screen, map->spinForward, 0);
    map->jetYaw = UiWorldMapGetJetAngle(screen, map->spinForward, 1);
    map->jetRoll = UiWorldMapGetJetAngle(screen, map->spinForward, 2);
  }

  JetPoseSetRotX(map->jetModel->data->rootMatrix, map->jetPitch + map->jetWobble);
  JetPoseScaleRows(map->jetModel->data->rootMatrix, map->jetScale);
  JetPoseMulRotY(map->jetModel->data->rootMatrix, map->jetYaw + map->jetYawOffset);
  JetPoseMulRotZ(map->jetModel->data->rootMatrix, map->jetRoll + map->jetRollOffset);

  if (animate) {
    UiWorldMapJetHoverStep(screen);
  }
}
