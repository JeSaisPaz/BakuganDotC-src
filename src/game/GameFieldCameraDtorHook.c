// bdc 0x088b99c4 GameFieldCameraDtorHook
#include "bdc.h"

/* Empty function called at the start of GameFieldCameraDtor (an empty inline/base cleanup). */
void GameFieldCameraDtorHook(void *cam)
{
    (void)cam;
}
