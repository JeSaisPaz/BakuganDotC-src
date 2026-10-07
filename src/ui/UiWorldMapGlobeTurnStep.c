// bdc 0x08999754 UiWorldMapGlobeTurnStep
#include "bdc.h"

/* Advances the globe turn of `UiWorldMap` started by `UiWorldMapStartGlobeTurn`:
   saves the current rotation quad in `prevRotation`, advances `turnT` by 1/`turnDuration` and eases
   each axis (`globePitch`, `globeYaw`, `globeRoll`) from `turnFrom` by ±(1 − (t − 1)²)·`turnDist`
   (sign from `turnForward`, wrapped by `UiWrapAngle`). Returns 0 while `turnT` < 1; otherwise
   snaps the three axes to `turnTo` and returns 1. */

int UiWorldMapGlobeTurnStep(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  float d;
  float angle;

  /* copies the rotation quad (globePitch..globeRotW) to prevRotation (lv.q/sv.q) */
  map->prevRotation[0] = map->globePitch;
  map->prevRotation[1] = map->globeYaw;
  map->prevRotation[2] = map->globeRoll;
  map->prevRotation[3] = map->globeRotW;
  map->turnT = map->turnT + 1.0f / map->turnDuration;

  if (map->turnForward[0] != 0) {
    d = map->turnT - 1.0f;
    angle = UiWrapAngle(map->turnFrom[0] + (1.0f - d * d) * map->turnDist[0]);
  } else {
    d = map->turnT - 1.0f;
    angle = UiWrapAngle(map->turnFrom[0] - (1.0f - d * d) * map->turnDist[0]);
  }
  map->globePitch = angle;

  if (map->turnForward[1] != 0) {
    d = map->turnT - 1.0f;
    angle = UiWrapAngle(map->turnFrom[1] + (1.0f - d * d) * map->turnDist[1]);
  } else {
    d = map->turnT - 1.0f;
    angle = UiWrapAngle(map->turnFrom[1] - (1.0f - d * d) * map->turnDist[1]);
  }
  map->globeYaw = angle;

  if (map->turnForward[2] != 0) {
    d = map->turnT - 1.0f;
    angle = UiWrapAngle(map->turnFrom[2] + (1.0f - d * d) * map->turnDist[2]);
  } else {
    d = map->turnT - 1.0f;
    angle = UiWrapAngle(map->turnFrom[2] - (1.0f - d * d) * map->turnDist[2]);
  }
  map->globeRoll = angle;

  if (map->turnT < 1.0f) {
    return 0;
  }
  map->globePitch = map->turnTo[0];
  map->globeYaw = map->turnTo[1];
  map->globeRoll = map->turnTo[2];
  return 1;
}
