// bdc 0x0889f8e0 CoreBezierCtor
#include "bdc.h"

/* Constructor of the small quadratic-Bezier helper object (0x64 bytes: weight `+0x10`, control
   points `+0x20`/`+0x30`/`+0x40`, result `+0x50`, vtable `g_coreBezierVtbl` at `+0x60`): zeroes the
   five vectors (bank constant C720 = (0, 0, 0, 0)) and the weight, and installs the vtable. Embedded at
   `+0x200` of the core-point gimmick by `GameGimmickCorePointCtor`. */
CoreBezier *CoreBezierCtor(CoreBezier *self)
{
    self->origin.x = 0.0f;
    self->origin.y = 0.0f;
    self->origin.z = 0.0f;
    self->origin.w = 0.0f;
    self->vtbl = g_coreBezierVtbl;
    self->weight = 0.0f;
    self->p0.x = 0.0f;
    self->p0.y = 0.0f;
    self->p0.z = 0.0f;
    self->p0.w = 0.0f;
    self->p1.x = 0.0f;
    self->p1.y = 0.0f;
    self->p1.z = 0.0f;
    self->p1.w = 0.0f;
    self->p2.x = 0.0f;
    self->p2.y = 0.0f;
    self->p2.z = 0.0f;
    self->p2.w = 0.0f;
    self->result.x = 0.0f;
    self->result.y = 0.0f;
    self->result.z = 0.0f;
    self->result.w = 0.0f;
    return self;
}
