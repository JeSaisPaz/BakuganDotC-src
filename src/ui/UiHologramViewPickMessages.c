// bdc 0x0892a1b8 UiHologramViewPickMessages
#include "bdc.h"

/* Chooses the two message ids of the hologram view screen (`UiHologramViewCtor`) in `pageIds[0]`
   and `pageIds[1]` (both default 0xff) from the story stage (script global 1,
   `g_scriptGlobalVars``[1]`) and the player profile:
   - stage 0 or 0xd: 0x12; stage 1: 0xe when save word 0x1c is 1, else 0x12 (flag bits 2 and 4 of
     profile word 0x30 cleared);
   - otherwise the stage's recommendation (`GameStageGetRecommendation`: low byte Bakugan id, high
     byte attribute) and info record (`GameStageGetInfo`) are read. When neither the recommended
     Bakugan nor its pair (`UiBakuganGetPair`) is the current Bakugan (`curBakugan`): 0x21, plus a
     per-Bakugan follow-up 0x13..0x18 in `pageIds[1]` if flag bit 2 was already set (else it is set
     now); flag bit 4 is cleared;
   - otherwise bit 2 is cleared; if the info record's last byte is nonzero: no hologram placed ->
     0x1a; no placed hologram of the recommended attribute (`UiHologramGalleryMapAttribute`) ->
     0x22, plus an attribute follow-up 0x1b..0x20 if flag bit 4 was already set (else it is set now);
   - otherwise bit 4 is cleared and the message is 0xd unless all 6 upgrades of the current Bakugan
     are owned, then 0xe / 0x12 as for stage 1. */

void UiHologramViewPickMessages(UiHologramView *self)
{
  s32 stage;
  s16 rec;
  u8 bakugan;
  u8 attr;
  u8 pair;
  u8 placed;
  u16 info[10];
  u16 id;
  s32 count;
  s32 i;
  SaveProfile *p;
  SaveProfile *q;

  self->pageIds[0] = 0xff;
  self->pageIds[1] = 0xff;
  stage = g_scriptGlobalVars[1];
  if (stage == 0 || stage == 0xd) {
    self->pageIds[0] = 0x12;
    SaveProfileModifyWord30Bits(0, 2);
    SaveProfileModifyWord30Bits(0, 4);
    return;
  }
  if (stage == 1) {
    id = 0x12;
    if (SaveProfileGetWord(SaveGetProfile(), 0x1c) == 1) {
      id = 0xe;
    }
    self->pageIds[0] = id;
    SaveProfileModifyWord30Bits(0, 2);
    SaveProfileModifyWord30Bits(0, 4);
    return;
  }

  rec = (s16)GameStageGetRecommendation((u8)(stage / 4), (u8)(stage % 4));
  stage = g_scriptGlobalVars[1];
  bakugan = (u8)rec;
  attr = (u8)((u16)rec >> 8);
  GameStageGetInfo(info, (u8)(stage / 4), (u8)(stage % 4));
  pair = UiBakuganGetPair(0, bakugan);

  if (bakugan != SaveGetProfile()->data->curBakugan &&
      pair != SaveGetProfile()->data->curBakugan) {
    self->pageIds[0] = 0x21;
    if (SaveProfileTestWord30Bits(2) == true) {
      switch (bakugan) {
      case 1: self->pageIds[1] = 0x13; break;
      case 3: self->pageIds[1] = 0x18; break;
      case 5: self->pageIds[1] = 0x14; break;
      case 6: self->pageIds[1] = 0x15; break;
      case 7: self->pageIds[1] = 0x16; break;
      case 8: self->pageIds[1] = 0x17; break;
      default: break;
      }
    } else {
      SaveProfileModifyWord30Bits(1, 2);
    }
    SaveProfileModifyWord30Bits(0, 4);
    return;
  }

  SaveProfileModifyWord30Bits(0, 2);
  if ((u8)(info[9] >> 8) != 0) {
    count = 0;
    for (i = 0; i < 4; i++) {
      if (SaveGetProfile()->data->placedHolograms[(u8)i] != 0) {
        count++;
      }
    }
    if (count == 0) {
      self->pageIds[0] = 0x1a;
      SaveProfileModifyWord30Bits(0, 4);
      return;
    }
    count = 0;
    for (i = 0; i < 4; i++) {
      if (SaveGetProfile()->data->placedHolograms[(u8)i] != 0) {
        placed = SaveGetProfile()->data->placedHolograms[(u8)i];
        if (attr == (u8)UiHologramGalleryMapAttribute(true, (u8)(((s32)placed - 0xe) / 3))) {
          count++;
        }
      }
    }
    if (count == 0) {
      self->pageIds[0] = 0x22;
      if (SaveProfileTestWord30Bits(4) == true) {
        switch (attr) {
        case 0: self->pageIds[1] = 0x1b; break;
        case 1: self->pageIds[1] = 0x1c; break;
        case 2: self->pageIds[1] = 0x1f; break;
        case 3: self->pageIds[1] = 0x1d; break;
        case 4: self->pageIds[1] = 0x1e; break;
        case 5: self->pageIds[1] = 0x20; break;
        default: break;
        }
      } else {
        SaveProfileModifyWord30Bits(1, 4);
      }
      return;
    }
  }

  SaveProfileModifyWord30Bits(0, 4);
  count = 0;
  for (i = 0; i < 6; i++) {
    p = SaveGetProfile();
    q = SaveGetProfile();
    if (p->data->upgradeOwned[q->data->curBakugan][i] == 1) {
      count++;
    }
  }
  if (count < 6) {
    self->pageIds[0] = 0xd;
    return;
  }
  id = 0x12;
  if (SaveProfileGetWord(SaveGetProfile(), 0x1c) == 1) {
    id = 0xe;
  }
  self->pageIds[0] = id;
}
