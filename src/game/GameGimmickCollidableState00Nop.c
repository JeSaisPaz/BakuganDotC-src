// bdc 0x088d51d4 GameGimmickCollidableState00Nop
#include "bdc.h"

/* Empty state 0 (idle) handler of the breakable container gimmick (`GameGimmickCollidableCtor`);
   state 1 is `GameGimmickCollidableStateHit`. Does nothing. */
void GameGimmickCollidableState00Nop(CoreObject *obj)
{
    (void)obj;
}
