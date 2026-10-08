// bdc 0x088f7538 GameQuestCamFollowPath
#include "bdc.h"

/* Places the eye goal (`goal`) of a path camera mode on its attached path segment (`segment`,
   `GameQuestPathSegment`): the point is interpolated between the segment's two node positions by
   the parameter `t` and, when the target distance `targetDist` exceeds the segment distance
   `dist`, pulled back along the segment direction `dir` by `sqrt(targetDist^2 - dist^2)` (`pow`/`sqrt`
   in double); sets `pullFactor = 1.5` and calls `GameQuestCamRefreshLookAt` when the segment is
   NULL or the byte at `from + 0x61` is set. Does nothing in state 2 (`stateIndex`).
   The VFPU temporaries start from the bank zero C720 and the `w` lane of every scaled vector is the
   bank zero S713, so `goal.w` ends as 0. */

void GameQuestCamFollowPath(GameQuestCamPathMode *self)
{
    ScePspFVector4 point;
    GameQuestPathSegment *seg;
    float s;
    float t;
    float d2;
    float n2;
    float pull;

    if (self->base.stateIndex == 2) {
        return;
    }
    self->pullFactor = 1.5f;
    seg = (GameQuestPathSegment *)self->segment;
    /* point = from * (1 - t) + to * t (vscl.t: lane w is S713 = 0) */
    s = 1.0f - seg->t;
    point.x = seg->from->pos.x * s;
    point.y = seg->from->pos.y * s;
    point.z = seg->from->pos.z * s;
    t = seg->t;
    point.x = point.x + seg->to->pos.x * t;
    point.y = point.y + seg->to->pos.y * t;
    point.z = point.z + seg->to->pos.z * t;
    point.w = 0.0f;
    if (seg->dist < self->targetDist) {
        d2 = (float)pow((double)self->targetDist, 2.0);
        n2 = (float)pow((double)seg->dist, 2.0);
        pull = (float)sqrt((double)(d2 - n2));
        s = -pull;
        point.x = point.x + seg->dir.x * s;
        point.y = point.y + seg->dir.y * s;
        point.z = point.z + seg->dir.z * s;
    }
    self->base.base.base.goal = point;
    if (self->segment == NULL ||
        ((GameQuestPathSegment *)self->segment)->from->collide != 0) {
        GameQuestCamRefreshLookAt();
    }
}
