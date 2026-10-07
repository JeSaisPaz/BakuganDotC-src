// bdc 0x08914dd8 UiUpgradeApplyPurchase
#include "bdc.h"

/* Buys upgrade `slot` for the selected Bakugan on the Bakugan upgrade screen (task 490,
   `UiUpgradeCtor`): marks it owned in the save profile (`upgradeOwned[bakugan][slot] = 1`),
   shows the slot's owned icon (sprite 66 + slot, flags bit0 = visible) and hides its 5 cost
   sprites (72 + slot * 5 ..), plays the model's `modelName` motion with blend 0.2
   (`GfxModelPlayMotion`), restarts the background fab at frame 0 (`GfxFabSeek`, when
   `bgData` and its fab exist), sets `points` = save word 45 + the cost of the focused slot's
   upgrade (`g_upgradeInfo`), redraws the three counters with `UiUpgradeSetNumber` at the
   positions of sprites 23, 30 and 37 (profile points, `points`, profile points - word 45) and
   plays sound 10 when a sound manager exists. */

void UiUpgradeApplyPurchase(UiUpgrade *self, s32 slot)
{
  GfxSprite **sprites;
  GfxSprite *pos;
  GfxModel *model;
  s32 motion;
  u32 spent;
  u8 upgradeId;
  s32 profilePoints;
  GfxFab **bg;
  int i;

  SaveGetProfile()->data->upgradeOwned[self->bakugan][slot] = 1;
  sprites = (GfxSprite **)self->base.data;
  sprites[66 + slot]->flags |= 1;
  for (i = 0; i < 5; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[72 + slot * 5 + i]->flags &= ~1u;
  }

  model = (GfxModel *)self->model;
  motion = GmoMotionIndexOfName(GmoMotionMgrGet(), self->modelName);
  GfxModelPlayMotion(0.2f, model, motion, 0);

  bg = (GfxFab **)self->base.bgData;
  if (bg != NULL && *bg != NULL) {
    GfxFabSeek(*bg, 0);
  }

  spent = SaveProfileGetWord(SaveGetProfile(), 45);
  upgradeId = UiUpgradeGetUpgradeId(self->bakugan, self->focus);
  self->points = spent + g_upgradeInfo[upgradeId].cost;

  profilePoints = SaveGetProfile()->data->points;
  pos = ((GfxSprite **)self->base.data)[23];
  UiUpgradeSetNumber(self, profilePoints, 23, 0, pos->posX, pos->posY);
  pos = ((GfxSprite **)self->base.data)[30];
  UiUpgradeSetNumber(self, self->points, 30, 1, pos->posX, pos->posY);

  profilePoints = SaveGetProfile()->data->points;
  spent = SaveProfileGetWord(SaveGetProfile(), 45);
  pos = ((GfxSprite **)self->base.data)[37];
  UiUpgradeSetNumber(self, profilePoints - spent, 37, 0, pos->posX, pos->posY);

  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), 10, 0, 0);
  }
}
