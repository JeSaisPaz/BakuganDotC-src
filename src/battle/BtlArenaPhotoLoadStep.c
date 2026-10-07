// bdc 0x08845ed4 BtlArenaPhotoLoadStep
#include "bdc.h"

/* Load state machine of the arena portrait task (`BtlArenaPhotoTaskUpdate`, `step`): states
   0..2 / 10..12 / 20..22 handle opponents 1..3 (`BtlFindTeamBakugan`). An opponent with a
   positive duplicate index (`BtlBakuganGetSameKindIndex`) gets a 64-byte path from the low heap
   formatted as `"data/battle/arena_pho/arena_com_pho_%03d_%02d.lzs"` (kind, index), requested from
   the data manager (`IoDataMngRequest`, flags 10 via `IoDataAddFlags`); once loaded, a
   0x140-byte texture object (`GfxTextureCtor`) named after the file stem is stored in
   `sprites[slot]`. State 0 jumps straight to 100 and returns 1 when no opponent has a nonzero
   index; every other state outside the table (3..9, 13..19, >= 23, i.e. 100 = done) returns 1.
   All other paths return 0. */

int BtlArenaPhotoLoadStep(BtlArenaPhotoTask *task)
{
  BtlBakugan *unit;
  IoData *load;
  CoreObject *tex;
  char *name;
  char *mark;
  bool fromLow;
  u32 kind;
  int index;
  char stem0[64];
  char stem1[64];
  char stem2[64];

  switch (task->step) {
  case 0:
    if (BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(1)) == 0 &&
        BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(2)) == 0 &&
        BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(3)) == 0) {
      task->step = 100;
      return 1;
    }
    if (BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(1)) <= 0) {
      task->step = 10;
      return 0;
    }
    task->step = task->step + 1;
    /* fall through */
  case 1:
    unit = BtlFindTeamBakugan(1);
    if (unit != NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      name = MemAlloc(64 * sizeof(char), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      task->paths[0] = name;
      kind = unit->base.base.unk08;
      index = BtlBakuganGetSameKindIndex(unit);
      sprintf(task->paths[0], "data/battle/arena_pho/arena_com_pho_%03d_%02d.lzs", kind, index);
      load = IoDataMngRequest(IoGetDataMng(), task, task->paths[0], 1, false, false);
      task->loads[0] = load;
      if (load != NULL) {
        IoDataAddFlags(load, 10);
      }
    }
    task->step = task->step + 1;
    /* fall through */
  case 2:
    load = task->loads[0];
    if (load == NULL) {
      task->step = 10;
      return 0;
    }
    if (!IoDataIsDone(load)) {
      return 0;
    }
    strcpy(stem0, task->paths[0]);
    mark = strrchr(stem0, '.');
    if (mark != NULL) {
      *mark = '\0';
    }
    mark = strrchr(stem0, '/');
    name = (mark != NULL) ? mark + 1 : stem0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    tex = MemAlloc(sizeof(GfxTexture), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (tex != NULL) {
      GfxTextureCtor(tex, name, IoDataGetBuffer(task->loads[0]), 1);
    }
    task->sprites[0] = tex;
    task->step = 10;
    return 0;

  case 10:
    if (BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(2)) <= 0) {
      task->step = 20;
      return 0;
    }
    task->step = task->step + 1;
    /* fall through */
  case 11:
    unit = BtlFindTeamBakugan(2);
    if (unit != NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      name = MemAlloc(64 * sizeof(char), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      task->paths[1] = name;
      kind = unit->base.base.unk08;
      index = BtlBakuganGetSameKindIndex(unit);
      sprintf(task->paths[1], "data/battle/arena_pho/arena_com_pho_%03d_%02d.lzs", kind, index);
      load = IoDataMngRequest(IoGetDataMng(), task, task->paths[1], 1, false, false);
      task->loads[1] = load;
      if (load != NULL) {
        IoDataAddFlags(load, 10);
      }
    }
    task->step = task->step + 1;
    /* fall through */
  case 12:
    load = task->loads[1];
    if (load == NULL) {
      task->step = 20;
      return 0;
    }
    if (!IoDataIsDone(load)) {
      return 0;
    }
    strcpy(stem1, task->paths[1]);
    mark = strrchr(stem1, '.');
    if (mark != NULL) {
      *mark = '\0';
    }
    mark = strrchr(stem1, '/');
    name = (mark != NULL) ? mark + 1 : stem1;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    tex = MemAlloc(sizeof(GfxTexture), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (tex != NULL) {
      GfxTextureCtor(tex, name, IoDataGetBuffer(task->loads[1]), 1);
    }
    task->sprites[1] = tex;
    task->step = 20;
    return 0;

  case 20:
    if (BtlBakuganGetSameKindIndex(BtlFindTeamBakugan(3)) <= 0) {
      task->step = 100;
      return 0;
    }
    task->step = task->step + 1;
    /* fall through */
  case 21:
    unit = BtlFindTeamBakugan(3);
    if (unit != NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      name = MemAlloc(64 * sizeof(char), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      task->paths[2] = name;
      kind = unit->base.base.unk08;
      index = BtlBakuganGetSameKindIndex(unit);
      sprintf(task->paths[2], "data/battle/arena_pho/arena_com_pho_%03d_%02d.lzs", kind, index);
      load = IoDataMngRequest(IoGetDataMng(), task, task->paths[2], 1, false, false);
      task->loads[2] = load;
      if (load != NULL) {
        IoDataAddFlags(load, 10);
      }
    }
    task->step = task->step + 1;
    /* fall through */
  case 22:
    load = task->loads[2];
    if (load == NULL) {
      task->step = 100;
      return 0;
    }
    if (!IoDataIsDone(load)) {
      return 0;
    }
    strcpy(stem2, task->paths[2]);
    mark = strrchr(stem2, '.');
    if (mark != NULL) {
      *mark = '\0';
    }
    mark = strrchr(stem2, '/');
    name = (mark != NULL) ? mark + 1 : stem2;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    tex = MemAlloc(sizeof(GfxTexture), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (tex != NULL) {
      GfxTextureCtor(tex, name, IoDataGetBuffer(task->loads[2]), 1);
    }
    task->sprites[2] = tex;
    task->step = 100;
    return 0;

  default:
    return 1;
  }
}
