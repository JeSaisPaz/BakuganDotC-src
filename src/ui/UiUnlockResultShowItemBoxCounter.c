// bdc 0x0893c02c UiUnlockResultShowItemBoxCounter
#include "bdc.h"

/* Shows or hides the "broken / total" item-box counter on the unlock-result screen (task 375,
   `UiUnlockResultCtor`): with `show` it counts the field's item-box gimmicks (type id 0xbdf,
   `GameGimmickItemBoxCtor`) and those already broken (inactive), writes both as two digits and
   makes the counter sprites visible; without `show` it hides them. Only for reward kinds (`+0x5ee`)
   other than 0/2/4/7/8/9 and only while bit 0 of save-profile word `0x34` is set. */

void UiUnlockResultShowItemBoxCounter(UiUnlockResult *self, bool show)
{
  GameGimmick *gimmick;
  s32 total;
  s32 broken;
  s32 i;

  switch (self->rewardKind) {
  case 0:
  case 2:
  case 4:
  case 7:
  case 8:
  case 9:
    break;
  default:
    if ((SaveProfileGetWord(SaveGetProfile(), 0x34) & 1) == 0) {
      break;
    }
    if (show) {
      /* digit sheet: 6 cells per row, digit d at column d / 6, row d % 6 */
      total = 0;
      broken = 0;
      for (gimmick = ((GameFieldTask *)GameFieldFindTask())->gimmicks; gimmick != NULL;
           gimmick = (GameGimmick *)gimmick->base.base.next) {
        if (gimmick->typeId == 0xbdf) {
          if (gimmick->active == 0) {
            broken++;
          }
          total++;
        }
      }
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[0x17], (float)((broken / 10) / 6),
                       (float)((broken / 10) % 6));
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[0x18], (float)((broken % 10) / 6),
                       (float)((broken % 10) % 6));
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[0x1a], (float)((total / 10) / 6),
                       (float)((total / 10) % 6));
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[0x1b], (float)((total % 10) / 6),
                       (float)((total % 10) % 6));
      for (i = 0x17; i < 0x1c; i++) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
      }
      ((GfxSprite **)self->base.data)[0x1e]->flags |= 1;
      ((GfxSprite **)self->base.data)[0x1e]->alpha = 1.0f;
    }
    else {
      for (i = 0x17; i < 0x1c; i++) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[0x1e]->flags &= ~1u;
    }
    break;
  }
}
