// bdc 0x08906e48 BtlDemoSceneRunEvents
#include "bdc.h"

/* Runs one frame of the loaded .scb scene of a battle demo scene player. For every event table of
   the scene (scene->events) it passes each event of six lists to its handler, in this order:
   lists[1] effects, lists[2] expire, lists[4] pose, lists[3] motion, lists[5] sound, lists[6]
   expire B. Per table it also plays sound 0x4300012 once for a specialId player, and when the
   frame has reached the table's frameCount (or frameCount is 0xffff and the frame is past 21) it
   sets `finished` and, unless suppressDemoEnd, the owning demo's end request. Then it applies the
   placement keys of every object on motionEvents for the current frame, copies the unit's y
   position into its groundY, and advances the frame. */

void BtlDemoSceneRunEvents(BtlDemoScenePlayer *player)
{
    BtlDemoScenePlayer *self = player;
    BtlDemoScbEventTable *table;
    BtlDemoScbEventTable *nextTable;
    CoreObject *ev;
    CoreObject *next;
    BtlBakugan *unit;

    table = self->scene->events.head;
    if (table != NULL) {
        nextTable = (BtlDemoScbEventTable *)table->base.next;
        for (;;) {
            ev = table->lists[1] != NULL ? table->lists[1]->head : NULL;
            for (; ev != NULL; ev = next) {
                next = ev->next;
                BtlDemoSceneEventEffect(self, ev);
            }
            ev = table->lists[2] != NULL ? table->lists[2]->head : NULL;
            for (; ev != NULL; ev = next) {
                next = ev->next;
                BtlDemoSceneEventExpire(self, (BtlDemoScbEvent *)ev);
            }
            ev = table->lists[4] != NULL ? table->lists[4]->head : NULL;
            for (; ev != NULL; ev = next) {
                next = ev->next;
                BtlDemoSceneEventSetPose(self, ev);
            }
            ev = table->lists[3] != NULL ? table->lists[3]->head : NULL;
            for (; ev != NULL; ev = next) {
                next = ev->next;
                BtlDemoSceneEventMotion(self, ev);
            }
            ev = table->lists[5] != NULL ? table->lists[5]->head : NULL;
            for (; ev != NULL; ev = next) {
                next = ev->next;
                BtlDemoSceneEventSound(self, ev);
            }
            ev = table->lists[6] != NULL ? table->lists[6]->head : NULL;
            for (; ev != NULL; ev = next) {
                next = ev->next;
                BtlDemoSceneEventExpireB(self, (BtlDemoScbEvent *)ev);
            }

            if (self->specialId != 0 && self->specialSoundPlayed == 0) {
                self->specialSoundPlayed = 1;
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0x4300012, 0, 0);
                }
            }

            /* Signed compares: frame is read as s32. */
            if ((s32)self->frame >= table->frameCount
                || (table->frameCount == 0xffff && (s32)self->frame > 21)) {
                if (self->suppressDemoEnd == 0) {
                    ((BtlDemo *)self->demo)->endRequest = 1;
                }
                self->finished = 1;
            }

            table = nextTable;
            if (table == NULL) {
                break;
            }
            nextTable = (BtlDemoScbEventTable *)table->base.next;
        }
    }

    for (ev = self->motionEvents.head; ev != NULL; ev = next) {
        next = ev->next;
        BtlDemoSceneObjPlaceKey((BtlDemoSceneMotionEvent *)ev, (s32)self->frame);
    }

    unit = self->unit;
    if (unit != NULL) {
        unit->groundY = unit->base.pos[1];
    }
    self->frame++;
}
