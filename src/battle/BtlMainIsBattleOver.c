// bdc 0x08a29864 BtlMainIsBattleOver
#include "bdc.h"

/* Returns the battle-over flag `g_btlBattleOver`: set to 1 by `BtlMainPhaseBattle` once the battle
   result becomes non-zero (just before `BtlEndCutIn` and the result demo), cleared
   by `BtlMainTaskCtor` and `BtlMainTeardown`. */

u8 BtlMainIsBattleOver(void)

{
  return g_btlBattleOver;
}

