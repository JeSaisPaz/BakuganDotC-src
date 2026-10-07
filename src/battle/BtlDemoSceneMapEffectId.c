// bdc 0x0890598c BtlDemoSceneMapEffectId
#include "bdc.h"

/* Maps a `.scb` effect event id to a `GfxEffect` id for the battle demo scene player task
   (`BtlDemoScenePlayerCtor`): clears `*attach` and `stagePool`, then 0x0f/0x124 → 5,
   0x93 → 0x20, 0x97 → 0x22, 0xe8..0xed → 0x23/0x27/0x24/0x25/0x26/0x28, 0xf4..0xfa →
   0x29/0x2a/0x2e/0x2b/0x2c/0x2d/0x2f, 0x150 → 0x21, 0x2b1 → 0x150, 0x2b2 → 0x14c (all of these set
   `stagePool`); 0xa5/0xa7/0xa8/0xa9/0xaa/0xab → 0x70/0x71/0x75/0x72/0x73/0x74 with `*attach = 1`;
   0x37 and 0xe0 map to themselves. Every other id returns -1. */

int BtlDemoSceneMapEffectId(void *task, int id, u8 *attach)
{
    BtlDemoScenePlayer *self = (BtlDemoScenePlayer *)task;

    *attach = 0;
    self->stagePool = 0;
    switch (id) {
    case 0x0f:
    case 0x124:
        self->stagePool = 1;
        return 5;
    case 0x37:
    case 0xe0:
        return id;
    case 0x93:  self->stagePool = 1; return 0x20;
    case 0x97:  self->stagePool = 1; return 0x22;
    case 0xa5:  *attach = 1; return 0x70;
    case 0xa7:  *attach = 1; return 0x71;
    case 0xa8:  *attach = 1; return 0x75;
    case 0xa9:  *attach = 1; return 0x72;
    case 0xaa:  *attach = 1; return 0x73;
    case 0xab:  *attach = 1; return 0x74;
    case 0xe8:  self->stagePool = 1; return 0x23;
    case 0xe9:  self->stagePool = 1; return 0x27;
    case 0xea:  self->stagePool = 1; return 0x24;
    case 0xeb:  self->stagePool = 1; return 0x25;
    case 0xec:  self->stagePool = 1; return 0x26;
    case 0xed:  self->stagePool = 1; return 0x28;
    case 0xf4:  self->stagePool = 1; return 0x29;
    case 0xf5:  self->stagePool = 1; return 0x2a;
    case 0xf6:  self->stagePool = 1; return 0x2e;
    case 0xf7:  self->stagePool = 1; return 0x2b;
    case 0xf8:  self->stagePool = 1; return 0x2c;
    case 0xf9:  self->stagePool = 1; return 0x2d;
    case 0xfa:  self->stagePool = 1; return 0x2f;
    case 0x150: self->stagePool = 1; return 0x21;
    case 0x2b1: self->stagePool = 1; return 0x150;
    case 0x2b2: self->stagePool = 1; return 0x14c;
    default:
        return -1;
    }
}
