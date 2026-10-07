// bdc 0x08969fa0 UiCardEquipSetTabTexture
#include "bdc.h"

/* Sets the texture of a tab button sprite of `UiCardEquip` by tab index and
   highlight state: `"c_setting_bo_2"`/`"c_setting_bo_1_b"` for plain tabs,
   `"cha_bo_maru_ita_4"`/`"_5"` and `"cha_bo_maru_ita_2"`/`"_1"` for the round buttons; the mapping
   depends on the small/large layout. An out-of-range tab leaves the name buffer unwritten and the
   lookup runs on stack garbage, as in the original. */

void UiCardEquipSetTabTexture(UiCardEquip *self, GfxSprite *sprite, u8 highlighted, u8 tab)
{
  char name[64];

  if (self->bakuganCount < 3) {
    switch (tab) {
    case 0:
    case 3:
      sprintf(name, highlighted ? "c_setting_bo_1_b" : "c_setting_bo_2");
      break;
    case 1:
      sprintf(name, highlighted ? "cha_bo_maru_ita_5" : "cha_bo_maru_ita_4");
      break;
    case 2:
      sprintf(name, highlighted ? "cha_bo_maru_ita_1" : "cha_bo_maru_ita_2");
      break;
    }
  } else {
    switch (tab) {
    case 0:
    case 1:
    case 4:
    case 5:
      sprintf(name, highlighted ? "c_setting_bo_1_b" : "c_setting_bo_2");
      break;
    case 2:
      sprintf(name, highlighted ? "cha_bo_maru_ita_5" : "cha_bo_maru_ita_4");
      break;
    case 3:
      sprintf(name, highlighted ? "cha_bo_maru_ita_1" : "cha_bo_maru_ita_2");
      break;
    }
  }
  sprite->texture = GfxFindTexture(name);
}
