// bdc 0x088f5578 GameQuestParsePathTable
#include "bdc.h"

/* Parses the quest path table `f%d_quest_%02d_00.ptlb` (loaded by `GameFieldCameraLoadQuestCam`),
   a big-endian binary file, into the `GameQuestPathSet` `vec`: the file starts with up to 8
   big-endian section offsets (a 0 offset ends the list); each section holds a count and that many
   0x50-byte `GameQuestPathRecord`s. Each section becomes a `GameQuestPathNodeVec` (low heap, capacity = count)
   and each record a 0x60-byte `GameQuestPathNode` (position, rotation, bounds read as big-endian
   floats, kind/flags copied) with up to 4 `GameQuestPathLink`s (one per set `linkUsed` byte,
   filled from the big-endian `linkFromNode`/`linkCamSet` halfwords and `linkFlag`). Nodes are appended
   to their section, sections to `vec` (growable vectors, doubled with `memcpy`/`MemFree`).
   Finally `vec->cur` is the first section (or `g_gameQuestPathNullSection`, zeroed, when there
   is none) and `vec->flag` is set to 1. `size` is unused.
   The inlined node constructor zeroes `pos`, `boundsMin` and `boundsMax` (bank zero vector C720)
   before the parsed x/y/z overwrite them; `w` stays 0. */

#define PTLB_BE32(p) \
  ((u32)(p)[1] << 16 | (u32)(p)[0] << 24 | ((u32)(p)[2] << 8 | (u32)(p)[3]))

/* inlined static initialisers of the zero constants, run before every float store */
#define ZERO_F_INIT()                                                                              \
  if (g_staticZeroFGuard == 0) {                                                                   \
    g_staticZeroFGuard = 1;                                                                        \
    g_staticZeroF = 0.0f;                                                                          \
  }
#define ZERO_F4_INIT()                                                                             \
  if (g_staticZeroF4Guard == 0) {                                                                  \
    g_staticZeroF4Guard = 1;                                                                       \
    g_staticZeroF4 = 0.0f;                                                                         \
  }

void GameQuestParsePathTable(void *vec, u8 *file, s32 size)

