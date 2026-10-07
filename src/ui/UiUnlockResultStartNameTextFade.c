// bdc 0x0893ad88 UiUnlockResultStartNameTextFade
#include "bdc.h"

/* For rewards with a name (reward kind `+0x5ee` 1, 2, 6, 8, 9) on
   `UiUnlockResult`: starts the fade of the name text (t `+0x77c` = 0, alpha
   `+0x740`/start `+0x748` = 0 when opening, 1 when closing) and marks it dirty (`+0x778`). */

void UiUnlockResultStartNameTextFade(UiUnlockResult *self, u8 closing)
{
    switch (self->rewardKind) {
    case 1:
    case 2:
    case 6:
    case 8:
    case 9:
        break;
    default:
        return;
    }
    if (closing == 0) {
        self->textAlpha[0] = 0.0f;
        self->textFadeBase[0] = 0.0f;
        self->nameFadeT = 0.0f;
    } else {
        self->nameFadeT = 0.0f;
        self->textAlpha[0] = 1.0f;
        self->textFadeBase[0] = 1.0f;
    }
    self->textVisible[0] = 1;
}
