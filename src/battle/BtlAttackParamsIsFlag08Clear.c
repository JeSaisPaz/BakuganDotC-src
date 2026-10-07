// bdc 0x0888c088 BtlAttackParamsIsFlag08Clear
#include "bdc.h"

/* Returns 1 when attack type `type` has an entry in `g_btlAttackParamTable` and its byte `+0x08`
   (`BtlAttackParams``.noGuardFlag`) is 0, else 0. */

int BtlAttackParamsIsFlag08Clear(int type)

{
  BtlAttackParams *params = BtlAttackParamsGet(type);

  return params != NULL && params->noGuardFlag == 0;
}
