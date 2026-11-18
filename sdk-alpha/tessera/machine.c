/* SPDX-License-Identifier: GPL-2.0-or-later
 * TESSERA — Phase Lattice. Shared machine SDK alpha / Digitone MKI OS 1.44.
 */
#include <stdint.h>
#include "elekloader/sdk_alpha_machine.h"
#include "../common/phase_sine.h"

enum { RATIO, INDEX, FOLD, SKEW, SYNC, FBK, SUB, LVL,
       DETUN, BIAS, RING, BITS, GLCH, NOIS, MIX };
enum { Q15_ONE = 32767 };

struct tessera_state {
    uint32_t carrier;
    uint32_t modulator;
    uint32_t sub;
    int32_t feedback;
    uint32_t rng;
};

static const int32_t ratio_semitones[8] = {
    -12 * 65536, 0, 7 * 65536, 12 * 65536,
    19 * 65536, 24 * 65536, 28 * 65536, 34 * 65536
};

static uint32_t random32(struct tessera_state *s)
{
    uint32_t x = s->rng;
    if (!x) x = 0x1b873593u;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s->rng = x;
    return x;
}

/* The signal path bounds x to [-208122, 207922] at all knob values. */
static int32_t clamp_q15(int32_t x)
{
    if (x > Q15_ONE) { DN1_LAB_CLIP_HIT(x); return Q15_ONE; }
    if (x < -Q15_ONE) { DN1_LAB_CLIP_HIT(x); return -Q15_ONE; }
    return x;
}

static uint32_t scale_phase_q16(uint32_t x, uint32_t q)
{
    return (x >> 16) * q + (((x & 0xffffu) * q) >> 16);
}

static const struct elek_knob knobs[] = {
    [RATIO] = { "RATIO", .min = 0, .max = 7, .def = 4 },
    [INDEX] = { "INDEX", .max = 127, .def = 63 },
    [FOLD]  = { "FOLD",  .max = 127, .def = 0 },
    [SKEW]  = { "SKEW",  .max = 127, .def = 16 },
    [SYNC]  = { "SYNC",  .max = 127, .def = 0 },
    [FBK]   = { "FBK",   .max = 127, .def = 30 },
    [SUB]   = { "SUB",   .max = 127, .def = 12 },
    [LVL]   = { "LVL",   .max = 127, .def = 64 },
    [DETUN] = { "DETUN", .min = -100, .max = 100, .def = 1 },
    [BIAS]  = { "BIAS",  .max = 127, .def = 64 },
    [RING]  = { "RING",  .max = 127, .def = 24 },
    [BITS]  = { "BITS",  .max = 127, .def = 0 },
    [GLCH]  = { "GLCH",  .max = 127, .def = 4 },
    [NOIS]  = { "NOIS",  .max = 127, .def = 8 },
    [MIX]   = { "MIX",   .max = 127, .def = 110 }
};

