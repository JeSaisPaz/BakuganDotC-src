// bdc 0x0884b630 BtlCheckEnemiesDefeated
#include "bdc.h"

/* Returns 1 when every opponent of the player is defeated, else 0. Opponents are the units of
   `BtlGetBakuganList` other than `BtlGetPlayerBakugan` whose virtual slot 14 returns 0 or whose
   slot 19 returns 1; each one still alive (`combat.dead` clear) is stored in `g_btlFinishUnit`.
   With no opponents the result is 1. When all are defeated and `startCinematic` is set, a NULL
   `g_btlFinishUnit` becomes the player unit and the finish cinematic starts on it
   (`BtlStartFinishCinematic``(main, unit, 0, 0)`). */

int BtlCheckEnemiesDefeated(void *main, bool startCinematic)
{
    CoreObject **head = BtlGetBakuganList();
    CoreObject *player = BtlGetPlayerBakugan();
    CoreObject *obj;
    int opponents = 0;
    int defeated = 0;

    for (obj = *head; obj != NULL; obj = obj->next) {
        const VtblEntry *slot14 = &((const VtblEntry *)obj->vtable)[14];
        const VtblEntry *slot19;

        if (((int (*)(void *))slot14->fn)((u8 *)obj + slot14->delta) != 0) {
            slot19 = &((const VtblEntry *)obj->vtable)[19];
            if (((int (*)(void *))slot19->fn)((u8 *)obj + slot19->delta) != 1) {
                continue;
            }
        }
        if (obj == player) {
            continue;
        }
        if (((BtlBakugan *)obj)->combat.dead != 0) {
            defeated++;
        } else {
            g_btlFinishUnit = obj;
        }
        opponents++;
    }
    if (opponents != defeated) {
        return 0;
    }
    if (startCinematic) {
        if (g_btlFinishUnit == NULL) {
            g_btlFinishUnit = player;
        }
        BtlStartFinishCinematic(main, g_btlFinishUnit, 0, 0);
    }
    return 1;
}
