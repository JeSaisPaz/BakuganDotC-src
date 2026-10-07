// bdc 0x088f6cf0 GameQuestCamPathModeSmooth
#include "bdc.h"

typedef struct SmoothNode {
  char pad[0x4c];
  float scale;
} SmoothNode;

typedef struct SmoothSegment {
  const SmoothNode *from;
  const SmoothNode *to;
  char pad[8];
  float dir[4];
  float t;
} SmoothSegment;

/* Runs `GameQuestCamModeSmooth` toward the attached segment's normal/second node with time step
   `dt` when attached. */

void GameQuestCamPathModeSmooth(float dt, GameQuestCamPathMode *self)

{
  const SmoothSegment *seg = (const SmoothSegment *)self->segment;

  if (seg != (const SmoothSegment *)0x0) {
    GameQuestCamModeSmooth(dt, &self->base, (float *)seg->dir,
                           seg->from->scale * (1.0f - seg->t) + seg->to->scale * seg->t);
  }
}
