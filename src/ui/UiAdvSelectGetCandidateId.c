// bdc 0x08917468 UiAdvSelectGetCandidateId
#include "bdc.h"

/* Returns the Bakugan id of candidate `index` for the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner,
   locked, ?}` slots): from the fixed list {1, 3, 5, 6, 7, 8} when `adventure` is set, else from the
   21-byte table `0x08ac10f0`. */

u8 UiAdvSelectGetCandidateId(UiAdvSelect *self, bool adventure, u32 index)

{
  u8 table[24];
  u8 fixed[8] = {1, 3, 5, 6, 7, 8};
  
  memcpy(table,g_advSelectCandidateIds,0x15);
  if (adventure) {
    return fixed[index & 0xff];
  }
  else {
    return table[index & 0xff];
  }
}

