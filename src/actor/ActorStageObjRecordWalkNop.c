// bdc 0x088b2798 ActorStageObjRecordWalkNop
#include "bdc.h"

/* Walks the spawn-record list `0x08abd620` from the second entry without doing anything (debug code
   compiled out). Called from undisassembled jump-table cases of `BtlMainPhaseFinish` (`jal` at
   `0x08853cc8`). */

void ActorStageObjRecordWalkNop(void)

{
  void **node;

  if (g_stageObjRecordList != (void **)0x0 && *g_stageObjRecordList != (void *)0x0) {
    for (node = (void **)((void **)*g_stageObjRecordList)[1]; node != (void **)0x0;
         node = (void **)node[1]) {
    }
  }
}
