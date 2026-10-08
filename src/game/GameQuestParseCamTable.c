// bdc 0x088fa80c GameQuestParseCamTable
#include "bdc.h"

/* Parses a quest camera table `f%d_quest_%02d_00.cptb` (loaded by `GameFieldCameraLoadQuestCam`)
   into the `GameQuestCamTable` `table`. The big-endian file starts with up to 8 section offsets
   (a 0 offset ends the list); each section is a 32-bit record count followed by 0x58-byte records.
   Every section becomes a low-heap `GameQuestCamPtrVec` camera set (capacity = record count, NULL
   for an empty section) pushed onto `table`; every record a 0x70-byte low-heap
   `GameQuestCamEntry` pushed onto the set: `pos`/`pos2` start zeroed (the bank zero column C720),
   `vec10` from zero, a 10-entry s16 vector is allocated and `extra0`/`extra1` default to 3.9/30000,
   then the record's fields are stored (`sideOffset` below FLT_EPSILON becomes 600) and its 8 s16
   values pushed. Vector pushes double the capacity when full (nothing is pushed when the capacity
   is 0). Finally `table->cur` is the first set (or the zeroed `g_gameQuestCamNullSet` for an empty
   table) and `flag0` is set. A failed set/entry allocation only skips its initialisation: the
   NULL pointer is still pushed and written through. The `sv.q C720` stores are the bank zero vector. */

