// bdc 0x0893d940 UiPasscodeBuildSprites
#include "bdc.h"

/* Builds the sprites of `UiPasscode`: clears the part tween records (0x6b8 bytes),
   creates the 0x3f layout sprites (`UiLayoutCreateSprites`), hides the first 0x2b with alpha 0
   while saving their posZ in `spriteDepth`, and insets the UVs by 0.5 texel (`GfxSpriteInsetUv`)
   on sprite 0, sprite 0x25, the symbol sprites 0x14..0x1d and sprites 1..10. */

#define PASSCODE_SPRITE_VISIBLE 1u

void UiPasscodeBuildSprites(UiScreen *screen)
{
    UiPasscode *pc = (UiPasscode *)screen;
    u32 i;

    memset(pc->tween, 0, sizeof(pc->tween));
    UiLayoutCreateSprites(screen->spriteLayer, (GfxSprite **)screen->data, 0x3f);
    for (i = 0; i < 0x2b; i++) {
        ((GfxSprite **)screen->data)[i]->flags &= ~PASSCODE_SPRITE_VISIBLE;
        ((GfxSprite **)screen->data)[i]->alpha = 0.0f;
        pc->spriteDepth[i] = ((GfxSprite **)screen->data)[i]->posZ;
    }
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)screen->data)[0]);
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)screen->data)[0x25]);
    for (i = 0x14; i < 0x1e; i++)
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)screen->data)[i]);
    for (i = 1; i < 0xb; i++)
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)screen->data)[i]);
}
