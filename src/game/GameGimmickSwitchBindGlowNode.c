// bdc 0x088db3c8 GameGimmickSwitchBindGlowNode
#include "bdc.h"

/* Clears the vec4 at `+0x180` and binds the node callback `0x088db39c` with that vec4 to the
   switch's glow node `fz_quest_switch01_04` (`GfxModelSetMaterialAnimCallback`). Called by `GameGimmickSwitchCtor`.
    */

void GameGimmickSwitchBindGlowNode(GameGimmickSwitch *gimmick)

{
  gimmick->glow[0] = 0.0f;
  gimmick->glow[1] = 0.0f;
  gimmick->glow[2] = 0.0f;
  gimmick->glow[3] = 0.0f;
  GfxModelSetMaterialAnimCallback
            ((GfxModel *)gimmick,"fz_quest_switch01_04",GameGimmickSwitchGlowDlWriteTexOffsetU,
             gimmick->glow);
  return;
}

