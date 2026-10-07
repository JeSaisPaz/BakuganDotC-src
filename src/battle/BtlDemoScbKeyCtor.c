// bdc 0x08a2d1b0 BtlDemoScbKeyCtor
#include "bdc.h"

/* Trivial element constructor of the 8-byte `{exponent, value}` keys of a `.scb` key-track event:
   returns `key` unchanged. */
void *BtlDemoScbKeyCtor(void *key)
{
    return key;
}
