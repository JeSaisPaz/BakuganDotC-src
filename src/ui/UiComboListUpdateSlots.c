// bdc 0x089b3278 UiComboListUpdateSlots
#include "bdc.h"

/* Refreshes the attribute icons of combo `combo` of the combo-list screen (task 3002): for each of
   the four entries (`index` 0..3) of the combo's lists (`g_btlKindComboTables[combo]`) and each of
   its six icon positions (list 0 first/second, list 1 first/second, list 2, list 3), asks
   `UiComboListGetSlotAttr(lists, index, list, second)` for the attribute (2 = none); hides the icon
   sprite (sprite table `base.data`, indices 0x25 + 4 * position + index) and, for a real attribute,
   sets its cell to `g_uiComboListAttrCells[self->attrIcon[attr]]` (`GfxSpriteSetVCell`), sets its
   `posZ` to -5 and shows it. Called by `UiComboListCreateSprites`. */

static inline void UiComboListUpdateSlot(UiComboList *self, void **lists, s32 index, u32 list, u8 second,
                                  s32 sprite)
{
  u32 attr;
  GfxSprite *spr;

  attr = UiComboListGetSlotAttr(lists, index, list, second);
  spr = ((GfxSprite **)self->base.data)[sprite];
  spr->flags &= ~1u;
  if (attr != 2) {
    GfxSpriteSetVCell(g_uiComboListAttrCells[self->attrIcon[attr]],
                      ((GfxSprite **)self->base.data)[sprite]);
    ((GfxSprite **)self->base.data)[sprite]->posZ = -5.0f;
    spr = ((GfxSprite **)self->base.data)[sprite];
    spr->flags |= 1;
  }
}

void UiComboListUpdateSlots(UiComboList *self, s32 combo)
{
  void **lists;
  s32 index;

  lists = (void **)g_btlKindComboTables[combo];
  for (index = 0; index < 4; index++) {
    UiComboListUpdateSlot(self, lists, index, 0, 0, index + 0x25);
    UiComboListUpdateSlot(self, lists, index, 0, 1, index + 0x29);
    UiComboListUpdateSlot(self, lists, index, 1, 0, index + 0x2d);
    UiComboListUpdateSlot(self, lists, index, 1, 1, index + 0x31);
    UiComboListUpdateSlot(self, lists, index, 2, 0, index + 0x35);
    UiComboListUpdateSlot(self, lists, index, 3, 0, index + 0x39);
  }
}
