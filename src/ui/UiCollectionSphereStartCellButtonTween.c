// bdc 0x0897eac4 UiCollectionSphereStartCellButtonTween
#include "bdc.h"

/* Starts the appear (`out == 0`) or disappear (`out != 0`) tween (start scale 1.5, flags 3) of the
   24 grid cell sprites of the sphere (Bakugan figure) collection screen
   (`UiCollectionSphere`): cell frames (sprites 0-5), name labels (27-32),
   entry-id-0 overlays (7-12) and new markers (14-19), each with its own record in `tweens`.
   On appear it first refreshes the current page: shows only the used cells
   (`UiCollectionSphereIsCellUsed`), resets the frames to `"waku_4_b"` (`UiCollectionSphereSetCellFrame`),
   sets the name textures (`UiCollectionSphereSetNameTexture`, category 2:
   `UiCollectionSphereSetSpecialNameTexture`), shows overlay 7+i when entry id is 0 and marker 14+i when
   the entry is new, moves every sprite to its cell position (`UiCollectionSpherePlaceCellSprite`)
   and, except for the labels, restores its depth from `spriteZ` and clears its add colour.
   On special pages (page kind 1/2) only the first cell of each group is used, moved to slot
   4/31/13/20 and fed from `kind1Ids`/`kind2Ids` and `kind1New`/`kind2New`. */

void UiCollectionSphereStartCellButtonTween(UiCollectionSphere *self, u8 out)
{
  u8 kind;
  int i;
  bool used;
  s8 category;
  s8 page;
  GfxSprite *sprite;
  float z;

  if (out == 0) {
    kind = UiCollectionSphereGetPageKind(self, (u8)self->page);

    /* cell frames 0-5 */
    for (i = 0; i < 6; i++) {
      used = UiCollectionSphereIsCellUsed(self, (u8)self->category, (u8)i, (u8)self->page);
      sprite = ((GfxSprite **)self->base.data)[i];
      if (used == 1) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      UiCollectionSphereSetCellFrame(self, ((GfxSprite **)self->base.data)[i], 0);
      if (kind != 0xff && kind != 0 && i == 0) {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, 4);
      } else {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, (u8)i);
      }
      z = self->spriteZ[i];
      ((GfxSprite **)self->base.data)[i]->posZ = z;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }

    /* name labels 27-32 */
    for (i = 27; i < 33; i++) {
      used = UiCollectionSphereIsCellUsed(self, (u8)self->category, (u8)(i - 27), (u8)self->page);
      sprite = ((GfxSprite **)self->base.data)[i];
      if (used == 1) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      category = self->category;
      if (category == 0 || category == 1) {
        UiCollectionSphereSetNameTexture(self, ((GfxSprite **)self->base.data)[i],
                                         self->entryIds[(i - 27) + self->page * 6]);
      } else if (category == 2) {
        if (kind == 0) {
          UiCollectionSphereSetSpecialNameTexture(
              self, ((GfxSprite **)self->base.data)[i],
              self->entryIds[(i - 27) + (self->page / 3) * 6]);
        } else if (i != 27) {
          UiCollectionSphereSetSpecialNameTexture(self, ((GfxSprite **)self->base.data)[i], 0);
        } else if (kind == 1) {
          UiCollectionSphereSetSpecialNameTexture(self, ((GfxSprite **)self->base.data)[i],
                                                  self->kind1Ids[self->page / 3]);
        } else if (kind == 2) {
          UiCollectionSphereSetSpecialNameTexture(self, ((GfxSprite **)self->base.data)[i],
                                                  self->kind2Ids[self->page / 3]);
        }
      }
      if (kind != 0xff && kind != 0 && i == 27) {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, 31);
      } else {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, (u8)i);
      }
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }

    /* entry-id-0 overlays 7-12 */
    for (i = 7; i < 13; i++) {
      if (kind == 0xff || kind == 0) {
        category = self->category;
        page = self->page;
        used = UiCollectionSphereIsCellUsed(self, (u8)category, (u8)(i - 7), (u8)page);
        sprite = ((GfxSprite **)self->base.data)[i];
        if (used == 1) {
          u8 id;
          if (category == 2) {
            id = self->entryIds[(i - 7) + (page / 3) * 6];
          } else {
            id = self->entryIds[(i - 7) + page * 6];
          }
          if (id != 0) {
            sprite->flags &= ~1u;
          } else {
            sprite->flags |= 1;
          }
        } else {
          sprite->flags &= ~1u;
        }
      } else if (i != 7) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      } else if (kind == 1) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->kind1Ids[self->page / 3] != 0) {
          sprite->flags &= ~1u;
        } else {
          sprite->flags |= 1;
        }
      } else if (kind == 2) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->kind2Ids[self->page / 3] != 0) {
          sprite->flags &= ~1u;
        } else {
          sprite->flags |= 1;
        }
      }
      if (kind != 0xff && kind != 0 && i == 7) {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, 13);
      } else {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, (u8)i);
      }
      z = self->spriteZ[i];
      ((GfxSprite **)self->base.data)[i]->posZ = z;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }

    /* new markers 14-19 */
    for (i = 14; i < 20; i++) {
      if (kind == 0xff || kind == 0) {
        category = self->category;
        page = self->page;
        used = UiCollectionSphereIsCellUsed(self, (u8)category, (u8)(i - 14), (u8)page);
        sprite = ((GfxSprite **)self->base.data)[i];
        if (used == 1) {
          u8 isNew;
          if (category == 2) {
            isNew = self->entryNew[(i - 14) + (page / 3) * 6];
          } else {
            isNew = self->entryNew[(i - 14) + page * 6];
          }
          if (isNew != 0) {
            sprite->flags |= 1;
          } else {
            sprite->flags &= ~1u;
          }
        } else {
          sprite->flags &= ~1u;
        }
      } else if (i != 14) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      } else if (kind == 1) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->kind1New[self->page / 3] == 1) {
          sprite->flags |= 1;
        } else {
          sprite->flags &= ~1u;
        }
      } else if (kind == 2) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (self->kind2New[self->page / 3] == 1) {
          sprite->flags |= 1;
        } else {
          sprite->flags &= ~1u;
        }
      }
      if (kind != 0xff && kind != 0 && i == 14) {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, 20);
      } else {
        UiCollectionSpherePlaceCellSprite(self, (u8)i, (u8)i);
      }
      z = self->spriteZ[i];
      ((GfxSprite **)self->base.data)[i]->posZ = z;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 0.0f;
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0; i < 6; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 27; i < 33; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 7; i < 13; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 14; i < 20; i++) {
      UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
