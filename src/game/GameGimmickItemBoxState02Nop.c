// bdc 0x088d78a4 GameGimmickItemBoxState02Nop
#include "bdc.h"

/* Empty state 2 handler of the item box gimmick (`GameGimmickItemBoxCtor`); does nothing. No
   item box code sets state 2 (`GameGimmickItemBoxStateBreak` returns to state 0), so it is
   unreachable in practice. */
void GameGimmickItemBoxState02Nop(CoreObject *obj)
{
    (void)obj;
}
