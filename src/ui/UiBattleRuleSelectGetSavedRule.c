// bdc 0x089527c4 UiBattleRuleSelectGetSavedRule
#include "bdc.h"

/* Returns profile word 7 (the battle rule chosen last time, `SaveProfileGetWord`) as a signed
   byte. */

s32 UiBattleRuleSelectGetSavedRule(void)
{
  return (s8)SaveProfileGetWord(SaveGetProfile(), 7);
}
