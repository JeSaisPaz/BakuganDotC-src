// bdc 0x08a18d58 GmoDlCachePatch
#include "bdc.h"

/* Rebuilds one copy of a model's cached display list (`model->dlCache`) at `dst`: walks the
   cache's patch table and, when `src` is given, copies the unchanged words between patch spots
   (and after the last one, up to `listWords`) from the same offsets of `src`. At each entry's
   `wordOffset` it then writes, by entry flag:
   - 1: forgets the last bone sub-list block (see 4);
   - 2: the world matrix (`0x3a` + 12 `0x3b` words): the model's `rootMatrix`, or the identity when
     `flags28 & 1`, combined with the node's `localMatrix` through `GmoMat4MulOffsetScale` when
     node flag 0x10000 is set;
   - 4: the node's bone matrices (bone node `localMatrix` x inverse bind matrix, through
     `GmoMat4MulOffsetScale`). Without an entry `block`: `0xff` then per bone `0x2a` (index
     `i*12`) + 12 `0x2b` words. With a `block`: ORIGIN/BASE/JUMP over a run of per-bone 13-word
     sub-lists (12 `0x2b` words + RET `0x0b`); this run is skipped when one was already written
     since the last flag-1 entry.
   `slot` is unused. */

static u32 GmoDlCacheFloatWord(u32 cmd, float f)
{
  union { float f; u32 u; } bits;

  bits.f = f;
  return cmd | (bits.u >> 8);
}

void GmoDlCachePatch(u32 *dst, const u32 *src, GmoModel *model, s32 slot)
{
  GmoDlCache *cache = (GmoDlCache *)model->dlCache;
  GmoDlCacheEntry *entry;
  GmoNode *node;
  const float *m;
  u32 *out;
  u32 *next;
  u32 *boneBlock;
  float tmp[16] __attribute__((aligned(16)));
  s32 entryCount;
  s32 i;
  s32 row;
  s32 bone;
  s32 boneCount;
  u32 boneIndex;
  u16 flags;

  (void)slot;
  entryCount = cache->entryCount;
  boneBlock = NULL;
  entry = cache->entries;
  out = dst;
  for (i = 0; i < entryCount; i++, entry++) {
    node = &model->nodes[entry->node];
    next = dst + entry->wordOffset;
    if (src != NULL) {
      memcpy(out, src + (out - dst), (size_t)((u8 *)next - (u8 *)out));
    }
    out = next;
    flags = entry->flags;
    if ((flags & 1) != 0) {
      boneBlock = NULL;
    }
    if ((flags & 2) != 0) {
      m = model->rootMatrix;
      if ((model->flags28 & 1) != 0) {
        m = (const float *)&g_gmoIdentityMatrix;
      }
      if ((node->flags & 0x10000) != 0) {
        GmoMat4MulOffsetScale((ScePspFMatrix4 *)tmp, (const ScePspFMatrix4 *)m,
                              (const ScePspFMatrix4 *)node->localMatrix,
                              (const ScePspFVector4 *)model->scaleVec);
        m = tmp;
      }
      next[0] = 0x3a000000;
      for (row = 0; row < 4; row++) {
        next[1 + row * 3] = GmoDlCacheFloatWord(0x3b000000, m[row * 4 + 0]);
        next[2 + row * 3] = GmoDlCacheFloatWord(0x3b000000, m[row * 4 + 1]);
        next[3 + row * 3] = GmoDlCacheFloatWord(0x3b000000, m[row * 4 + 2]);
      }
      out = next + 13;
    }
    if ((flags & 4) != 0 && (entry->block == NULL || boneBlock == NULL)) {
      boneCount = node->boneCount;
      if (entry->block == NULL) {
        *out++ = 0xff000000;
      } else {
        /* ORIGIN here, then jump past the 3 header words and 13 words per bone */
        u32 target = (u32)boneCount * 0x34 + 0xc;

        out[0] = 0x14000000;
        out[1] = 0x10000000 | ((target >> 24) << 16);
        out[2] = 0x08000000 | (target & 0xffffff);
        out += 3;
        boneBlock = out;
      }
      if (boneCount > 0) {
        if (entry->block != NULL) {
          for (bone = 0; bone < boneCount; bone++) {
            GmoMat4MulOffsetScale(
                (ScePspFMatrix4 *)tmp,
                (const ScePspFMatrix4 *)model->nodes[((const s16 *)node->block10)[bone]].localMatrix,
                (const ScePspFMatrix4 *)node->block14 + bone, (const ScePspFVector4 *)model->scaleVec);
            for (row = 0; row < 4; row++) {
              out[row * 3 + 0] = GmoDlCacheFloatWord(0x2b000000, tmp[row * 4 + 0]);
              out[row * 3 + 1] = GmoDlCacheFloatWord(0x2b000000, tmp[row * 4 + 1]);
              out[row * 3 + 2] = GmoDlCacheFloatWord(0x2b000000, tmp[row * 4 + 2]);
            }
            out += 12;
            *out++ = 0x0b000000;
          }
        } else {
          boneIndex = 0;
          for (bone = 0; bone < boneCount; bone++) {
            const s16 boneNode = ((const s16 *)node->block10)[bone];
            const float *invBind = (const float *)node->block14 + bone * 16;
            const GmoNode *nodes = model->nodes;

            *out++ = 0x2a000000 | boneIndex;
            GmoMat4MulOffsetScale((ScePspFMatrix4 *)tmp, (const ScePspFMatrix4 *)nodes[boneNode].localMatrix,
                                  (const ScePspFMatrix4 *)invBind, (const ScePspFVector4 *)model->scaleVec);
            for (row = 0; row < 4; row++) {
              out[row * 3 + 0] = GmoDlCacheFloatWord(0x2b000000, tmp[row * 4 + 0]);
              out[row * 3 + 1] = GmoDlCacheFloatWord(0x2b000000, tmp[row * 4 + 1]);
              out[row * 3 + 2] = GmoDlCacheFloatWord(0x2b000000, tmp[row * 4 + 2]);
            }
            out += 12;
            boneIndex += 0xc;
          }
        }
      }
    }
  }
  if (src != NULL) {
    memcpy(out, src + (out - dst), (size_t)((cache->listWords - (out - dst)) * 4));
  }
}
