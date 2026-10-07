// bdc 0x08af6c30 g_btlAiParamSetSlot1Vtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_btlAiParamSetSlot1Vtbl = {
    {0}, { .fn = (void *)BtlAiParamSetSlot1Dtor }, { .fn = (void *)BtlAiParamSetLevel1DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel2DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel3DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel4DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel5DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel6DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel7DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel8DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel9DelayWeight },
    { .fn = (void *)BtlAiParamSetLevel10DelayWeight },
};
