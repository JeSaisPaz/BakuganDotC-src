// bdc 0x089840e4 UiCollectionCardSetCardArt
#include "bdc.h"

/* Sets a card art sprite of `UiCollectionCard` to the large card texture
   `"card_L_%03d"` (card id + 1), or `"card_L_001"` for an empty slot (0xff). */
void UiCollectionCardSetCardArt(UiScreen *screen, GfxSprite *sprite, u8 cardId)
{
    char textureName[64];

    if (cardId == 0xff) {
        sprintf(textureName, "card_L_001");
    } else {
        sprintf(textureName, "card_L_%03d", cardId + 1);
    }
    sprite->texture = GfxFindTexture(textureName);
}
