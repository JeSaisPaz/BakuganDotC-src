// bdc 0x089c0a40 SndEmitterUpdateAll
#include "bdc.h"

/* Per-frame update of all positional (3D) sound emitters against the listener, called from
   `BootEndOfFrame` with `SndGetListener`. It first runs the group arbitration
   (`SndEmitterGroupSelectNearest`, `SndEmitterAssignGroups`), then flushes
   `g_soundEmitterList` and for every `SndEmitter`: drops a stale voice handle
   (`SndManagerIsHandlePlaying`); stops the voice if the emitter is silenced or released or the
   listener's `flag24` is set; otherwise refreshes `posA`/`posB` from `src`, measures the squared
   distance to the listener (point, or point-to-segment for `shape == 2`), and if it exceeds
   `radius^2` stops the voice and releases its group request, else starts a voice with
   `SndManagerPlay` when none is playing (registering it with `SndGroupLoaderRequest`) and
   updates its volume (`SndApplyFalloffCurve`, shared across a group, `SndManagerSetVolume`) and
   pan (`SndListenerGetPan` weighted by curve 1, `SndManagerSetPan`); finished emitters are
   unlinked and, if `autoFree`, freed. It ends with `SndGroupLoaderUpdate`. */

void SndEmitterUpdateAll(SndListener *listener)
{
    SndEmitter *e;
    SndEmitterProfile *profile;
    SndManager *mgr;
    bool done;    /* not reset per emitter: carries over from the previous one (see Notes) */
    bool stopped;
    s32 newHandle;
    float dist;
    float radiusSq;
    float ratio;
    float vol;
    float pan;
    float pt[3];  /* not reset per emitter: stale for shape >= 3 */
    float t;
    float t2;

    SndEmitterGroupSelectNearest(listener->pos);
    SndEmitterAssignGroups(listener);
    done = false;
    if (g_soundEmitterList != NULL) {
        SndEmitterListFlush(g_soundEmitterList);
        while ((e = SndEmitterListNext(g_soundEmitterList)) != NULL) {
            if (!SndHasManager()) {
                e->handle = -1;
            }
            if (e->handle >= 0) {
                mgr = SndGetManager();
                if (!SndManagerIsHandlePlaying(mgr, e->handle)) {
                    e->handle = -1;
                }
            }

            if (e->silenced != 0 || e->released != 0 || SndListenerIsMuted(listener) != 0) {
                if (e->state38 <= 0) {
                    e->state38 = 0;
                }
                done = true;
                if (e->handle >= 0) {
                    if (e->repeat > 0) {
                        e->repeat = e->repeat - 1;
                    }
                    stopped = false;
                    if (SndHasManager()) {
                        mgr = SndGetManager();
                        stopped = SndManagerStop(mgr, e->handle);
                    }
                    if (stopped) {
                        if (SndHasGroupLoader()) {
                            SndGroupLoaderRelease(SndGetGroupLoader(), e->soundId);
                        }
                        e->handle = -1;
                    }
                }
            }

            if (!done) {
                if (e->src != NULL) {
                    e->posA[0] = e->src[0];
                    e->posA[1] = e->src[1];
                    e->posA[2] = e->src[2];
                    if (e->params->shape == 2) {
                        e->posB[0] = e->src[3];
                        e->posB[1] = e->src[4];
                        e->posB[2] = e->src[5];
                    }
                }
                if (e->handle >= 0 || e->src != NULL) {
                    profile = e->params;
                    if (profile->shape == 1) {
                        dist = SndListenerDistSq(listener, e->posA);
                        pt[0] = e->posA[0];
                        pt[1] = e->posA[1];
                        pt[2] = e->posA[2];
                    } else if (profile->shape == 2) {
                        dist = SndPointSegmentDistSq(listener->pos, e->posA, e->posB, &t);
                        pt[0] = e->posA[0];
                        pt[1] = e->posA[1];
                        pt[2] = e->posA[2];
                        pt[0] = pt[0] + (e->posB[0] - pt[0]) * t;
                        pt[1] = pt[1] + (e->posB[1] - pt[1]) * t;
                        pt[2] = pt[2] + (e->posB[2] - pt[2]) * t;
                    } else {
                        dist = 0.0f;
                    }
                    radiusSq = profile->radius * profile->radius;

                    if (!(dist <= radiusSq)) {
                        /* out of range: stop the voice, release its group */
                        if (SndHasGroupLoader()) {
                            SndGroupLoaderRelease(SndGetGroupLoader(), e->soundId);
                        }
                        if (e->handle >= 0) {
                            stopped = false;
                            if (SndHasManager()) {
                                mgr = SndGetManager();
                                stopped = SndManagerStop(mgr, e->handle);
                            }
                            if (stopped) {
                                e->handle = -1;
                            }
                        }
                        if (e->repeat == 0) {
                            e->src = NULL;
                        } else {
                            e->repeat = e->repeat - 1;
                        }
                    } else {
                        if (e->handle < 0) {
                            if (e->repeat == 0) {
                                e->src = NULL;
                                continue;
                            }
                            newHandle = 0;
                            e->state38 = (e->state38 >= 0) ? e->state38 : 0;
                            if (SndHasManager()) {
                                mgr = SndGetManager();
                                newHandle = (s32)SndManagerPlay(mgr, e->soundId, 0, 1);
                            }
                            if (newHandle < 0) {
                                continue;
                            }
                            if (e->repeat != -1) {
                                e->repeat = e->repeat - 1;
                            }
                            e->handle = newHandle;
                            e->volume = 0.0f;
                            e->pan = 0.0f;
                        } else {
                            e->state38 = e->state38 + 1;
                        }

                        if (SndHasGroupLoader()) {
                            SndGroupLoaderRequest(SndGetGroupLoader(), e->soundId, e->handle);
                        }
                        if (profile->shape == 0) {
                            continue;
                        }

                        ratio = dist / radiusSq;
                        vol = SndApplyFalloffCurve(ratio, profile->curve)
                                * (profile->volFar - profile->volNear) + profile->volNear;
                        e->targetVolume = vol;
                        if (e->groupCount >= 2) {
                            vol = vol * 0.5f;
                            vol = vol + vol / (float)e->groupCount;
                        }
                        if (e->volume != vol) {
                            if (SndHasManager()) {
                                SndManagerSetVolume(vol, SndGetManager(), e->handle);
                            }
                            e->volume = vol;
                        }

                        if (profile->shape == 2) {
                            dist = SndPointSegmentDistSq(listener->target, e->posA, e->posB, &t2);
                            pt[0] = e->posA[0];
                            pt[1] = e->posA[1];
                            pt[2] = e->posA[2];
                            pt[0] = pt[0] + (e->posB[0] - pt[0]) * t2;
                            pt[1] = pt[1] + (e->posB[1] - pt[1]) * t2;
                            pt[2] = pt[2] + (e->posB[2] - pt[2]) * t2;
                        }
                        pan = SndListenerGetPan(listener, pt);
                        pan = pan * SndApplyFalloffCurve(ratio, 1);
                        if (e->pan != pan) {
                            e->pan = pan;
                            if (SndHasManager()) {
                                mgr = SndGetManager();
                                SndManagerSetPan(e->pan, mgr, e->handle);
                            }
                        }
                    }
                }
            }

            /* no voice left: unlink finished emitters, free autoFree ones */
            if (e->handle >= 0) {
                continue;
            }
            done = false;
            if (e->src == NULL) {
                done = true;
            } else if (e->autoFree != 0 && e->repeat == 0) {
                done = true;
            }
            if (!done) {
                continue;
            }
            SndEmitterListRemove(g_soundEmitterList, e);
            if (e->autoFree == 0) {
                continue;
            }
            if (g_soundEmitterPool != NULL && MemPoolFree(g_soundEmitterPool, e)) {
                e = NULL;
            }
            if (e != NULL) {
                MemLock();
                MemFree(e, NULL, 0);
                MemUnlock();
            }
        }
        SndEmitterListFlush(g_soundEmitterList);
    }
    if (SndHasGroupLoader()) {
        SndGroupLoaderUpdate(SndGetGroupLoader());
    }
}
