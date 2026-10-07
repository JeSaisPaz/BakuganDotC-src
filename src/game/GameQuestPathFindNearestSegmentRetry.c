// bdc 0x088f71ec GameQuestPathFindNearestSegmentRetry
#include "bdc.h"

/* Looks up the nearest path segment (`GameQuestPathFindNearestSegment`) with the camera's minimum
   distance (`*(cam+4) + 0x44`) and margin 0, retrying with the margin equal to the distance when
   nothing was found. */

void GameQuestPathFindNearestSegmentRetry(GameQuestCamPathMode *self, s32 *outSeg, s32 *outT, s16 pathId)

{
  float minDist;
  
  minDist = ((self->base).base.base.ctrl)->radius;
  if (GameQuestPathFindNearestSegment(self,outSeg,outT,pathId,minDist,0.0f) == 0) {
    GameQuestPathFindNearestSegment(self,outSeg,outT,pathId,minDist,minDist);
  }
  return;
}

