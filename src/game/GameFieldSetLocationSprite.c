// bdc 0x088c21d8 GameFieldSetLocationSprite
#include "bdc.h"

/* Replaces the location label sprite `locationLabel` of the field task (id 500, `GameFieldCtor`)
   with layout record `0x31 + index` of table 0xc. Indices 5 and 6 use records 0x41/0x42 instead,
   show the marker sprite `locationMarker` (flags bit 0) and set `pulseActive`, which restarts the
   pulse (`GameFieldPulseSpriteReset`) once the new label exists; other indices hide the marker and
   clear `pulseActive`. The old label is released from `spriteLayer` (`UiSpriteLayerRelease`) and
   the new one (`GameFieldCreateLayoutSprite`) gets flags bit 0 set. */

void GameFieldSetLocationSprite(CoreTask *task, u8 index)
{
  GameFieldTask *field = (GameFieldTask *)task;
  u32 i = index;
  s16 *record;
  GfxSprite *old;
  GfxSprite *label;

  if (i == 5) {
    record = UiLayoutGetEntry(0xc, 0x41);
    field->locationMarker->flags |= 1;
    field->pulseActive = 1;
  } else if (i == 6) {
    record = UiLayoutGetEntry(0xc, 0x42);
    field->locationMarker->flags |= 1;
    field->pulseActive = 1;
  } else {
    record = UiLayoutGetEntry(0xc, i + 0x31);
    field->locationMarker->flags &= ~1u;
    field->pulseActive = 0;
  }
  old = field->locationLabel;
  if (old != NULL) {
    UiSpriteLayerRelease(field->spriteLayer, old);
  }
  label = GameFieldCreateLayoutSprite(task, record);
  field->locationLabel = label;
  label->flags |= 1;
  if (field->pulseActive != 0) {
    GameFieldPulseSpriteReset(task);
  }
}
