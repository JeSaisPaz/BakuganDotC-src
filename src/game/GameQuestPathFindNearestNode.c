// bdc 0x088f9490 GameQuestPathFindNearestNode
#include "bdc.h"

/* Picks the quest path node (`GameQuestPathNode`, `.ptlb` table parsed by
   `GameQuestParsePathTable` into `g_questPathSet`) whose oriented box contains the followed
   object (`GameQuestPathCursor`, the quest camera controller's `nodeCursor`) and stores its
   index in `obj->node`. The object's position comes from its vtable slot 2 (`+0x10`, the read of
   `g_questPathSet` happens before that call). For every node of `g_questPathSet->cur` (out-of-range
   reads go through the inlined accessor that zeroes `g_gameQuestPathNullNode` and reads it back),
   the offset `pos - node->pos` (xyz; w = the position's w) is rotated into the node's frame with
   the inverse of the matrix built from the quaternion `node->rot`, and the node is a hit when
   `boundsMin < local < boundsMax` on x, y and z (`local < max` and `!(local <= min)`). Among the
   hits the one with the smallest `kind` below 5 wins (first one on ties); with no hit the stored
   index is -1. Each compare is preceded by the inlined static-zero guards (`g_staticZeroFGuard`,
   `g_staticZeroF2Guard`), which are only written. The VFPU block is lifted: the rotation is the
   3x3 part of `A * B^T` (A, B the quaternion product matrices, as `vmmul.q E000, E200, E100`), and
   `vtfm4` applies its transpose with the translation `-(R * 0)` kept for NaN/inf inputs. */

void GameQuestPathFindNearestNode(s16 *obj)

{
  GameQuestPathCursor *cursor = (GameQuestPathCursor *)obj;
  GameQuestPathSet *set;
  const ScePspFVector4 *pos;
  GameQuestPathNodeVec *vec;
  GameQuestPathNode **slot;
  GameQuestPathNode *node;
  ScePspFVector4 q;
  float a[4][4];
  float b[4][4];
  float m[3][3];
  float tr[3];
  float local[4];
  float out[4];
  int c;
  int r;
  int i;
  int inside;
  int bestKind;
  s16 idx;
  s16 best;

  set = g_questPathSet;
  pos = ((const ScePspFVector4 *(*)(void *))cursor->vtbl[2].fn)((u8 *)cursor +
                                                                 cursor->vtbl[2].delta);
  best = -1;
  bestKind = 5;
  idx = 0;
  for (i = 0; i < set->cur->count; i++) {
    vec = set->cur;
    if ((i > -1) && (i < vec->count)) {
      slot = &vec->data[i];
    }
    else {
      memset(&g_gameQuestPathNullNode, 0, 4);
      slot = &g_gameQuestPathNullNode;
    }
    node = *slot;
    /* local = (pos - node->pos) on xyz, w = the position's w */
    local[0] = pos->x - node->pos.x;
    local[1] = pos->y - node->pos.y;
    local[2] = pos->z - node->pos.z;
    local[3] = pos->w;
    /* m = A * B^T with A, B the left/right product matrices of the quaternion (columns) */
    q = node->rot;
    a[0][0] = q.w;  a[0][1] = q.z;  a[0][2] = -q.y; a[0][3] = -q.x;
    a[1][0] = -q.z; a[1][1] = q.w;  a[1][2] = q.x;  a[1][3] = -q.y;
    a[2][0] = q.y;  a[2][1] = -q.x; a[2][2] = q.w;  a[2][3] = -q.z;
    a[3][0] = q.x;  a[3][1] = q.y;  a[3][2] = q.z;  a[3][3] = q.w;
    b[0][0] = q.w;  b[0][1] = q.z;  b[0][2] = -q.y; b[0][3] = q.x;
    b[1][0] = -q.z; b[1][1] = q.w;  b[1][2] = q.x;  b[1][3] = q.y;
    b[2][0] = q.y;  b[2][1] = -q.x; b[2][2] = q.w;  b[2][3] = q.z;
    b[3][0] = -q.x; b[3][1] = -q.y; b[3][2] = -q.z; b[3][3] = q.w;
    for (c = 0; c < 3; c++) {
      for (r = 0; r < 3; r++) {
        m[c][r] = a[0][r] * b[0][c] + a[1][r] * b[1][c] + a[2][r] * b[2][c] + a[3][r] * b[3][c];
      }
    }
    /* inverse: transposed rotation, translation -(R * 0) */
    for (r = 0; r < 3; r++) {
      tr[r] = m[r][0] * 0.0f + m[r][1] * 0.0f + m[r][2] * 0.0f;
    }
    for (r = 0; r < 3; r++) {
      out[r] = local[0] * m[r][0] + local[1] * m[r][1] + local[2] * m[r][2] + local[3] * -tr[r];
    }
    out[3] = local[0] * 0.0f + local[1] * 0.0f + local[2] * 0.0f + local[3] * 1.0f;
    local[0] = out[0];
    local[1] = out[1];
    local[2] = out[2];
    local[3] = out[3];
    inside = 0;
    if (g_staticZeroFGuard == 0) {
      g_staticZeroFGuard = 1;
      g_staticZeroF = 0.0f;
    }
    if (g_staticZeroF2Guard == 0) {
      g_staticZeroF2Guard = 1;
      g_staticZeroF2 = 0.0f;
    }
    if (local[0] < node->boundsMax.x) {
      if (g_staticZeroFGuard == 0) {
        g_staticZeroFGuard = 1;
        g_staticZeroF = 0.0f;
      }
      if (g_staticZeroF2Guard == 0) {
        g_staticZeroF2Guard = 1;
        g_staticZeroF2 = 0.0f;
      }
      if (!(local[0] <= node->boundsMin.x)) {
        if (g_staticZeroFGuard == 0) {
          g_staticZeroFGuard = 1;
          g_staticZeroF = 0.0f;
        }
        if (g_staticZeroF2Guard == 0) {
          g_staticZeroF2Guard = 1;
          g_staticZeroF2 = 0.0f;
        }
        if (local[1] < node->boundsMax.y) {
          if (g_staticZeroFGuard == 0) {
            g_staticZeroFGuard = 1;
            g_staticZeroF = 0.0f;
          }
          if (g_staticZeroF2Guard == 0) {
            g_staticZeroF2Guard = 1;
            g_staticZeroF2 = 0.0f;
          }
          if (!(local[1] <= node->boundsMin.y)) {
            if (g_staticZeroFGuard == 0) {
              g_staticZeroFGuard = 1;
              g_staticZeroF = 0.0f;
            }
            if (g_staticZeroF2Guard == 0) {
              g_staticZeroF2Guard = 1;
              g_staticZeroF2 = 0.0f;
            }
            if (local[2] < node->boundsMax.z) {
              if (g_staticZeroFGuard == 0) {
                g_staticZeroFGuard = 1;
                g_staticZeroF = 0.0f;
              }
              if (g_staticZeroF2Guard == 0) {
                g_staticZeroF2Guard = 1;
                g_staticZeroF2 = 0.0f;
              }
              if (!(local[2] <= node->boundsMin.z)) {
                inside = 1;
              }
            }
          }
        }
      }
    }
    if ((inside != 0) && (node->kind < bestKind)) {
      bestKind = node->kind;
      best = idx;
    }
    idx = (s16)(idx + 1);
  }
  cursor->node = best;
  return;
}
