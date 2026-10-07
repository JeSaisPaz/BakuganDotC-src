// bdc 0x088b6a00 BtlItemApplyPickup
#include "bdc.h"

/* Applies a picked-up item to the unit that took it (`item->picker`): plays pickup sound 0x200190 at
   the unit's anchor (`anchorMatrix[3]`), spawns effect 0x52 at the item, flashes the unit white
   (strength 0.9), and computes a duration multiplier (1.0, +0.5 with upgrade 0x15, +1.0 with
   upgrade 0x16). Then by `item->type`:
   0: when the talk/HUD task exists, starts its item notice and counts the pickup;
   1: stops buff effects, clears statuses 2/4/0x11 and their visual bits, attaches effect 0x57,
      applies status 0 for 450*mult frames, sound 0x200192 (+ local-player jingle 0x5100001);
   2: same with statuses 0/4/0x11 cleared, effect 0x58, status 2 for 600*mult, sound 0x200194
      (+ jingle 0x5100002);
   3: attaches effect 0x5b, clears statuses 0/2/4 and bits, heals 10% of max HP, status 0x11 for
      90*mult, sound 0x200191 (+ jingle 0x5100003);
   4: stops buff effects, clears 0/2/0x11, attaches effect 0x5a, status 4 for 300 frames, sound
      0x200193;
   5: only counts the pickup and plays sound 0x200192;
   other types do nothing. Types 1..5 bump `itemPickupCount`. An attached effect gets the unit as
   owner (`ownerBakugan`, `ownerId`). */

void BtlItemApplyPickup(BtlItem *item)
{
  GfxEffect *effect;
  BtlCombatState *combat;
  float mult;
  float hp;

  mult = 1.0f;
  effect = NULL;
  BtlItemPlaySound(item, 0x200190, item->picker->anchorMatrix[3]);
  GfxEffectSpawn(g_btlItemEffectMgr, 0x52, item->pos);
  BtlBakuganStartColorFlash(0.9f, item->picker, (float *)&g_colorWhite);
  if (item->picker != NULL) {
    if (BtlCombatHasUpgrade(&item->picker->combat, 0x15) != 0) {
      mult = 1.0f + 0.5f;
    }
    if (BtlCombatHasUpgrade(&item->picker->combat, 0x16) != 0) {
      mult = mult + 1.0f;
    }
  }
  switch ((u32)item->type) {
  case 0:
    if (UiTalkTaskExists() != 0) {
      void *win = UiGetTalkTask();

      UiTalkStartItemNotice(win, item->picker);
      item->picker->itemPickupCount++;
    }
    break;
  case 1:
    BtlBakuganStopBuffEffects(item->picker);
    BtlCombatClearStatus(&item->picker->combat, 2);
    BtlCombatClearStatus(&item->picker->combat, 4);
    BtlCombatClearStatus(&item->picker->combat, 0x11);
    item->picker->statusVisualBits &= ~0x20014u;
    effect = GfxEffectSpawnAttached(g_btlItemEffectMgr, 0x57, item->picker->anchorMatrix[3]);
    BtlCombatApplyStatus(&item->picker->combat, 0, (int)(mult * 450.0f));
    item->picker->itemPickupCount++;
    BtlItemPlaySound(item, 0x200192, item->picker->anchorMatrix[3]);
    if (BtlBakuganIsLocalPlayer(item->picker) && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x5100001, 0, 0);
    }
    break;
  case 2:
    BtlBakuganStopBuffEffects(item->picker);
    BtlCombatClearStatus(&item->picker->combat, 0);
    BtlCombatClearStatus(&item->picker->combat, 4);
    BtlCombatClearStatus(&item->picker->combat, 0x11);
    item->picker->statusVisualBits &= ~0x20011u;
    effect = GfxEffectSpawnAttached(g_btlItemEffectMgr, 0x58, item->picker->anchorMatrix[3]);
    BtlCombatApplyStatus(&item->picker->combat, 2, (int)(mult * 600.0f));
    item->picker->itemPickupCount++;
    BtlItemPlaySound(item, 0x200194, item->picker->anchorMatrix[3]);
    if (BtlBakuganIsLocalPlayer(item->picker) && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x5100002, 0, 0);
    }
    break;
  case 3:
    effect = GfxEffectSpawnAttached(g_btlItemEffectMgr, 0x5b, item->picker->anchorMatrix[3]);
    BtlCombatClearStatus(&item->picker->combat, 0);
    BtlCombatClearStatus(&item->picker->combat, 2);
    BtlCombatClearStatus(&item->picker->combat, 4);
    item->picker->statusVisualBits &= ~0x15u;
    combat = &item->picker->combat;
    hp = BtlCombatGetHp(combat);
    /* max HP converted as unsigned (cvt.s.w plus 2^32 when negative) */
    BtlCombatSetHp(hp + (float)(u32)BtlCombatGetMaxHp(combat) * 0.1f, combat);
    BtlCombatApplyStatus(&item->picker->combat, 0x11, (int)(mult * 90.0f));
    item->picker->itemPickupCount++;
    BtlItemPlaySound(item, 0x200191, item->picker->anchorMatrix[3]);
    if (BtlBakuganIsLocalPlayer(item->picker) && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x5100003, 0, 0);
    }
    break;
  case 4:
    BtlBakuganStopBuffEffects(item->picker);
    BtlCombatClearStatus(&item->picker->combat, 0);
    BtlCombatClearStatus(&item->picker->combat, 2);
    BtlCombatClearStatus(&item->picker->combat, 0x11);
    effect = GfxEffectSpawnAttached(g_btlItemEffectMgr, 0x5a, item->picker->anchorMatrix[3]);
    BtlCombatApplyStatus(&item->picker->combat, 4, 300);
    item->picker->itemPickupCount++;
    BtlItemPlaySound(item, 0x200193, item->picker->anchorMatrix[3]);
    break;
  case 5:
    item->picker->itemPickupCount++;
    BtlItemPlaySound(item, 0x200192, item->picker->anchorMatrix[3]);
    break;
  default:
    break;
  }
  if (effect != NULL) {
    BtlBakugan *owner = item->picker;

    effect->ownerBakugan = owner;
    if (owner != NULL) {
      effect->ownerId = owner->base.base.id;
    }
  }
}
