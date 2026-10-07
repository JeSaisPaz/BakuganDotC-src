// bdc 0x089b2ae8 SaveProfileRecordStageRank
#include "bdc.h"

/* Stores the evaluation of a cleared story stage in the profile if it beats the current one:
   `stageRecords[stage]` (`id`, `rank | 0x80` (bit 7 = set), `score`) is replaced when empty, when
   `rank` is better (lower; 0 = S … 3 = C) than the stored one, or when the rank is equal and
   `score` is higher. Stage 0x25 is additionally mirrored into `stageRecords[20]`. */

void SaveProfileRecordStageRank(u32 stage, u8 id, u8 rank, u16 score)
{
  s32 s = stage & 0xff;
  u8 hi = s / 4;
  u8 lo = s % 4;
  s32 better = 0;

  if ((SaveGetProfile()->data->stageRecords[hi * 4 + lo].rank & 0x80) == 0) {
    better = 1;
  }
  else if (rank < (SaveGetProfile()->data->stageRecords[hi * 4 + lo].rank & 0x7f)) {
    better = 1;
  }
  else if (rank == (SaveGetProfile()->data->stageRecords[hi * 4 + lo].rank & 0x7f)) {
    if (SaveGetProfile()->data->stageRecords[hi * 4 + lo].score < score) {
      better = 1;
    }
  }
  if (better) {
    SaveGetProfile()->data->stageRecords[hi * 4 + lo].id = id;
    SaveGetProfile()->data->stageRecords[hi * 4 + lo].rank = rank | 0x80;
    SaveGetProfile()->data->stageRecords[hi * 4 + lo].score = score;
    if (s == 0x25) {
      SaveGetProfile()->data->stageRecords[20].id = id;
      SaveGetProfile()->data->stageRecords[20].rank = rank | 0x80;
      SaveGetProfile()->data->stageRecords[20].score = score;
    }
  }
}
