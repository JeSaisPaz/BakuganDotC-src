// bdc 0x089174ec UiAdvSelectBuildCandidates
#include "bdc.h"

/* Builds the six candidate slots `candidates` (`+0x8a0`) of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376): clears them and `lockedCount`, then for each candidate id
   (`UiAdvSelectGetCandidateId`) whose bit is set in the profile's `ownedBakugan` bitset, stores
   the id and its pair entry (`UiBakuganGetPair(0, id)`). If the pair entry's `ownedBakugan` bit is
   set the slot gets `locked = 1`; otherwise `lockedCount` is incremented and, when the id is the
   profile's `curBakugan`, the slot gets `isCurrent = 1`. */

void UiAdvSelectBuildCandidates(UiAdvSelect *self)
{
  int i;
  int id;
  int pair;
  u8 owned;

  memset(self->candidates, 0, 0x18);
  self->lockedCount = 0;
  for (i = 0; i < 6; i++) {
    id = UiAdvSelectGetCandidateId(self, true, (u8)i);
    if ((u8)(SaveGetProfile()->data->ownedBakugan[id / 8] & (1 << (id % 8))) == 0) {
      continue;
    }
    self->candidates[i].bakugan = id;
    pair = UiBakuganGetPair(0, id);
    self->candidates[i].partner = pair;
    owned = SaveGetProfile()->data->ownedBakugan[pair / 8] & (1 << (pair % 8));
    if (owned != 0) {
      self->candidates[i].locked = 1;
    } else {
      self->lockedCount++;
      if (SaveGetProfile()->data->curBakugan == id) {
        self->candidates[i].isCurrent = 1;
      }
    }
  }
}
