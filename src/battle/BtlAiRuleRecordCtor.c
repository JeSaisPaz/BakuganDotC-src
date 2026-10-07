// bdc 0x08a2a978 BtlAiRuleRecordCtor
#include "bdc.h"

/* Element constructor of the 0x48-byte CPU AI rule records allocated by `BtlAiLoadComScript`
   through `CxxVecNew`: clears the mode mask and returns `rec`. */
BtlAiRuleRecord *BtlAiRuleRecordCtor(BtlAiRuleRecord *rec)
{
    rec->modeMask = 0;
    return rec;
}