static void tessera_render(const struct elek_voice *v, elek_sample *out)
{
    struct tessera_state *s = ELEK_STATE(struct tessera_state, v);
    int32_t ratio = v->knob[RATIO], index = v->knob[INDEX];
    int32_t fold_amt = v->knob[FOLD], skew = v->knob[SKEW];
    int32_t sync = v->knob[SYNC], fbk = v->knob[FBK];
    int32_t sub = v->knob[SUB], level = v->knob[LVL];
    int32_t detune = v->knob[DETUN], bias = v->knob[BIAS];
    int32_t ring = v->knob[RING], bits = v->knob[BITS];
    int32_t glitch = v->knob[GLCH], noise = v->knob[NOIS], mix = v->knob[MIX];
    int32_t index_scale, fbk_scale, skew_scale, ring_scale, sub_scale;
    int32_t mix_scale, mix_inverse, noise_scale, level_scale_q8, sync_scale;
    int32_t glitch_threshold, glitch_shift;
    uint32_t inc_a, inc_b, inc_sub, pa, pb, ps;
    int32_t i;

    if (ratio < 0) ratio = 0;
    if (ratio > 7) ratio = 7;
    index_scale = (index * 65536) / 127;
    fbk_scale = (fbk * 65536) / 127;
    skew_scale = (skew * 65536) / 127;
    ring_scale = (ring * 65536) / 127;
    sub_scale = (sub * 65536) / 127;
    mix_scale = (mix * 65536) / 127;
    mix_inverse = 65536 - mix_scale;
    noise_scale = (noise * 65536) / (127 * 12);
    /* 245/256 of full scale leaves about 0.38 dB for feedback + bias + noise peaks;
     * this avoids recurring hard clamps at the all-controls-maximum corner. */
    level_scale_q8 = (level * 245) / 127;
    sync_scale = ((127 - sync) * 65536) / 127;
    glitch_threshold = glitch ? 1 + (glitch * 15) / 127 : 0;
    glitch_shift = 5 + (glitch * 8) / 127;

    if (v->start) {
        s->carrier = 0;
        s->modulator = 0;
        s->sub = 0;
        s->feedback = 0;
        s->rng = 0x165667b1u ^ (v->track * 0x9e3779b9u) ^ (uint32_t)v->pitch;
    }
    inc_a = elek_phase_inc(v->pitch);
    inc_b = elek_phase_inc(v->pitch + ratio_semitones[ratio] + (detune * 65536) / 100);
    inc_sub = inc_a >> 1;
    pa = s->carrier;
    pb = s->modulator;
    ps = s->sub;

    for (i = 0; i < ELEK_VOICE_FRAMES; ++i) {
        int32_t mod = 0, fb = s->feedback;
        uint32_t phase_mod = 0, warped;
        int32_t a, b = 0, sample;
        if (index_scale) {
            mod = dn1_sin_q15(pb);
            phase_mod = (uint32_t)(mod * index_scale);
        }
        if (fbk_scale) phase_mod += (uint32_t)(fb * fbk_scale);
        warped = pa + phase_mod;
        if (skew_scale) {
            int32_t sgn = dn1_sin_q15(warped);
            warped += (uint32_t)((sgn * skew_scale) / 2);
        }
        a = dn1_sin_q15(warped);
        if (mix_inverse || ring_scale)
            b = (inc_b <= 0x7fffffffu) ? dn1_sin_q15(pb + 0x40000000u) : 0;
        if (ring_scale) {
            int32_t ring_mod = (a * b) >> 15;
            a = dn1_lerp_q16(a, ring_mod, (uint32_t)ring_scale);
        }
        a = dn1_lerp_q16(a, b, (uint32_t)mix_inverse);
        sample = a * (128 + fold_amt * 5) / 128;
        sample += (bias - 64) * 200;
        sample = dn1_fold_q15(sample);
        if (sub_scale) {
            int32_t sub_wave = dn1_sin_q15(ps);
            sample = dn1_lerp_q16(sample, sub_wave, (uint32_t)sub_scale);
        }
        /* Noise and glitch are voice layers; the native AMP envelope supplies
         * the key-off/release contour (the SDK voice has no gate flag). */
        if (noise_scale || glitch) {
            int32_t n = (int32_t)(random32(s) >> 17) - 16384;
            sample += (n * noise_scale) / 65536;
            if (glitch && (random32(s) & 127u) < (uint32_t)glitch_threshold)
                sample = dn1_quantize_pow2(sample, glitch_shift);
        }
        if (bits) {
            /* 0 bypasses quantization; 1..127 increases resolution smoothly
             * from a coarse 2-bit texture to a finer 10-bit texture. */
            int32_t q = 14 - (((bits - 1) * 9) / 126);
            sample = dn1_quantize_pow2(sample, q);
        }
        sample = (sample * level_scale_q8) / 256;
        sample = clamp_q15(sample);
        out[i] = elek_from_q15(sample);

        s->feedback = sample;
        pa += inc_a;
        pb += inc_b;
        ps += inc_sub;
        if (pa < inc_a && sync)
            pb = scale_phase_q16(pb, (uint32_t)sync_scale);
    }
    s->carrier = pa;
    s->modulator = pb;
    s->sub = ps;
}

const struct elek_machine tessera_machine = {
    .id = 102, .name = "TESSERA", .short_name = "TESS", ELEK_KNOBS(knobs),
    .pages = {
        { "PHASE", { [ELEK_A] = &knobs[RATIO], [ELEK_B] = &knobs[INDEX],
                      [ELEK_C] = &knobs[FOLD], [ELEK_D] = &knobs[SKEW],
                      [ELEK_E] = &knobs[SYNC], [ELEK_F] = &knobs[FBK],
                      [ELEK_G] = &knobs[SUB], [ELEK_H] = &knobs[LVL] } },
        { "SHAPE", { [ELEK_A] = &knobs[DETUN], [ELEK_B] = &knobs[BIAS],
                      [ELEK_C] = &knobs[RING], [ELEK_D] = &knobs[BITS],
                      [ELEK_E] = &knobs[GLCH], [ELEK_F] = &knobs[NOIS],
                      [ELEK_G] = &knobs[MIX] } }
    },
    .render = tessera_render
};
ELEK_REGISTER_MACHINE(tessera_machine);
