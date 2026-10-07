// bdc 0x088f4b90 GameFieldCharSetClearFaces
#include "bdc.h"

/* Clears the face expression of every placed actor (`ActorClearFaceExpression`). */

void GameFieldCharSetClearFaces(void *mgr)
{
    GameFieldCharSet *set = (GameFieldCharSet *)mgr;
    Actor **actors = (Actor **)mgr;
    u32 i = 0;

    do {
        ActorClearFaceExpression(actors[i]);
        i = (i + 1) & 0xff;
    } while ((s32)i < set->placedCount);
}
