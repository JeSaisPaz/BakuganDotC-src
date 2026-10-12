// bdc 0x08878cd8 BtlAttackSystemInit
#include "bdc.h"

/* Empties the battle attack list (head, tail, count), stores the effect
   manager attacks spawn into, and creates the attacks' shared 8-slot sound
   object on the sound object list, positioned at the origin. */
void BtlAttackSystemInit(void *effectMgr)
{
    g_btlAttackEffectMgr = effectMgr;
    g_btlAttackList.tail = NULL;
    g_btlAttackList.head = NULL;
    g_btlAttackList.count = 0;
    g_btlAttackSndObject = SndObjectCreate(SndGetObjectList(), 8);
    SndObjectSetPos((SndObject *)g_btlAttackSndObject, 0.0f, 0.0f, 0.0f);
}