/* Low-heap allocation under the heap lock (the inlined allocator of the quest tables). */
static inline void *CptbAllocLow(s32 size)
{
  bool fromLow;
  void *p;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  p = MemAlloc(size, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  return p;
}

static inline void CptbFree(void *p)
{
  MemLock();
  MemFree(p, NULL, 0);
  MemUnlock();
}

static inline u32 CptbReadU32(const u8 *p)
{
  return ((u32)p[1] << 16) | ((u32)p[0] << 24) | (((u32)p[2] << 8) | p[3]);
}

static inline float CptbReadF32(const u8 *p)
{
  union {
    u32 u;
    float f;
  } v;

  v.u = CptbReadU32(p);
  return v.f;
}

/* Big-endian s16: one halfword load, byte-swapped through a stack copy. */
static inline s16 CptbReadS16(const u8 *p)
{
  union {
    s16 s;
    u8 b[2];
  } v;

  v.s = *(const s16 *)p;
  return (s16)((v.b[0] << 8) | v.b[1]);
}

/* Inlined function-local `static float zero = 0.0f;` initialisers of the vector helpers. */
static inline void CptbStaticZeroInit(void)
{
  if (g_staticZeroFGuard == 0) {
    g_staticZeroFGuard = 1;
    g_staticZeroF = 0.0f;
  }
}

static inline void CptbStaticZeroF4Init(void)
{
  if (g_staticZeroF4Guard == 0) {
    g_staticZeroF4Guard = 1;
    g_staticZeroF4 = 0.0f;
  }
}

/* Inlined push_back of the entry's s16 vector (`subData`/`subCap`/`subCount`). */
static inline void CptbEntryPushSub(GameQuestCamEntry *entry, s16 value)
{
  s32 newCap;
  s16 *buf;
  s32 count;

  count = entry->subCount;
  if (!(count < entry->subCap)) {
    newCap = entry->subCap + entry->subCap;
    if (newCap == 0) {
      return;
    }
    buf = CptbAllocLow(newCap * sizeof(s16));
    count = entry->subCount;
    if (newCap < count) {
      entry->subCount = newCap;
      count = newCap;
    }
    memcpy(buf, entry->subData, count * sizeof(s16));
    entry->subCap = newCap;
    if (entry->subData != NULL) {
      CptbFree(entry->subData);
      entry->subData = NULL;
    }
    count = entry->subCount;
    entry->subData = buf;
    if (!(count < entry->subCap)) {
      return;
    }
  }
  entry->subData[count] = value;
  entry->subCount = entry->subCount + 1;
}

void GameQuestParseCamTable(GameQuestCamTable *table, u8 *file)
{
  GameQuestCamPtrVec *set;
  GameQuestCamEntry *entry;
  GameQuestCamEntry **buf;
  const u8 *rec;
  float sideOffset;
  s32 numRecords;
  s32 newCap;
  s32 count;
  u32 offset;
  s32 i;
  s32 j;
  s32 k;

  for (i = 0; i < 8; i++) {
    offset = CptbReadU32(file + i * 4);
    if (offset == 0) {
      break;
    }
    rec = file + offset;
    numRecords = (s32)CptbReadU32(rec);
    rec += 4;

    set = NULL;
    if (numRecords != 0) {
      set = CptbAllocLow(sizeof(GameQuestCamPtrVec));
      if (set != NULL) {
        buf = CptbAllocLow(numRecords * sizeof(*buf));
        set->data = buf;
        set->cap = numRecords;
        set->count = 0;
      }
    }

    for (j = 0; j < numRecords; j++) {
      entry = CptbAllocLow(sizeof(GameQuestCamEntry));
      if (entry != NULL) {
        /* sv.q C720: the bank zero vector */
        entry->pos.x = 0.0f;
        entry->pos.y = 0.0f;
        entry->pos.z = 0.0f;
        entry->pos.w = 0.0f;
        entry->vec10[3] = 0.0f;
        entry->vec10[2] = 0.0f;
        entry->vec10[1] = 0.0f;
        entry->vec10[0] = 0.0f;
        entry->pos2.x = 0.0f;
        entry->pos2.y = 0.0f;
        entry->pos2.z = 0.0f;
        entry->pos2.w = 0.0f;
        entry->subData = CptbAllocLow(10 * sizeof(s16));
        entry->subCap = 10;
        entry->subCount = 0;
        entry->extra0 = 3.9f;
        entry->extra1 = 30000.0f;
      }

      /* inlined push_back onto the camera set */
      count = set->count;
      if (!(count < set->cap)) {
        newCap = set->cap + set->cap;
        if (newCap != 0) {
          buf = CptbAllocLow(newCap * sizeof(*buf));
          if (newCap < set->count) {
            set->count = newCap;
          }
          memcpy(buf, set->data, set->count * sizeof(*buf));
          set->cap = newCap;
          if (set->data != NULL) {
            CptbFree(set->data);
            set->data = NULL;
          }
          set->data = buf;
          count = set->count;
        }
      }
      if (count < set->cap) {
        set->data[count] = entry;
        set->count = set->count + 1;
      }

      CptbStaticZeroInit();
      entry->pos.x = CptbReadF32(rec + 0x00);
      CptbStaticZeroInit();
      entry->pos.y = CptbReadF32(rec + 0x04);
      CptbStaticZeroInit();
      entry->pos.z = CptbReadF32(rec + 0x08);
      CptbStaticZeroF4Init();
      entry->vec10[0] = CptbReadF32(rec + 0x0c);
      CptbStaticZeroF4Init();
      entry->vec10[1] = CptbReadF32(rec + 0x10);
      CptbStaticZeroF4Init();
      entry->vec10[2] = CptbReadF32(rec + 0x14);
      CptbStaticZeroF4Init();
      entry->vec10[3] = CptbReadF32(rec + 0x18);
      entry->f40 = CptbReadF32(rec + 0x28);
      entry->distance = CptbReadF32(rec + 0x2c);
      sideOffset = CptbReadF32(rec + 0x30);
      entry->sideOffset = sideOffset;
      if (sideOffset < 1.1920929e-07f) {
        entry->sideOffset = 600.0f;
      }
      entry->extra0 = CptbReadF32(rec + 0x38);
      entry->extra1 = CptbReadF32(rec + 0x3c);
      for (k = 0; k < 8; k++) {
        CptbEntryPushSub(entry, CptbReadS16(rec + 0x40 + k * 2));
      }
      CptbStaticZeroInit();
      entry->pos2.x = CptbReadF32(rec + 0x1c);
      CptbStaticZeroInit();
      entry->pos2.y = CptbReadF32(rec + 0x20);
      CptbStaticZeroInit();
      entry->pos2.z = CptbReadF32(rec + 0x24);
      entry->id = CptbReadS16(rec + 0x50);
      entry->f4c = CptbReadF32(rec + 0x34);
      entry->type = rec[0x52];
      entry->springKind = rec[0x53];
      entry->collide = rec[0x54] != 0;
      rec += 0x58;
    }

    /* inlined push_back onto the table (its `data` holds the camera sets) */
    count = table->count;
    if (!(count < table->cap)) {
      newCap = table->cap + table->cap;
      if (newCap != 0) {
        buf = CptbAllocLow(newCap * sizeof(*buf));
        count = table->count;
        if (newCap < count) {
          table->count = newCap;
          count = newCap;
        }
        memcpy(buf, table->data, count * sizeof(*buf));
        table->cap = newCap;
        if (table->data != NULL) {
          CptbFree(table->data);
          table->data = NULL;
        }
        table->data = (GameQuestCamPtrVec **)buf;
        count = table->count;
      }
    }
    if (count < table->cap) {
      table->data[count] = set;
      table->count = table->count + 1;
    }
  }

  if (0 < table->count) {
    table->cur = table->data[0];
  } else {
    memset(&g_gameQuestCamNullSet, 0, 4);
    table->cur = g_gameQuestCamNullSet;
  }
  table->flag0 = 1;
}
