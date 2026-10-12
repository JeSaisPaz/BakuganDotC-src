// bdc 0x088d44f4 GameStageSpawnFieldPoints
#include "bdc.h"

/* Once per stage (`g_gameStageSpawned`), walks the 0x30-byte point records of the current stage
   (`g_gameStageSpawnTables``[profile word 8]`, entry `g_gameStageIndex`: count, records) and, for kinds 1..3 that
   also pass `GameStageIsOtherPointKind`, creates a field point (`GameFieldPointCreate` with the
   record position `+0x10`, the kind and the record index); as compiled that test always fails, so
   nothing is created. */

void GameStageSpawnFieldPoints(void)
{
  SaveProfile *self;
  u32 variant;
  GameStageSpawnPointList *list;
  s32 count;
  GameStageSpawnPoint *rec;
  s32 id;
  s32 kind;
  float pos[4] __attribute__((aligned(16)));

  self = SaveGetProfile();
  variant = SaveProfileGetWord(self, 8);
  list = g_gameStageSpawnTables[variant] + g_gameStageIndex;
  count = list->count;
  rec = list->records;
  if (g_gameStageSpawned == 0) {
    for (id = 0; id < count; id++, rec++) {
      kind = rec->kind;
      if (0 < kind && kind < 4 && GameStageIsOtherPointKind(kind) != 0) {
        const float *src = rec->pos;
        g_gameStageSpawned = 1;
        pos[0] = src[0];
        pos[1] = src[1];
        pos[2] = src[2];
        pos[3] = src[3];
        GameFieldPointCreate(pos, kind, id, rec->heading);
      }
    }
  }
}