{
  GameQuestPathSet *set = (GameQuestPathSet *)vec;
  union { u32 u; float f; } bits;
  bool fromLow;
  u32 sectionIdx;
  u8 *offsets;
  u32 offset;
  u8 *sectionData;
  GameQuestPathRecord *rec;
  s32 recCount;
  s32 recIdx;
  s32 linkIdx;
  s32 count;
  s32 cap;
  s32 newCap;
  union { u16 h; u8 b[2]; } raw;
  void *newData;
  void *oldData;
  GameQuestPathNodeVec *section;
  GameQuestPathNodeVec *sec;
  GameQuestPathNode *node;
  GameQuestPathNode *newNode;
  GameQuestPathLink *link;
  GameQuestPathLink *newLink;
  bool canPush;

  sectionIdx = 0;
  offsets = file;
  do {
    offset = PTLB_BE32(offsets);
    if (offset == 0) break;
    sectionData = file + offset;
    recCount = (s32)PTLB_BE32(sectionData);
    rec = (GameQuestPathRecord *)((u32 *)sectionData + 1);
    section = NULL;
    if (recCount != 0) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      sec = (GameQuestPathNodeVec *)MemAlloc(0xc, NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      section = NULL;
      if (sec != NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        newData = MemAlloc(recCount << 2, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        sec->data = (GameQuestPathNode **)newData;
        sec->cap = recCount;
        sec->count = 0;
        section = sec;
      }
    }
    for (recIdx = 0; recIdx < recCount; recIdx++) {
      /* inlined GameQuestPathNode constructor */
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      newNode = (GameQuestPathNode *)MemAlloc(0x60, NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      node = NULL;
      if (newNode != NULL) {
        /* sv.q of the bank zero vector C720 */
        newNode->pos.x = 0.0f;
        newNode->pos.y = 0.0f;
        newNode->pos.z = 0.0f;
        newNode->pos.w = 0.0f;
        newNode->rot.w = 0.0f;
        newNode->rot.z = 0.0f;
        newNode->rot.y = 0.0f;
        newNode->rot.x = 0.0f;
        newNode->boundsMin.x = 0.0f;
        newNode->boundsMin.y = 0.0f;
        newNode->boundsMin.z = 0.0f;
        newNode->boundsMin.w = 0.0f;
        newNode->boundsMax.x = 0.0f;
        newNode->boundsMax.y = 0.0f;
        newNode->boundsMax.z = 0.0f;
        newNode->boundsMax.w = 0.0f;
        newNode->kind = 2;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        newData = MemAlloc(0x28, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        newNode->links = (GameQuestPathLink **)newData;
        newNode->linkCap = 10;
        newNode->linkCount = 0;
        newNode->flag50 = 0;
        newNode->flag51 = 0;
        node = newNode;
      }

      bits.u = PTLB_BE32(rec->pos[0]);
      ZERO_F_INIT();
      node->pos.x = bits.f;
      bits.u = PTLB_BE32(rec->pos[1]);
      ZERO_F_INIT();
      node->pos.y = bits.f;
      bits.u = PTLB_BE32(rec->pos[2]);
      ZERO_F_INIT();
      node->pos.z = bits.f;
      bits.u = PTLB_BE32(rec->rot[0]);
      ZERO_F4_INIT();
      node->rot.x = bits.f;
      bits.u = PTLB_BE32(rec->rot[1]);
      ZERO_F4_INIT();
      node->rot.y = bits.f;
      bits.u = PTLB_BE32(rec->rot[2]);
      ZERO_F4_INIT();
      node->rot.z = bits.f;
      bits.u = PTLB_BE32(rec->rot[3]);
      ZERO_F4_INIT();
      node->rot.w = bits.f;
      bits.u = PTLB_BE32(rec->boundsMax[0]);
      ZERO_F_INIT();
      node->boundsMax.x = bits.f;
      bits.u = PTLB_BE32(rec->boundsMax[1]);
      ZERO_F_INIT();
      node->boundsMax.y = bits.f;
      bits.u = PTLB_BE32(rec->boundsMax[2]);
      ZERO_F_INIT();
      node->boundsMax.z = bits.f;
      bits.u = PTLB_BE32(rec->boundsMin[0]);
      ZERO_F_INIT();
      node->boundsMin.x = bits.f;
      bits.u = PTLB_BE32(rec->boundsMin[1]);
      ZERO_F_INIT();
      node->boundsMin.y = bits.f;
      bits.u = PTLB_BE32(rec->boundsMin[2]);
      ZERO_F_INIT();
      node->boundsMin.z = bits.f;
      node->flag50 = rec->flag50;
      node->flag51 = rec->flag51;
      node->kind = rec->kind;

      for (linkIdx = 0; linkIdx < 4; linkIdx++) {
        if (rec->linkUsed[linkIdx] == 0) continue;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        newLink = (GameQuestPathLink *)MemAlloc(6, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        link = NULL;
        if (newLink != NULL) {
          newLink->fromNode = -1;
          newLink->camSet = -1;
          newLink->flag = 0;
          link = newLink;
        }
        /* big-endian halfwords: copied with lh, then reassembled byte by byte */
        raw.h = *(u16 *)&rec->linkFromNode[linkIdx];
        link->fromNode = (s16)(raw.b[1] | raw.b[0] << 8);
        raw.h = *(u16 *)&rec->linkCamSet[linkIdx];
        link->camSet = (s16)(raw.b[1] | raw.b[0] << 8);
        link->flag = rec->linkFlag[linkIdx] != 0;

        /* push link onto node->links (inlined vector push_back) */
        count = node->linkCount;
        cap = node->linkCap;
        newCap = cap * 2;
        if (count >= cap && newCap != 0) {
          MemLock();
          fromLow = MemIsAllocFromLow();
          MemSetAllocFromLow(true);
          newData = MemAlloc(newCap << 2, NULL, 0);
          MemSetAllocFromLow(fromLow);
          MemUnlock();
          count = node->linkCount;
          if (newCap < count) {
            node->linkCount = newCap;
            count = newCap;
          }
          memcpy(newData, node->links, count << 2);
          oldData = node->links;
          node->linkCap = newCap;
          if (oldData != NULL) {
            MemLock();
            MemFree(oldData, NULL, 0);
            MemUnlock();
            node->links = NULL;
          }
          node->links = (GameQuestPathLink **)newData;
          count = node->linkCount;
          cap = node->linkCap;
        }
        if (count < cap) {
          node->links[count] = link;
          node->linkCount = node->linkCount + 1;
        }
      }

      /* push node onto section (inlined vector push_back) */
      count = section->count;
      cap = section->cap;
      newCap = cap * 2;
      if (count >= cap && newCap != 0) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        newData = MemAlloc(newCap << 2, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        count = section->count;
        if (newCap < count) {
          section->count = newCap;
          count = section->count;
        }
        memcpy(newData, section->data, count << 2);
        section->cap = newCap;
        oldData = section->data;
        if (oldData != NULL) {
          MemLock();
          MemFree(oldData, NULL, 0);
          MemUnlock();
          section->data = NULL;
        }
        section->data = (GameQuestPathNode **)newData;
        count = section->count;
        cap = section->cap;
      }
      if (count < cap) {
        section->data[count] = node;
        section->count = section->count + 1;
      }
      rec = rec + 1;
    }

    /* push section onto the set (inlined vector push_back) */
    count = set->count;
    cap = set->cap;
    canPush = count < cap;
    if (!canPush) {
      newCap = cap * 2;
      if (newCap != 0) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        newData = MemAlloc(newCap << 2, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        count = set->count;
        if (newCap < count) {
          set->count = newCap;
          count = newCap;
        }
        memcpy(newData, set->data, count << 2);
        oldData = set->data;
        set->cap = newCap;
        if (oldData != NULL) {
          MemLock();
          MemFree(oldData, NULL, 0);
          MemUnlock();
          set->data = NULL;
        }
        set->data = (void **)newData;
        count = set->count;
        canPush = count < set->cap;
      }
    }
    if (canPush) {
      set->data[count] = section;
      set->count = set->count + 1;
    }
    sectionIdx = sectionIdx + 1;
    offsets = offsets + 4;
  } while (sectionIdx < 8);

  if (set->count > 0) {
    set->cur = (GameQuestPathNodeVec *)set->data[0];
  }
  else {
    memset(&g_gameQuestPathNullSection, 0, 4);
    set->cur = g_gameQuestPathNullSection;
  }
  set->flag = 1;
  return;
}
