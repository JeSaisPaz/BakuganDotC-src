// bdc 0x088f4768 GameFieldCharSetClearForDtor
#include "bdc.h"

/* Calls `GameFieldCharSetClear`; used by `GameFieldDtor`. */
void GameFieldCharSetClearForDtor(void *mgr)
{
    GameFieldCharSetClear(mgr);
}
