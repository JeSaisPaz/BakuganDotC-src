// bdc 0x089f4e10 GfxSpriteLayerCollect3D
#include "bdc.h"

/* Collects the drawable billboard sprites of a 3D layer: walks the list from `sprite` (`next`),
   skips sprites with `billboardMode == 9` or without the visible flag (bit0), calls
   `preDrawCallback`, stores `camera.eye - sprite.pos` into `toCamera` (W = `eye[3]`) and appends a
   GfxSpriteSortPair `{sprite, depth}` to `out`, where `depth` is the squared camera distance
   (dot of the XYZ difference), or g_gfxSpriteFarDepthKey (+inf) when sprite flag `0x80` is set.
   Returns the count as soon as `max` entries are stored, else at the end of the list. */

int GfxSpriteLayerCollect3D(GfxSprite *sprite, GfxSpriteSortPair *out, GfxCamera *camera, int max)
{
    int count = 0;

    while (sprite != NULL) {
        if (sprite->billboardMode != 9 && (sprite->flags & 1) != 0) {
            float key;

            if (sprite->preDrawCallback != NULL) {
                ((void (*)(GfxSprite *))sprite->preDrawCallback)(sprite);
            }
            sprite->toCamera[0] = camera->eye[0] - sprite->posX;
            sprite->toCamera[1] = camera->eye[1] - sprite->posY;
            sprite->toCamera[2] = camera->eye[2] - sprite->posZ;
            sprite->toCamera[3] = camera->eye[3];
            if ((sprite->flags & 0x80) != 0) {
                key = g_gfxSpriteFarDepthKey;
            } else {
                key = sprite->toCamera[0] * sprite->toCamera[0] +
                      sprite->toCamera[1] * sprite->toCamera[1] +
                      sprite->toCamera[2] * sprite->toCamera[2];
            }
            out->depth = key;
            out->sprite = sprite;
            count = count + 1;
            out = out + 1;
            if (!(count < max)) {
                return count;
            }
        }
        sprite = sprite->next;
    }
    return count;
}
