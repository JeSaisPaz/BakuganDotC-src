// bdc 0x08915c28 UiUpgradeTweenNamePanel
#include "bdc.h"

/* Starts the open/close tweens (`UiTweenBegin`, scale 1.0, flags 3) of the Bakugan name panel of
   the Bakugan upgrade screen (task 490, `maybe_UiScreen490Ctor`): sprites 14, 16 and 40..42 of
   the screen's sprite table, each with the tween of the same index. When opening (`hide` == 0) it
   first makes each sprite visible (`flags` bit 0), sets the attribute icons of the selected
   `bakugan` (`UiBakuganGetAttribute`; sprite 41 shows cell (attr / 3, attr % 3), sprite 40 cell
   (0, attr)) and its name image on sprite 42 (`UiBakuganSetFullNameTexture`,
   `"f_cha_name_baku_%02d"`), sets the layer mask of all five sprites to 2, then mirrors sprite 40
   (`GfxSpriteFlipU`) and re-places sprites 41 and 42 at their previous offset from sprite 40
   (minus 1 and 2 px in y). When closing it only starts the fade-out tweens. */

void UiUpgradeTweenNamePanel(UiUpgrade *self, u8 hide)

{
  GfxSprite *sprite;
  GfxSprite *icon;
  GfxSprite *name;
  float dxIcon;
  float dyIcon;
  float dxName;
  float dyName;
  int attr;
  int i;

  if (hide == 0) {
    for (i = 14; i < 15; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 16; i < 17; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    attr = UiBakuganGetAttribute(self->bakugan & 0xff);
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[41], (float)(attr / 3), (float)(attr % 3));
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[40], 0.0f, (float)attr);
    UiBakuganSetFullNameTexture(((GfxSprite **)self->base.data)[42], self->bakugan & 0xff);
    for (i = 40; i < 43; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    ((GfxSprite **)self->base.data)[14]->layerMask = 2;
    ((GfxSprite **)self->base.data)[16]->layerMask = 2;
    ((GfxSprite **)self->base.data)[40]->layerMask = 2;
    ((GfxSprite **)self->base.data)[41]->layerMask = 2;
    ((GfxSprite **)self->base.data)[42]->layerMask = 2;

    sprite = ((GfxSprite **)self->base.data)[40];
    icon = ((GfxSprite **)self->base.data)[41];
    name = ((GfxSprite **)self->base.data)[42];
    dxIcon = sprite->posX - icon->posX;
    dyIcon = (sprite->posY - icon->posY) - 1.0f;
    dxName = sprite->posX - name->posX;
    dyName = (sprite->posY - name->posY) - 2.0f;
    GfxSpriteFlipU(sprite);
    ((GfxSprite **)self->base.data)[41]->posX = ((GfxSprite **)self->base.data)[40]->posX + dxIcon;
    ((GfxSprite **)self->base.data)[41]->posY = ((GfxSprite **)self->base.data)[40]->posY + dyIcon;
    ((GfxSprite **)self->base.data)[42]->posX = ((GfxSprite **)self->base.data)[40]->posX + dxName;
    ((GfxSprite **)self->base.data)[42]->posY = ((GfxSprite **)self->base.data)[40]->posY + dyName;
  }
  else {
    for (i = 14; i < 15; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 16; i < 17; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 40; i < 43; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
