// bdc 0x088a0e8c GameGimmickCorePointState00Idle
#include "bdc.h"

/* State 0 of the core-point gimmick (table `0x08a83c54`): bobs the model up and down (cosine of the
   angle `spin`, +5 degrees per frame), clears bit 0 of the attached collider's flags when it took a
   new hit, and returns early when the gimmick is `invisible` and the player's scan power is off.
   Otherwise clears contact flag 2, copies `alpha` to the draw alpha, and when the player is within
   `pickupRadius` (XZ distance) and 22 units of height, or the attached collider was hit, collects
   it: spawns the pickup effect 0xe, plays sound 0x2c00033/34/35 and adds 100/300/1000 to the save
   profile's `points` (clamped to 0..9999999) for kinds 10/13, 11/14, 12/15 (S/M/L), sets alpha 1
   and the translucent material and advances to state 1. Then steps the model motion, runs
   `GameGimmickCorePointUpdateBounce` while launched and copies the camera billboard matrix to
   the core node `coreNode` (found by name `coreNodeName`).
   `shapePos` is zeroed (bank C720) on pickup. */

void GameGimmickCorePointState00Idle(GameGimmickCorePoint *obj)
{
    int spin;
    float bob;
    ActorPlayer *player;
    ActorPlayer *near;
    bool hit;
    GfxEffect *effect;
    SaveProfile *profile;
    GfxCamera *cam;
    GmoNode *node;
    int pts;
    float pos[4];

    spin = obj->spin;
    bob = (1.0f - __builtin_cosf((float)spin * 0.0055555557f * 3.1415927f)) * 0.5f;
    bob = bob * 2.0f;
    obj->base.base.pos[1] = obj->baseY + bob;
    obj->spin = (spin + 5) % 360;

    player = (ActorPlayer *)ActorFindPlayer();
    hit = false;
    if (CollisionColliderTickHit((CoreNode *)obj->base.attached)) {
        CollisionCollider *collider = (CollisionCollider *)obj->base.attached;

        hit = true;
        collider->flags = collider->flags & ~1u;
    }
    if (player != NULL && obj->invisible != 0 && player->scan == 0) {
        return;
    }
    obj->base.contactFlags = obj->base.contactFlags & ~2;
    obj->base.base.ambient[3] = obj->base.alpha;

    if (player != NULL) {
        pos[0] = obj->base.base.pos[0];
        pos[1] = obj->base.base.pos[1];
        pos[2] = obj->base.base.pos[2];
        pos[3] = obj->base.base.pos[3];
        pos[1] = pos[1] - bob;
        near = (ActorPlayer *)ActorFindPlayer();
        if (near != NULL && fabsf(pos[1] - near->base.base.pos[1]) <= 22.0f) {
            float dist;

            float dx;
            float dz;

            /* XZ distance: the y lane of the difference is zeroed before the dot product. */
            dx = near->base.base.pos[0] - pos[0];
            dz = near->base.base.pos[2] - pos[2];
            dist = __builtin_sqrtf(dx * dx + dz * dz);
            if (dist <= obj->pickupRadius) {
                hit = true;
            }
        }
    }

    if (hit) {
        obj->base.active = 0;
        obj->shapePos.x = 0.0f;
        obj->shapePos.y = 10000.0f;
        obj->shapePos.z = 0.0f;
        obj->shapePos.w = 0.0f;
        obj->shapeParam = 0.0f;
        effect = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0xe,
                                                     &obj->base.base.data->rootMatrix[12]);
        switch (obj->cpKind - 10) {
        case 0:
        case 3:
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c00033, 0, 0);
            }
            profile = SaveGetProfile();
            pts = profile->data->points + 100;
            if (pts > 9999999) {
                pts = 9999999;
            } else if (pts < 0) {
                pts = 0;
            }
            profile->data->points = pts;
            effect->textureSlot = 2;
            effect->size[0] = 1.0f;
            effect->vec1d0[1] = 4.0f;
            effect->size[1] = 1.0f;
            effect->size[2] = 1.0f;
            effect->size[3] = 0.0f;
            break;
        case 1:
        case 4:
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c00034, 0, 0);
            }
            profile = SaveGetProfile();
            pts = profile->data->points + 300;
            if (pts > 9999999) {
                pts = 9999999;
            } else if (pts < 0) {
                pts = 0;
            }
            profile->data->points = pts;
            effect->textureSlot = 1;
            effect->size[0] = 2.0f;
            effect->vec1d0[1] = 8.0f;
            effect->size[1] = 2.0f;
            effect->size[2] = 2.0f;
            effect->size[3] = 0.0f;
            break;
        case 2:
        case 5:
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c00035, 0, 0);
            }
            profile = SaveGetProfile();
            pts = profile->data->points + 1000;
            if (pts > 9999999) {
                pts = 9999999;
            } else if (pts < 0) {
                pts = 0;
            }
            profile->data->points = pts;
            effect->textureSlot = 3;
            effect->vec1d0[1] = 16.0f;
            effect->size[0] = 3.0f;
            effect->size[1] = 3.0f;
            effect->size[2] = 3.0f;
            effect->size[3] = 0.0f;
            break;
        default:
            break;
        }
        obj->base.alpha = 1.0f;
        GfxModelForEachMaterial(&obj->base.base, (void *)GameGimmickCorePointMaterialSetTranslucent,
                                NULL);
        obj->cpState = obj->cpState + 1;
    }

    GfxModelUpdateMotion(&obj->base.base);
    GfxModelApplyMotion(&obj->base.base);
    if (obj->launched != 0) {
        GameGimmickCorePointUpdateBounce(obj);
        GameGimmickCorePointUpdateLaunchScale(obj);
    }
    cam = g_gfxActiveCamera;
    if (cam != NULL) {
        if (obj->coreNode == NULL) {
            obj->coreNode = GfxModelFindNode(&obj->base.base, obj->coreNodeName);
        }
        if (obj->coreNode != NULL) {
            node = (GmoNode *)obj->coreNode;
            __builtin_memcpy(node->localMatrix, &cam->billboard, sizeof(node->localMatrix));
        }
    }
}
