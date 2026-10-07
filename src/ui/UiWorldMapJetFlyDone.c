// bdc 0x0899f7ec UiWorldMapJetFlyDone
#include "bdc.h"

/* Advances Marucho's jet entrance (`out` = 0) or exit (`out` != 0) on
   `UiWorldMap` by one frame; in rank mode it does nothing and returns true. Step
   `jetFlyStep`, timer `jetFlyT` (+1/16 per frame, a step ends once it is no longer < 1).
   Coming in: step 0 eases the jet from `jetFlyStart` by `jetFlyDelta` (ease-out, to (−4, −20),
   snapped there at the end) while its scale shrinks from `jetFlyScale` (1.2) by 0.6, oriented for
   flight direction 2 (`UiWorldMapGetJetAngle`); step 1 records the direction-2 angles as the turn
   start and the direction-0 angles as its target with the shortest path per axis
   (`UiWorldMapAngleDelta`); step 2 turns the jet (ease-out, `UiWrapAngle`) at scale 0.6 into
   `jetPitch/Yaw/Roll` and returns true on the frame it ends.
   Going out: step 0 starts a turn from the current angles (plus wobble/offsets) to direction 2, or
   direction 3 when `cancelFlag` is set; step 1 turns (ease-in) at scale 0.6 without reporting the
   end. Not cancelled: step 2 aims at (0, 0) when an area is selected (`areaId` != 0), otherwise
   stays in place and hides the `"afx_130m__CN_BA"` material
   (`GfxModelSetMaterialVisibleByName`); step 3 eases in that move with direction-2 orientation,
   shrinking the scale 0.6 → 0 (area) or growing it 0.6 → 3.0 while fading the model alpha
   (`ambient[3]` = 1 − t²) (no area), and returns true when it ends (alpha 0). Cancelled: step 2
   aims at (−4, −50); step 3 eases there with direction-3 orientation, scale 0.6 → 1.2, and
   returns true when it ends (alpha 0). Every animated step rebuilds the GMO root matrix: X
   rotation, scale of the three axis rows, Y and Z rotations, translation = jet position, w = 1.
   Any other step returns true.
   Angles are radians: the VFPU scales them by the bank's 2/π before its quarter-turn `vrot`, so
   the lifted rotations use `cosf`/`sinf` of the angle directly. */

typedef struct UiWorldMapJetDelta {
  u8 dir;
  float dist;
} UiWorldMapJetDelta;

/* m = Rx(angle): rows (1,0,0,0), (0,c,s,0), (0,-s,c,0), (0,0,0,1). */
static void JetRotX(float *m, float angle)
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

/* Rows 0..2 (all four lanes) scaled by `k` (vscl.q with the stack vector {k, k, k, 0}). */
static void JetScaleAxes(float *m, float k)
{
  int i;

  for (i = 0; i < 12; i++) {
    m[i] = m[i] * k;
  }
}

/* m = m * Ry(angle), Ry rows (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1) (vmmul.q E200,E100,E000):
   each row becomes (c*x + s*z, y, -s*x + c*z, w). */
static void JetRotY(float *m, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);
  float x;
  float z;
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
static void JetRotZ(float *m, float angle)
{
  float c = __builtin_cosf(angle);
  float s = __builtin_sinf(angle);
  float x;
  float y;
  int r;

  for (r = 0; r < 4; r++) {
    x = m[r * 4 + 0];
    y = m[r * 4 + 1];
    m[r * 4 + 0] = x * c + y * -s;
    m[r * 4 + 1] = x * s + y * c;
  }
}

/* Translation row of the root matrix = jet position (all four lanes, lv.q/sv.q), then w = 1. */
static void JetPlaceAtPos(UiWorldMap *map)
{
  float *pos = map->jetModel->pos;
  float *t = &map->jetModel->data->rootMatrix[12];

  t[0] = pos[0];
  t[1] = pos[1];
  t[2] = pos[2];
  t[3] = pos[3];
  map->jetModel->data->rootMatrix[15] = 1.0f;
}

/* Turn target = the angles of flight direction `dir`, shortest distance and direction per axis;
   then the next step. */
