// bdc 0x088da574 GameGimmickTriggerZoneDraw
#include "bdc.h"

/* Draw method of the trigger-zone gimmick (vtable `0x08af3474` slot 8): `GfxModelDlWriteState`,
   marks the zone as drawn (`drawn = 1`) and copies the fade alpha (`ambient[3]`) into the
   alpha component of its effect's colour (`effect->color[3]`). */

void GameGimmickTriggerZoneDraw(GameGimmickTriggerZone *gimmick, u32 **dl)
{
  float alpha;

  GfxModelDlWriteState((GfxModel *)gimmick, dl);
  alpha = gimmick->base.base.ambient[3];
  gimmick->drawn = 1;
  ((GfxEffect *)gimmick->effect)->color[3] = alpha;
}
