// bdc 0x089b1ffc GameStageGetRecommendation
#include "bdc.h"

/* Returns the recommendation word of a story stage: the s16 at index `areaBase[area] + slot` of the
   table `g_gameStageRecTable` (`areaBase` = `g_gameStageRecAreaBase`, same indexing as
   `GameStageGetInfo`). The low byte is a Bakugan id and the high byte an attribute index;
   `UiHologramViewPickMessages` compares the low byte with the partner Bakugan (profile `+0x48c`,
   or its pair form `UiBakuganGetPair`) and the high byte with the attributes of the holograms
   placed in the four save-profile slots `+0x84..+0x87` (decoded `(id - 0xe) / 3` through
   `UiHologramGalleryMapAttribute`) to choose the hologram's advice messages. */

s32 GameStageGetRecommendation(u8 area, u8 slot)

{
  return g_gameStageRecTable[g_gameStageRecAreaBase[area] + slot];
}