static inline void JetSetTurnTarget(UiWorldMap *map, u8 dir)
{
  UiScreen *screen = &map->base;
  UiWorldMapJetDelta r0;
  UiWorldMapJetDelta r1;
  UiWorldMapJetDelta r2;
  UiWorldMapJetDelta d;

  map->jetTurnTo[0] = UiWorldMapGetJetAngle(screen, dir, 0);
  map->jetTurnTo[1] = UiWorldMapGetJetAngle(screen, dir, 1);
  map->jetTurnTo[2] = UiWorldMapGetJetAngle(screen, dir, 2);

  UiWorldMapAngleDelta(map->jetTurnFrom[0], map->jetTurnTo[0], &r0);
  d = r0;
  map->jetTurnDist[0] = d.dist;
  map->jetTurnForward[0] = d.dir;

  UiWorldMapAngleDelta(map->jetTurnFrom[1], map->jetTurnTo[1], &r1);
  d = r1;
  map->jetTurnDist[1] = d.dist;
  map->jetTurnForward[1] = d.dir;

  UiWorldMapAngleDelta(map->jetTurnFrom[2], map->jetTurnTo[2], &r2);
  d = r2;
  map->jetTurnDist[2] = d.dist;
  map->jetTurnForward[2] = d.dir;
  map->jetFlyStep++;
}

/* One axis of the turn: `from` ± `e` × `dist`, wrapped into [0, 6.28). */
static inline float JetTurnAxis(float from, float dist, u8 forward, float e)
{
  if (forward != 0) {
    return UiWrapAngle(from + e * dist);
  }
  return UiWrapAngle(from - e * dist);
}

/* Going out, step 0: turn from the current angles (plus wobble/offsets) to direction `dir`. */
static inline void JetStartTurnOut(UiWorldMap *map, u8 dir)
{
  float pitch = map->jetPitch + map->jetWobble;
  float yaw = map->jetYaw + map->jetYawOffset;
  float roll = map->jetRoll + map->jetRollOffset;

  map->jetFlyT = 0.0f;
  map->jetTurnFrom[0] = pitch;
  map->jetTurnOrigin[0] = pitch;
  map->jetTurnFrom[1] = yaw;
  map->jetTurnOrigin[1] = yaw;
  map->jetTurnFrom[2] = roll;
  map->jetTurnOrigin[2] = roll;
  JetSetTurnTarget(map, dir);
}

/* Going out, step 1: ease-in turn (t²) at scale 0.6; the end snaps to the target, no `true`. */
static inline void JetTurnOut(UiWorldMap *map)
{
  map->jetFlyT += 0.0625f;
  map->jetPitch = JetTurnAxis(map->jetTurnFrom[0], map->jetTurnDist[0], map->jetTurnForward[0],
                              map->jetFlyT * map->jetFlyT);
  map->jetYaw = JetTurnAxis(map->jetTurnFrom[1], map->jetTurnDist[1], map->jetTurnForward[1],
                            map->jetFlyT * map->jetFlyT);
  map->jetRoll = JetTurnAxis(map->jetTurnFrom[2], map->jetTurnDist[2], map->jetTurnForward[2],
                             map->jetFlyT * map->jetFlyT);
  if (!(map->jetFlyT < 1.0f)) {
    map->jetPitch = map->jetTurnTo[0];
    map->jetYaw = map->jetTurnTo[1];
    map->jetRoll = map->jetTurnTo[2];
    map->jetFlyStep++;
  }
  JetRotX(map->jetModel->data->rootMatrix, map->jetPitch);
  JetScaleAxes(map->jetModel->data->rootMatrix, 0.6f);
  JetRotY(map->jetModel->data->rootMatrix, map->jetYaw);
  JetRotZ(map->jetModel->data->rootMatrix, map->jetRoll);
  JetPlaceAtPos(map);
}

