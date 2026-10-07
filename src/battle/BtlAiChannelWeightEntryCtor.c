// bdc 0x08a2a1d0 BtlAiChannelWeightEntryCtor
#include "bdc.h"

/* Element constructor of the ten weight entries of an AI command channel (`BtlAiChannelCtor`):
   clears the weight pointer, count, total and both flags; returns `self`. */
BtlAiWeightTable *BtlAiChannelWeightEntryCtor(BtlAiWeightTable *self)
{
    self->weights = NULL;
    self->count = 0;
    self->total = 0;
    self->totalValid = 0;
    self->borrowed = 0;
    return self;
}
