// bdc 0x08906474 BtlDemoSceneEventMotion
#include "bdc.h"

/* Motion event handler of the `.scb` scene player (`BtlDemoScenePlayer`). Special case first:
   demo 0x52 at frame 0x7d plays motion 0x126 on the player's `unit` (`BtlBakuganPlayMotion`,
   blend 0.1). Then, only when the event's frame equals the player's frame, it looks up the event's
   scene object (`BtlDemoSceneFindObject` on `groupWord`) and picks the target by the object's
   `kind`: 2 = `actor`, 3 = `unit`, 4 = `follower`, else none. By the event's sub-type (`flags`):
   0 sets the target's `ambient[3]` to `flag0 / 255`; 2 queues a `BtlDemoSceneMotionEvent`
   (track `range2A` clamped to 0..7, overridden per demo id, some demo ids also shifting the actor's
   position / heading); 3 plays motion `motionParam` on the target (`ActorPlayMotion` /
   `BtlBakuganPlayMotion` / `ActorBallPlayMotion`, blend `blendTime`, loop `motionFlag`,
   forced); 1, 4 and others do nothing. On a frame match the event then deletes itself (vtable
   slot 1, flag 3). */

void BtlDemoSceneEventMotion(BtlDemoScenePlayer *player, void *event)
{
    BtlDemoScbMotionEvent *ev = (BtlDemoScbMotionEvent *)event;
    BtlDemoScbObject *obj;
    BtlDemoSceneMotionEvent *node;
    GfxModel *target;
    Actor *actor;
    BtlBakugan *unit;
    GfxModel *ball;
    s32 kind;
    s32 trackId;
    u16 param;
    bool fromLow;

    if (player->demoId == 0x52 && player->frame == 0x7d && player->unit != NULL) {
        BtlBakuganPlayMotion(0.1f, (BtlBakugan *)player->unit, 0x126, 1, 1);
    }
    if (ev->base.frame != player->frame) {
        return;
    }

    kind = 0;
    obj = (BtlDemoScbObject *)BtlDemoSceneFindObject((void **)&player->scene->objects,
                                                     ev->base.groupWord);
    if (obj != NULL) {
        kind = obj->kind;
    }
    target = NULL;
    if (kind == 2) {
        target = &player->actor->base;
    } else if (kind == 3) {
        target = (GfxModel *)player->unit;
    } else if (kind == 4) {
        target = player->follower;
    }

    switch (ev->base.flags) {
    case 0:
        if (target != NULL) {
            target->ambient[3] = (float)ev->flag0 * 0.003921569f;
        }
        break;
    case 2:
        trackId = ev->range2A;
        if (trackId < 0) {
            trackId = 0;
        } else if (trackId > 7) {
            trackId = 7;
        }
        switch (player->demoId) {
        case 0x19: case 0x21: case 0x25: case 0x29: case 0x2d: case 0x31: case 0x35:
        case 0x39: case 0x41: case 0x45: case 0x49: case 0x4d: case 0x51: case 0x55:
        case 0x59: case 0x5d: case 0x61: case 0x65:
            trackId = 1;
            break;
        case 0x33:
            trackId = 1;
            break;
        case 0x5b:
            if (kind == 3) {
                trackId = 0;
            } else {
                player->actor->base.pos[0] += 26.0f;
                trackId = 4;
                player->actor->base.pos[2] += 64.0f;
            }
            break;
        case 0x5f:
            if (kind == 2) {
                player->actor->base.pos[0] += -8.0f;
                trackId = 4;
                player->actor->base.pos[2] += 82.0f;
                player->actor->base.pos[3] = 1.8849558f;
                ActorSetHeading(player->actor, player->actor->base.pos[3]);
            }
            break;
        case 0x63:
            if (kind == 2) {
                trackId = 2;
            }
            break;
        default:
            break;
        }
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        node = (BtlDemoSceneMotionEvent *)MemAlloc(0x28, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (node != NULL) {
            BtlDemoSceneMotionEventCtor(node, player->scene, target, trackId, player->demoId,
                                        &player->motionEvents);
        }
        break;
    case 3:
        param = ev->motionParam;
        if (kind == 2) {
            actor = player->actor;
            if (actor != NULL) {
                ActorPlayMotion(ev->blendTime, actor, BtlDemoSceneGetVoiceMotion(player, param),
                                ev->motionFlag != 0, 1);
            }
        } else if (kind == 3) {
            unit = (BtlBakugan *)player->unit;
            if (unit != NULL) {
                BtlBakuganPlayMotion(ev->blendTime, unit, BtlDemoSceneGetMotionId(player, param),
                                     ev->motionFlag != 0, 1);
            }
        } else if (kind == 4) {
            ball = player->follower;
            if (ball != NULL) {
                ActorBallPlayMotion(ev->blendTime, &ball->base,
                                    BtlDemoSceneMotionSlot(player, param), ev->motionFlag != 0,
                                    true);
            }
        }
        break;
    default:
        break;
    }

    if (ev != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)ev->base.base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)ev + dtor->delta, 3);
    }
}
