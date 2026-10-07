// bdc 0x08af4a7c g_uiBakuganSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiBakuganSelectVtbl = {
    {0}, { .fn = (void *)UiBakuganSelectDtor }, { .fn = (void *)UiBakuganSelectUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiBakuganSelectDraw },
    { .fn = (void *)UiBakuganSelectSetField }, { .fn = (void *)UiBakuganSelectGetField },
};
