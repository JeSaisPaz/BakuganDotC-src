// bdc 0x0893a3c4 UiUnlockResultShowPieces
#include "bdc.h"

/* Shows (hide == 0) or pops out the piece sprites of the unlock-result screen (task 375,
   `UiUnlockResultCtor`) for reward kinds 2 and 5; other kinds do nothing.
   Kind 2: for each of the `digitCount` digits (stored in `digits` in reverse order, filled by
   `UiUnlockResultInitDigits`) sets the cell of sprite 8+i to (digit / 5, digit % 5)
   (`GfxSpriteSetCell`), makes it visible and starts its pop (`UiUnlockResultBeginPop`).
   Kind 5 (Maxus parts): counts 1 + the owned bits 1..6 of the profile's `specialBits` (byte offset
   `g_rewardMaxusSet`) and requests the flash when that count is 6; adds 1 more when the reward's
   bit (profile word 0x14 + 1) is not yet set, shows the count as the cell of sprite 15, shows
   sprites 16 and 17 (cell (1, 2)) and pops sprites 15..17.
   With `hide` set only the pops are started, in closing mode. */

void UiUnlockResultShowPieces(UiUnlockResult *self, s8 hide)

{
  u8 kind;
  s32 maxusSet;
  SaveProfile *profile;
  GfxSprite *sprite;
  s32 count;
  s32 bit;
  s32 i;
  u32 digit;

  kind = self->rewardKind;
  if (hide == 0) {
    if (kind == 2) {
      for (i = 0; i < (s32)self->digitCount; i++) {
        digit = self->digits[self->digitCount - 1 - i];
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[8 + i], (float)((s32)digit / 5),
                         (float)((s32)digit % 5));
        sprite = ((GfxSprite **)self->base.data)[8 + i];
        sprite->flags |= 1;
        UiUnlockResultBeginPop(self, (u8)hide, (u8)(i + 8));
      }
    }
    else if (kind == 5) {
      maxusSet = g_rewardMaxusSet;
      count = 1;
      bit = 0;
      do {
        profile = SaveGetProfile();
        bit++;
        if ((profile->data->specialBits[maxusSet + bit / 8] & (1 << (bit % 8))) != 0) {
          count++;
        }
      } while (bit < 6);
      if (count == 6) {
        self->flashRequested = 1;
      }
      profile = SaveGetProfile();
      bit = (s32)SaveProfileGetWord(SaveGetProfile(), 0x14) + 1;
      if ((profile->data->specialBits[maxusSet + bit / 8] & (1 << (bit % 8))) == 0) {
        count++;
      }
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[15], (float)(count / 5),
                       (float)(count % 5));
      ((GfxSprite **)self->base.data)[15]->flags |= 1;
      ((GfxSprite **)self->base.data)[16]->flags |= 1;
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[17], 1.0f, 2.0f);
      ((GfxSprite **)self->base.data)[17]->flags |= 1;
      UiUnlockResultBeginPop(self, (u8)hide, 15);
      UiUnlockResultBeginPop(self, (u8)hide, 16);
      UiUnlockResultBeginPop(self, (u8)hide, 17);
    }
  }
  else if (kind == 2) {
    for (i = 0; i < (s32)self->digitCount; i++) {
      UiUnlockResultBeginPop(self, (u8)hide, (u8)(i + 8));
    }
  }
  else if (kind == 5) {
    UiUnlockResultBeginPop(self, (u8)hide, 15);
    UiUnlockResultBeginPop(self, (u8)hide, 16);
    UiUnlockResultBeginPop(self, (u8)hide, 17);
  }
  return;
}
