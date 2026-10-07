// bdc 0x088d93bc GameGimmickGetRecordParam
#include "bdc.h"

/* Returns the u16 at `+0x24` of the gimmick's layout record (`*(gimmick + 0x170)`). Used by the
   field trigger/input code. */

u16 GameGimmickGetRecordParam(GameGimmick *gimmick)

{
  return ((GameGimmickRecord *)gimmick->record)->param;
}
