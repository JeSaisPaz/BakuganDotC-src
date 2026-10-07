// bdc 0x0894da04 UiBattleRecordStaticInit
#include "bdc.h"

/* Static constructor of the battle-record translation unit (`UiBattleRecordMenuPhase` ends right
   before it): sets the two fade colours used by `UiBattleRecordLoadPhase` /
   `UiBattleRecordFinishPhase` (`UiBattleRecordStartFade`): `g_battleRecordFadeColorOpaque` =
   `{0, 0, 0, 1}` and `g_battleRecordFadeColorClear` = `{0, 0, 0, 0}`. */

void UiBattleRecordStaticInit(void)
{
  g_battleRecordFadeColorOpaque[0] = 0.0f;
  g_battleRecordFadeColorOpaque[1] = 0.0f;
  g_battleRecordFadeColorOpaque[2] = 0.0f;
  g_battleRecordFadeColorOpaque[3] = 1.0f;
  g_battleRecordFadeColorClear[0] = 0.0f;
  g_battleRecordFadeColorClear[1] = 0.0f;
  g_battleRecordFadeColorClear[2] = 0.0f;
  g_battleRecordFadeColorClear[3] = 0.0f;
}
