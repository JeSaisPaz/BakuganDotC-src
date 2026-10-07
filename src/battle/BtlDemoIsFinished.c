// bdc 0x088feb08 BtlDemoIsFinished
#include "bdc.h"

/* Returns `g_btlDemoFinished` while the battle intro demo task (id 0x65) exists, else 1. */
bool BtlDemoIsFinished(void)
{
    u8 finished = 1;

    if (CoreTaskExists(0x65) != 0) {
        finished = g_btlDemoFinished;
    }
    return (bool)finished;
}
