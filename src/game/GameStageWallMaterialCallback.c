// bdc 0x088d3d6c GameStageWallMaterialCallback
#include "bdc.h"

/* `GfxModelForEachMaterialByName` callback applied by `GameStageBuild` to the
   `f3_quest_wall_re05_comp` materials of the stage model: sets material byte `+6` to 10 and clears
   the translucent bit 0x10 of the flag byte `+4`. */

void GameStageWallMaterialCallback(u8 *material)

{
  material[6] = '\n';
  material[4] = material[4] & 0xef;
  return;
}

