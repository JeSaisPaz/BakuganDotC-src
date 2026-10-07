// bdc 0x08a23a48 SndSsVoiceListFindSteal
#include "bdc.h"

/* Picks a voice to steal from a voice list: among the `count` records with priority (`+0x58`) ≤
   `priority`, the one with the lowest envelope height (`+4`). Returns its voice index or -1. */

s32 SndSsVoiceListFindSteal(u8 **list, u32 count, u32 priority)
{
  SndSsVoice *cand[32];
  u32 n;
  u32 i;
  u32 minHeight;

  n = 0;
  minHeight = 0xffffffff;
  for (i = 0; i < count; i++) {
    SndSsVoice *v = (SndSsVoice *)list[i];
    if ((u32)v->priority <= priority) {
      cand[n++] = v;
      if (v->field04 < minHeight) {
        minHeight = v->field04;
      }
    }
  }
  for (i = 0; i < n; i++) {
    if (cand[i]->field04 == minHeight) {
      return cand[i]->sasVoice;
    }
  }
  return -1;
}
