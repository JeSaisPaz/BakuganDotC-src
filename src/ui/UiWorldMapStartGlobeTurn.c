// bdc 0x089983c8 UiWorldMapStartGlobeTurn
#include "bdc.h"

/* Starts turning the `UiWorldMap` globe towards the selected area: clears the
   0x60-byte turn record `turnFrom..` (+0x21f0) and, when `UiWorldMapGetGlobeMotion` returns 0,
   stores the duration `turnDuration`, the current rotation (`globePitch..`) as `turnFrom` and
   `turnOrigin`, the area's target rotation from `g_uiWorldMapAreaRotations` (indexed by
   `areaGroup[areaId]`) as `turnTo` and, per axis, the shortest direction/distance
   (`UiWorldMapAngleDelta` → `turnForward[]`, `turnDist[]`); the idle spin direction
   `spinForward` takes the yaw direction. */

typedef struct UiWorldMapAngleDeltaResult {
  u8 dir;
  float dist;
} UiWorldMapAngleDeltaResult;

void UiWorldMapStartGlobeTurn(float duration, UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  float rotations[10][3];
  UiWorldMapAngleDeltaResult r0;
  UiWorldMapAngleDeltaResult r1;
  UiWorldMapAngleDeltaResult r2;
  UiWorldMapAngleDeltaResult d;
  u32 group;

  memcpy(rotations, g_uiWorldMapAreaRotations, sizeof(rotations));
  memset(map->turnFrom, 0, 0x60);
  if (UiWorldMapGetGlobeMotion(screen) == 0) {
    group = map->areaGroup[map->areaId];
    map->turnDuration = duration;
    map->turnFrom[0] = map->globePitch;
    map->turnFrom[1] = map->globeYaw;
    map->turnFrom[2] = map->globeRoll;
    map->turnOrigin[0] = map->globePitch;
    map->turnOrigin[1] = map->globeYaw;
    map->turnOrigin[2] = map->globeRoll;
    map->turnTo[0] = rotations[group][0];
    map->turnTo[1] = rotations[group][1];
    map->turnTo[2] = rotations[group][2];

    UiWorldMapAngleDelta(map->turnFrom[0], map->turnTo[0], &r0);
    d = r0;
    map->turnDist[0] = d.dist;
    map->turnForward[0] = d.dir;

    UiWorldMapAngleDelta(map->turnFrom[1], map->turnTo[1], &r1);
    d = r1;
    map->turnDist[1] = d.dist;
    map->turnForward[1] = d.dir;

    UiWorldMapAngleDelta(map->turnFrom[2], map->turnTo[2], &r2);
    d = r2;
    map->turnDist[2] = d.dist;
    map->turnForward[2] = d.dir;
    map->spinForward = map->turnForward[1];
  }
  return;
}
