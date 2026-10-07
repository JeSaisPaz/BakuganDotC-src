// bdc 0x0880df1c SaveGetTeamBakuganModelId
#include "bdc.h"

/* Returns the model/object id of the Bakugan in team slot `slot` (-1 = active):
   `SaveGetTeamBakugan(slot) + 0x57`, with 0x59 (Bakugan 2) remapped to 0x67 and 0x61 (Bakugan 10)
   to 0x60 (variants that share a model). */

s32 SaveGetTeamBakuganModelId(s32 slot)

{
  s32 model;

  if (slot == -1) {
    slot = SaveProfileGetWord(SaveGetProfile(), 0x13);
  }
  model = SaveGetTeamBakugan(slot) + 0x57;
  if (model < 0x5a) {
    if (0x58 < model) {
      return 0x67;
    }
  }
  else if (model == 0x61) {
    model = 0x60;
  }
  return model;
}

