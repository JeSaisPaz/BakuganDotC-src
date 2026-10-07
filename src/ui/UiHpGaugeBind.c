// bdc 0x08889e20 UiHpGaugeBind
#include "bdc.h"

/* Binds the HUD hit-point gauge to its data source and primes it. `mode` is stored at `+0x8c`: mode
   1 reads a battle unit (`+0x28 = +0x24`, HP from the unit's `BtlCombatState` at `unit + 0x434`),
   mode 2 reads a non-unit object (`+0x2c = +0x24`, integer HP fields at `+0x200` current / `+0x204`
   max). Then `+0x70 = ``UiHpGaugeGetMaxHp`, `+0x74/+0x78/+0x7c = ``UiHpGaugeGetHp` (the three
   displayed values start equal), `+0x80/+0x84 = 1.0`, `+0x88/+0x90/+0x91/+0x94/+0x98 = 0`, appends
   the node to the draw group `0x08b00890` (`CoreNodeGroupAppend`) and looks up the sprite texture
   `"tairyoku_01_gauge"` (`GfxFindTexture`, stored at `+0x30`). */

void UiHpGaugeBind(UiHpGauge *self, int mode)

{
  void *texture;
  float hp;
  
  self->mode = mode;
  if (mode < 2) {
    if (0 < mode) {
      self->object = (void *)0x0;
      self->unit = self->source;
    }
  }
  else if (mode < 3) {
    self->unit = (BtlBakugan *)0x0;
    self->object = self->source;
  }
  hp = UiHpGaugeGetMaxHp(self);
  self->maxHp = hp;
  hp = UiHpGaugeGetHp(self);
  self->hp = hp;
  self->hpTrail = hp;
  self->hpBeforeHit = hp;
  self->hitTimer = 0.0;
  self->fade = 1.0;
  self->alpha = 1.0;
  self->updated = '\0';
  self->forceOpaque = '\0';
  self->lowHpFlash = 0.0;
  self->lowHpPhase = 0.0;
  CoreNodeGroupAppend(&self->base,&g_uiHpGaugeGroup);
  texture = GfxFindTexture("tairyoku_01_gauge");
  self->texture = texture;
  return;
}

