// bdc 0x088f6cf0 GameQuestCamPathModeSmooth
#include "bdc.h"

/* Runs `GameQuestCamModeSmooth` along the attached segment's direction with time step `dt` when
   attached; the look-ahead scale is the segment end nodes' `f4c` lerped by the segment's `t`. */

void GameQuestCamPathModeSmooth(float dt, GameQuestCamPathMode *self)

{
  const GameQuestPathSegment *seg = (const GameQuestPathSegment *)self->segment;

  if (seg != NULL) {
    GameQuestCamModeSmooth(dt, &self->base, (float *)&seg->dir.x,
                           seg->from->f4c * (1.0f - seg->t) + seg->to->f4c * seg->t);
  }
}
