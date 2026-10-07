// bdc 0x088f4340 GameFieldLoadCharacterSet
#include "bdc.h"

/* Loads the character placement set of field area `area`, room `room` (`"f%01d_%02d.cset"` from the
   pack chain `g_ioLzsPackages`, copied to a low-heap buffer kept in `entries[1]` and `entries[0]`):
   the file is a u32 entry count followed by 0x2c-byte entries; every entry's position is converted in
   place from float to 20.12 fixed point (rounded half away from zero) and its angle from degrees to a
   16-bit binary angle. The indices of the entries accepted by `GameFieldCharSetEntryIsActive` are
   collected in a temporary low-heap list (appended at `placedCount`, which is incremented), one actor
   per listed entry is spawned with `GameFieldSpawnPlacedActor` (slot = running index), the list is
   freed and `GameFieldCharSetBuildGuardList` runs. The three loops are do-while: each runs at least
   once even for a count of 0. */

typedef struct CharSetFileEntry {
  union {
    float f;
    s32 i;
  } pos[3];   /* +0x00 float in the file, 20.12 fixed point after loading */
  u16 angle;  /* +0x0c degrees in the file, 16-bit binary angle after loading */
  u8 _unk0e[0x2c - 0xe];
} CharSetFileEntry;

typedef struct CharSetFile {
  u32 count;
  CharSetFileEntry entries[1];
} CharSetFile;

void GameFieldLoadCharacterSet(void *mgr, s32 area, u8 room, u8 reuse)
{
  GameFieldCharSet *set = (GameFieldCharSet *)mgr;
  char name[128];
  u32 size;
  u32 count;
  bool fromLow;
  void *buf;
  u8 *list;
  CharSetFileEntry *e;
  u8 i;
  s32 k;

  sprintf(name, "f%01d_%02d.cset", area, room);
  size = CorePackChainFindSize(g_ioLzsPackages, name);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buf = MemAlloc(size, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  set->entries[1] = buf;
  memcpy(buf, CorePackChainFind(g_ioLzsPackages, name), size);
  set->entries[0] = set->entries[1];

  i = 0;
  e = ((CharSetFile *)set->entries[1])->entries;
  do {
    for (k = 0; k < 3; k++) {
      float x = e->pos[k].f;
      float v = x * 4096.0f;

      if (x <= 0.0f) {
        v = v - 0.5f;
      } else {
        v = v + 0.5f;
      }
      e->pos[k].i = (s32)v;
    }
    i++;
    e->angle = (u16)(s32)((float)(s32)e->angle * 65536.0f * 0.00277777785f);
    count = ((CharSetFile *)set->entries[0])->count;
    e++;
  } while (i < count);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(count, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();

  i = 0;
  e = ((CharSetFile *)set->entries[0])->entries;
  do {
    if (GameFieldCharSetEntryIsActive(e)) {
      list[set->placedCount] = i;
      set->placedCount = set->placedCount + 1;
    }
    i++;
    e++;
  } while (i < ((CharSetFile *)set->entries[0])->count);

  i = 0;
  do {
    u8 idx = list[i];

    GameFieldSpawnPlacedActor(mgr, i, idx, &((CharSetFile *)set->entries[0])->entries[idx], (s8)reuse);
    i++;
  } while (i < set->placedCount);

  MemLock();
  MemFree(list, NULL, 0);
  MemUnlock();
  GameFieldCharSetBuildGuardList(mgr);
}
