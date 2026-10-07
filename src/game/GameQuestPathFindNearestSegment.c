// bdc 0x088f6d44 GameQuestPathFindNearestSegment
#include "bdc.h"

/* Finds the quest camera path segment closest to the followed object of the path camera mode
   `cam` (`GameQuestCamPathMode`; its `GameQuestPathCursor` `base.base.base.followed`, position
   from vtable slot 2). `path` is a `GameQuestCamPtrVec` of `GameQuestCamEntry` nodes; every node
   `from` links to the nodes listed in its `subData` index vector (up to a -1 terminator), and a
   link `from -> to` is a segment when `to->id` or `from->id` equals `node`. Working in the
   horizontal plane (y zeroed), the segment direction is normalised (segments whose length is
   `< 0` are skipped), the target offset from `from` is projected onto it, and the segment is kept
   when the projection `along` satisfies `!(along <= -minDist)` and `along < maxDist + length`; the
   perpendicular distance is the length of the cross product of the unit direction and the offset.
   For the smallest such distance (strictly below the previous best, starting at FLT_MAX) the
   result `GameQuestPathSegment` `*out` is allocated on first use (0x30 bytes from the low heap,
   `dir` zeroed from the VFPU bank's C720) and filled with `from`, `to`, the unit direction,
   `t = along / length` and the distance. Returns 1 when `*out` was written, else 0. Out-of-range
   vector reads go through the inlined accessors that zero and read back
   `g_gameQuestCamNullEntry` / `g_gameQuestCamNullLink`; the static-zero guard
   (`g_staticZeroFGuard`/`g_staticZeroF`) is only written. The w lanes of the unit direction and
   cross product come from the VFPU bank's S713 (0). */

u8 GameQuestPathFindNearestSegment(void *cam, s32 *out, s32 *path, s16 node, float minDist, float maxDist)

{
  GameQuestCamPathMode *self = (GameQuestCamPathMode *)cam;
  GameQuestPathSegment **segOut = (GameQuestPathSegment **)out;
  GameQuestCamPtrVec *vec = (GameQuestCamPtrVec *)path;
  GameQuestPathCursor *cursor;
  const ScePspFVector4 *target;
  GameQuestCamEntry **slot;
  GameQuestCamEntry *from;
  GameQuestCamEntry *to;
  GameQuestPathSegment *seg;
  short *link;
  ScePspFVector4 dir;
  ScePspFVector4 rel;
  ScePspFVector4 perp;
  float len;
  float inv;
  float along;
  float dist;
  float best;
  bool fromLow;
  u8 found;
  int i;
  int j;

  cursor = (GameQuestPathCursor *)self->base.base.base.followed;
  best = 3.4028235e+38f;
  target = ((const ScePspFVector4 *(*)(void *))cursor->vtbl[2].fn)((u8 *)cursor +
                                                                    cursor->vtbl[2].delta);
  found = 0;
  for (i = 0; i < vec->count; i++) {
    if ((i > -1) && (i < vec->count)) {
      slot = &vec->data[i];
    } else {
      memset(&g_gameQuestCamNullEntry, 0, 4);
      slot = &g_gameQuestCamNullEntry;
    }
    from = *slot;
    for (j = 0; j < from->subCount; j++) {
      if ((j > -1) && (j < from->subCount)) {
        link = &from->subData[j];
      } else {
        memset(&g_gameQuestCamNullLink, 0, 2);
        link = &g_gameQuestCamNullLink;
      }
      if (*link == -1) {
        break;
      }
      if ((*link > -1) && (*link < vec->count)) {
        slot = &vec->data[*link];
      } else {
        memset(&g_gameQuestCamNullEntry, 0, 4);
        slot = &g_gameQuestCamNullEntry;
      }
      to = *slot;
      if ((to->id != node) && (from->id != node)) {
        continue;
      }
      /* dir = to->pos - from->pos in the horizontal plane */
      dir.x = to->pos.x - from->pos.x;
      dir.y = to->pos.y - from->pos.y;
      dir.z = to->pos.z - from->pos.z;
      dir.w = to->pos.w;
      if (g_staticZeroFGuard == 0) {
        g_staticZeroFGuard = 1;
        g_staticZeroF = 0.0f;
      }
      dir.y = 0.0f;
      len = __builtin_sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
      if (len < 0.0f) {
        continue;
      }
      inv = 1.0f / len;
      /* w comes from the bank's S713 (0) */
      dir.x = dir.x * inv;
      dir.y = dir.y * inv;
      dir.z = dir.z * inv;
      dir.w = 0.0f;
      /* rel = target - from->pos in the horizontal plane */
      rel.x = target->x - from->pos.x;
      rel.y = target->y - from->pos.y;
      rel.z = target->z - from->pos.z;
      rel.w = target->w;
      if (g_staticZeroFGuard == 0) {
        g_staticZeroFGuard = 1;
        g_staticZeroF = 0.0f;
      }
      rel.y = 0.0f;
      along = dir.x * rel.x + dir.y * rel.y + dir.z * rel.z;
      if (along <= -minDist) {
        continue;
      }
      if (!(along < maxDist + len)) {
        continue;
      }
      /* perp = cross(dir, rel) (w from the bank's S713), dist = |perp| */
      perp.x = dir.y * rel.z - dir.z * rel.y;
      perp.y = dir.z * rel.x - dir.x * rel.z;
      perp.z = dir.x * rel.y - dir.y * rel.x;
      perp.w = 0.0f;
      dist = __builtin_sqrtf(perp.x * perp.x + perp.y * perp.y + perp.z * perp.z);
      if (best <= dist) {
        continue;
      }
      best = dist;
      if (*segOut == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        seg = (GameQuestPathSegment *)MemAlloc(sizeof(GameQuestPathSegment), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (seg != NULL) {
          seg->dir.x = 0.0f;
          seg->dir.y = 0.0f;
          seg->dir.z = 0.0f;
          seg->dir.w = 0.0f;
        }
        *segOut = seg;
      }
      /* the path nodes are GameQuestCamEntry records stored through the segment's node pointers */
      (*segOut)->from = (GameQuestPathNode *)from;
      (*segOut)->to = (GameQuestPathNode *)to;
      (*segOut)->t = along / len;
      (*segOut)->dir = dir;
      (*segOut)->dist = dist;
      found = 1;
    }
  }
  return found;
}
