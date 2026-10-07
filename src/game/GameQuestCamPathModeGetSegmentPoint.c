// bdc 0x088f6b38 GameQuestCamPathModeGetSegmentPoint
#include "bdc.h"

/* Slot 7 of the path camera mode: writes the point at parameter `t` (`seg+0x20`) on the attached
   segment `+0x104` (lerp `a * (1 - t) + b * t` of its two nodes' positions `+0x20`, `w` = 0), or
   the `zero` vector of `g_gameQuestCamPathModeAxisConsts` when not attached. */

typedef struct CamPathNode {
  char pad[0x20];
  ScePspFVector4 pos;
} CamPathNode;

typedef struct CamPathSegment {
  CamPathNode *a;
  CamPathNode *b;
  char pad[0x18];
  float t;
} CamPathSegment;

void GameQuestCamPathModeGetSegmentPoint(GameQuestCamPathMode *self, float *out)

{
  ScePspFVector4 a;
  ScePspFVector4 b;
  CamPathSegment *seg;
  float w;

  seg = (CamPathSegment *)self->segment;
  if (seg == (CamPathSegment *)0x0) {
    out[0] = g_gameQuestCamPathModeAxisConsts.zero.x;
    out[1] = g_gameQuestCamPathModeAxisConsts.zero.y;
    out[2] = g_gameQuestCamPathModeAxisConsts.zero.z;
    out[3] = g_gameQuestCamPathModeAxisConsts.zero.w;
    return;
  }
  a = seg->a->pos;
  w = 1.0f - seg->t;
  a.x = a.x * w;
  a.y = a.y * w;
  a.z = a.z * w;
  b = seg->b->pos;
  w = seg->t;
  b.x = b.x * w;
  b.y = b.y * w;
  b.z = b.z * w;
  out[0] = a.x + b.x;
  out[1] = a.y + b.y;
  out[2] = a.z + b.z;
  out[3] = 0.0f; /* lane S713 of the vscl.t result: the bank zero */
}
