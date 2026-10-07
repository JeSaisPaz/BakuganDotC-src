// bdc 0x0894a160 UiBattleRecordSetModeRow
#include "bdc.h"

/* Fills visible row `row` of the per-mode list of `UiBattleRecord` with
   Bakugan `g_battleRecordBakuganOrder``[index]`: if the profile says the Bakugan is not owned
   (`owned[bakugan]` = 0) shows the `???` sprites (39/45 + row) and hides icon/name (27/33 + row);
   otherwise copies the icon texture of sprite `147 + bakugan`, sets the name cell `bakugan`
   (`GfxSpriteSetCell`) and hides the `???` sprites. Both then fill the four stat cells
   (`UiBattleRecordSetModeCell` 0–3). The layout sprite list is `base.data`. */

void UiBattleRecordSetModeRow(UiBattleRecord *self, s32 row, s32 index)
{
  s32 bakugan = g_battleRecordBakuganOrder[index];
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *icon = sprites[27 + row];

  if (self->owned[bakugan] == 0) {
    icon->flags &= ~1u;
    ((GfxSprite **)self->base.data)[33 + row]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[39 + row]->flags |= 1;
    ((GfxSprite **)self->base.data)[45 + row]->flags |= 1;
  } else {
    icon->texture = sprites[147 + bakugan]->texture;
    ((GfxSprite **)self->base.data)[27 + row]->flags |= 1;
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[33 + row], 0.0f, (float)bakugan);
    ((GfxSprite **)self->base.data)[33 + row]->flags |= 1;
    ((GfxSprite **)self->base.data)[39 + row]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[45 + row]->flags &= ~1u;
  }
  UiBattleRecordSetModeCell(self, row, bakugan, 0);
  UiBattleRecordSetModeCell(self, row, bakugan, 1);
  UiBattleRecordSetModeCell(self, row, bakugan, 2);
  UiBattleRecordSetModeCell(self, row, bakugan, 3);
}
