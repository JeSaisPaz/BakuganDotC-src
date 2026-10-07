// bdc 0x08915124 UiUpgradePayCost
#include "bdc.h"

/* Pays for the selected upgrade on the Bakugan upgrade screen (`UiUpgradeCtor`, task 490;
   selected Bakugan `bakugan`, selected slot `focus`): subtracts cost/10 (`g_upgradeInfo` of
   `UiUpgradeGetUpgradeId`) from the running total `points` and from the profile points (clamped
   to 0..9999999), refreshes the three number displays (`UiUpgradeSetNumber` at sprites 0x17,
   0x1e, 0x25, each at its own sprite's position) and returns 0 when `points` exceeds save word
   0x2d, else 1. */

int UiUpgradePayCost(UiUpgrade *self)
{
  SaveProfileData *data;
  GfxSprite *sprite;
  int price;
  int total;
  int profilePoints;
  u8 id;

  id = UiUpgradeGetUpgradeId(self->bakugan, self->focus);
  price = g_upgradeInfo[id].cost / 10;
  self->points = self->points - price;
  data = SaveGetProfile()->data;
  total = data->points - price;
  if (total > 9999999) {
    total = 9999999;
  } else if (total < 0) {
    total = 0;
  }
  data->points = total;

  profilePoints = SaveGetProfile()->data->points;
  sprite = ((GfxSprite **)self->base.data)[0x17];
  UiUpgradeSetNumber(self, profilePoints, 0x17, 0, sprite->posX, sprite->posY);
  sprite = ((GfxSprite **)self->base.data)[0x1e];
  UiUpgradeSetNumber(self, self->points, 0x1e, 1, sprite->posX, sprite->posY);

  profilePoints = SaveGetProfile()->data->points;
  total = profilePoints - (int)SaveProfileGetWord(SaveGetProfile(), 0x2d);
  sprite = ((GfxSprite **)self->base.data)[0x25];
  UiUpgradeSetNumber(self, total, 0x25, 0, sprite->posX, sprite->posY);

  if ((int)SaveProfileGetWord(SaveGetProfile(), 0x2d) < self->points) {
    return 0;
  }
  return 1;
}
