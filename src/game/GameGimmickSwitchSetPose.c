// bdc 0x088db408 GameGimmickSwitchSetPose
#include "bdc.h"

/* Puts the switch model in its off (`on == 0`: `+0x190 = 0`, `+0xb0 = 0`) or on pose (`+0x190 =
   1.0`, `+0xb0 = 1`) through the model motion helpers `GfxModelSetMotionStart`/`GfxModelSetMotionEnd`/`GfxModelSwapMotionFrame`
   (frame 3, blend 3 or 60). */

void GameGimmickSwitchSetPose(GameGimmickSwitch *gimmick, u8 on)

{
  if (on == '\0') {
    gimmick->pose = 0.0f;
    GfxModelSetMotionStart((GfxModel *)gimmick,0.0f);
    GfxModelSetMotionEnd((GfxModel *)gimmick,3.0f);
    GfxModelSwapMotionFrame((GfxModel *)gimmick,3.0f);
    (gimmick->base).base.motionEnded = '\0';
  }
  else {
    gimmick->pose = 1.0f;
    GfxModelSetMotionStart((GfxModel *)gimmick,3.0f);
    GfxModelSetMotionEnd((GfxModel *)gimmick,60.0f);
    GfxModelSwapMotionFrame((GfxModel *)gimmick,3.0f);
    (gimmick->base).base.motionEnded = '\x01';
  }
  return;
}

