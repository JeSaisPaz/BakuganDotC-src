// bdc 0x088d6508 GameGimmickCollectionBoxStaticInit
#include "bdc.h"

/* Static initialiser (entry 20 of the static-constructor table `0x08af5bbc`) of the collection-box
   gimmick: sets the vec4 at `0x08abf050` to `{3.8, 5.9678, 7.3593, 0}`, read by
   `GameGimmickCollectionBoxCtor`. */

void GameGimmickCollectionBoxStaticInit(void)

{
  g_gameGimmickCollectionBoxTriggerPos.x = 3.8f;
  g_gameGimmickCollectionBoxTriggerPos.y = 5.96779f;
  g_gameGimmickCollectionBoxTriggerPos.z = 7.35933f;
  g_gameGimmickCollectionBoxTriggerPos.w = 0.0f;
  return;
}

