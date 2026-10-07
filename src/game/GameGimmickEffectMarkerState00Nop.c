// bdc 0x088daae4 GameGimmickEffectMarkerState00Nop
#include "bdc.h"

/* Empty state 0 handler, the only entry of the effect-marker gimmick's state table `0x08a96c50`
   dispatched by `GameGimmickEffectMarkerUpdate`. */
void GameGimmickEffectMarkerState00Nop(void *gimmick)
{
    (void)gimmick;
}
