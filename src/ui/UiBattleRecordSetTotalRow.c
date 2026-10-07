// bdc 0x0894b748 UiBattleRecordSetTotalRow
#include "bdc.h"

/* Fills visible row `row` of the totals list of `UiBattleRecord` with Bakugan
   `g_battleRecordBakuganOrder``[index]`: unowned Bakugan show `???` (sprites 33/39 + row
   visible, icon/name hidden), owned ones get the icon texture of sprite `81 + bakugan`, the name
   cell `bakugan` of the name sheet (`GfxSpriteSetCell`) and hide the `???` sprites; both then
   write the totals (`UiBattleRecordSetTotalCell`). The layout sprite list is `base.data`. */

void UiBattleRecordSetTotalRow(UiBattleRecord *self, s32 row, s32 index)
{
  s32 bakugan = g_battleRecordBakuganOrder[index];
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *icon = sprites[21 + row];

  if (self->owned[bakugan] == 0) {
    icon->flags &= ~1u;
    ((GfxSprite **)self->base.data)[27 + row]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[33 + row]->flags |= 1;
    ((GfxSprite **)self->base.data)[39 + row]->flags |= 1;
  } else {
    icon->texture = sprites[81 + bakugan]->texture;
    ((GfxSprite **)self->base.data)[21 + row]->flags |= 1;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[27 + row], 0.0f, (float)bakugan);
    ((GfxSprite **)self->base.data)[27 + row]->flags |= 1;
    ((GfxSprite **)self->base.data)[33 + row]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[39 + row]->flags &= ~1u;
  }
  UiBattleRecordSetTotalCell(self, row, bakugan);
}