bool UiWorldMapJetFlyDone(UiScreen *screen, u8 out)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  bool done = false;
  float *m;
  float a;
  float u;
  float e;
  float s;

  if (UiWorldMapIsRankMode(screen) == 1) {
    return true;
  }

  if (out == 0) {
    switch (map->jetFlyStep) {
    case 0:
      /* fly in: ease-out from jetFlyStart to (-4, -20), scale jetFlyScale -> jetFlyScale - 0.6 */
      map->jetFlyT += 0.0625f;
      u = map->jetFlyT - 1.0f;
      map->jetModel->pos[0] = map->jetFlyStart[0] + (1.0f - u * u) * map->jetFlyDelta[0];
      u = map->jetFlyT - 1.0f;
      map->jetModel->pos[1] = map->jetFlyStart[1] + (1.0f - u * u) * map->jetFlyDelta[1];

      m = map->jetModel->data->rootMatrix;
      a = UiWorldMapGetJetAngle(screen, 2, 0);
      JetRotX(m, a);

      u = map->jetFlyT - 1.0f;
      s = map->jetFlyScale - (1.0f - u * u) * 0.6f;
      JetScaleAxes(map->jetModel->data->rootMatrix, s);

      m = map->jetModel->data->rootMatrix;
      a = UiWorldMapGetJetAngle(screen, 2, 1);
      JetRotY(m, a);

      m = map->jetModel->data->rootMatrix;
      a = UiWorldMapGetJetAngle(screen, 2, 2);
      JetRotZ(m, a);

      if (!(map->jetFlyT < 1.0f)) {
        map->jetModel->pos[0] = -4.0f;
        map->jetModel->pos[1] = -20.0f;
        map->jetFlyStep++;
      }
      JetPlaceAtPos(map);
      return false;

    case 1:
      /* turn from direction 2 to direction 0 */
      map->jetFlyT = 0.0f;
      map->jetTurnFrom[0] = UiWorldMapGetJetAngle(screen, 2, 0);
      map->jetTurnFrom[1] = UiWorldMapGetJetAngle(screen, 2, 1);
      map->jetTurnFrom[2] = UiWorldMapGetJetAngle(screen, 2, 2);
      map->jetTurnOrigin[0] = UiWorldMapGetJetAngle(screen, 2, 0);
      map->jetTurnOrigin[1] = UiWorldMapGetJetAngle(screen, 2, 1);
      map->jetTurnOrigin[2] = UiWorldMapGetJetAngle(screen, 2, 2);
      JetSetTurnTarget(map, 0);
      return false;

    case 2:
      /* ease-out turn (1 - (t - 1)^2) at scale 0.6; true on the frame it ends */
      map->jetFlyT += 0.0625f;
      u = map->jetFlyT - 1.0f;
      e = 1.0f - u * u;
      map->jetPitch = JetTurnAxis(map->jetTurnFrom[0], map->jetTurnDist[0],
                                  map->jetTurnForward[0], e);
      u = map->jetFlyT - 1.0f;
      e = 1.0f - u * u;
      map->jetYaw = JetTurnAxis(map->jetTurnFrom[1], map->jetTurnDist[1],
                                map->jetTurnForward[1], e);
      u = map->jetFlyT - 1.0f;
      e = 1.0f - u * u;
      map->jetRoll = JetTurnAxis(map->jetTurnFrom[2], map->jetTurnDist[2],
                                 map->jetTurnForward[2], e);
      if (!(map->jetFlyT < 1.0f)) {
        map->jetPitch = map->jetTurnTo[0];
        map->jetYaw = map->jetTurnTo[1];
        map->jetRoll = map->jetTurnTo[2];
        map->jetFlyStep++;
        done = true;
      }
      JetRotX(map->jetModel->data->rootMatrix, map->jetPitch);
      JetScaleAxes(map->jetModel->data->rootMatrix, 0.6f);
      JetRotY(map->jetModel->data->rootMatrix, map->jetYaw);
      JetRotZ(map->jetModel->data->rootMatrix, map->jetRoll);
      JetPlaceAtPos(map);
      return done;

    default:
      return true;
    }
  }

  if (map->cancelFlag == 0) {
    switch (map->jetFlyStep) {
    case 0:
      JetStartTurnOut(map, 2);
      return false;

    case 1:
      JetTurnOut(map);
      return false;

    case 2:
      /* aim at (0, 0) when an area is selected, else stay and hide the afterburner material */
      map->jetFlyT = 0.0f;
      map->jetFlyStart[0] = map->jetModel->pos[0];
      map->jetFlyStart[1] = map->jetModel->pos[1];
      map->jetFlyScale = 0.6f;
      if (map->areaId != 0) {
        map->jetFlyTarget[0] = 0.0f;
        map->jetFlyTarget[1] = 0.0f;
      }
      else {
        map->jetFlyTarget[0] = map->jetModel->pos[0];
        map->jetFlyTarget[1] = map->jetModel->pos[1];
        GfxModelSetMaterialVisibleByName(map->jetModel, "afx_130m__CN_BA", false);
      }
      map->jetFlyDelta[0] = map->jetFlyTarget[0] - map->jetFlyStart[0];
      map->jetFlyStep++;
      map->jetFlyDelta[1] = map->jetFlyTarget[1] - map->jetFlyStart[1];
      return false;

    case 3:
      /* ease-in move; shrink to 0 (area) or grow to 3.0 and fade out (no area) */
      map->jetFlyT += 0.0625f;
      map->jetModel->pos[0] = map->jetFlyStart[0] + map->jetFlyT * map->jetFlyT * map->jetFlyDelta[0];
      map->jetModel->pos[1] = map->jetFlyStart[1] + map->jetFlyT * map->jetFlyT * map->jetFlyDelta[1];

      m = map->jetModel->data->rootMatrix;
      a = UiWorldMapGetJetAngle(screen, 2, 0);
      JetRotX(m, a);

      if (map->areaId != 0) {
        s = map->jetFlyScale - map->jetFlyT * map->jetFlyT * 0.6f;
        JetScaleAxes(map->jetModel->data->rootMatrix, s);
      }
      else {
        s = map->jetFlyScale + map->jetFlyT * map->jetFlyT * 2.4f;
        JetScaleAxes(map->jetModel->data->rootMatrix, s);
        map->jetModel->ambient[3] = 1.0f - map->jetFlyT * map->jetFlyT;
      }

      m = map->jetModel->data->rootMatrix;
      a = UiWorldMapGetJetAngle(screen, 2, 1);
      JetRotY(m, a);

      m = map->jetModel->data->rootMatrix;
      a = UiWorldMapGetJetAngle(screen, 2, 2);
      JetRotZ(m, a);

      if (!(map->jetFlyT < 1.0f)) {
        done = true;
        map->jetModel->ambient[3] = 0.0f;
        map->jetFlyStep++;
      }
      JetPlaceAtPos(map);
      return done;

    default:
      return true;
    }
  }

  switch (map->jetFlyStep) {
  case 0:
    JetStartTurnOut(map, 3);
    return false;

  case 1:
    JetTurnOut(map);
    return false;

  case 2:
    /* cancelled: fly back down to (-4, -50) */
    map->jetFlyT = 0.0f;
    map->jetFlyScale = 0.6f;
    map->jetFlyStart[0] = map->jetModel->pos[0];
    map->jetFlyStart[1] = map->jetModel->pos[1];
    map->jetFlyTarget[0] = -4.0f;
    map->jetFlyTarget[1] = -50.0f;
    map->jetFlyDelta[0] = -4.0f - map->jetFlyStart[0];
    map->jetFlyDelta[1] = -50.0f - map->jetFlyStart[1];
    map->jetFlyStep++;
    return false;

  case 3:
    /* ease-in move, scale jetFlyScale -> jetFlyScale + 0.6 */
    map->jetFlyT += 0.0625f;
    map->jetModel->pos[0] = map->jetFlyStart[0] + map->jetFlyT * map->jetFlyT * map->jetFlyDelta[0];
    map->jetModel->pos[1] = map->jetFlyStart[1] + map->jetFlyT * map->jetFlyT * map->jetFlyDelta[1];

    m = map->jetModel->data->rootMatrix;
    a = UiWorldMapGetJetAngle(screen, 3, 0);
    JetRotX(m, a);

    s = map->jetFlyScale + map->jetFlyT * map->jetFlyT * 0.6f;
    JetScaleAxes(map->jetModel->data->rootMatrix, s);

    m = map->jetModel->data->rootMatrix;
    a = UiWorldMapGetJetAngle(screen, 3, 1);
    JetRotY(m, a);

    m = map->jetModel->data->rootMatrix;
    a = UiWorldMapGetJetAngle(screen, 3, 2);
    JetRotZ(m, a);

    if (!(map->jetFlyT < 1.0f)) {
      done = true;
      map->jetModel->ambient[3] = 0.0f;
      map->jetFlyStep++;
    }
    JetPlaceAtPos(map);
    return done;

  default:
    return true;
  }
}
