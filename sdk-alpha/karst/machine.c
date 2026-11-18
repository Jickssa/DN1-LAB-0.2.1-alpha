/* SPDX-License-Identifier: GPL-2.0-or-later
 * KARST — Modal Matter. Shared machine SDK alpha / Digitone MKI OS 1.44.
 */
#include <stdint.h>
#include "elekloader/sdk_alpha_machine.h"
#include "../common/phase_sine.h"

enum { MTRL, STIF, SPRK, INH, BOW, EDGE, DAMP, DECY,
       TONE, HIT, NOIS, RASP, SPRD, BODY, LVL };
enum { MODES = 3, Q15_ONE = 32767, STATE_LIMIT = 18000 };

struct karst_state {
    int32_t y1[MODES];
    int32_t y2[MODES];
    uint32_t rng;
};

static const int32_t partial_semitones[8][MODES] = {
    { 0, 12 * 65536, 19 * 65536 },
    { 0, 18 * 65536, 29 * 65536 },
    { 0, 15 * 65536, 25 * 65536 },
    { 0,  8 * 65536, 13 * 65536 },
    { 0,  6 * 65536, 14 * 65536 },
    { 0, 19 * 65536, 26 * 65536 },
    { 0, 12 * 65536, 24 * 65536 },
    { 0, 17 * 65536, 31 * 65536 }
};

static uint32_t random32(struct karst_state *s)
{
    uint32_t x = s->rng;
    if (!x) x = 0x6d2b79f5u;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s->rng = x;
    return x;
}

static int32_t clamp_state(int32_t x)
{
    if (x > STATE_LIMIT) return STATE_LIMIT;
    if (x < -STATE_LIMIT) return -STATE_LIMIT;
    return x;
}

static int32_t clamp_q15(int32_t x)
{
    if (x > Q15_ONE) { DN1_LAB_CLIP_HIT(x); return Q15_ONE; }
    if (x < -Q15_ONE) { DN1_LAB_CLIP_HIT(x); return -Q15_ONE; }
    return x;
}

static const struct elek_knob knobs[] = {
    [MTRL] = { "MTRL", .min = 0, .max = 7, .def = 0 },
    [STIF] = { "STIF", .max = 127, .def = 0 },
    [SPRK] = { "SPRK", .max = 127, .def = 21 },
    [INH]  = { "INH",  .max = 127, .def = 0 },
    [BOW]  = { "BOW",  .max = 127, .def = 0 },
    [EDGE] = { "EDGE", .max = 127, .def = 64 },
    [DAMP] = { "DAMP", .max = 127, .def = 48 },
    [DECY] = { "DECY", .max = 127, .def = 100 },
    [TONE] = { "TONE", .max = 127, .def = 64 },
    [HIT]  = { "HIT",  .max = 127, .def = 96 },
    [NOIS] = { "NOIS", .max = 127, .def = 12 },
    [RASP] = { "RASP", .max = 127, .def = 8 },
    [SPRD] = { "SPRD", .max = 127, .def = 24 },
    [BODY] = { "BODY", .max = 127, .def = 64 },
    [LVL]  = { "LVL",  .max = 127, .def = 110 }
};

static void karst_render(const struct elek_voice *v, elek_sample *out)
{
    struct karst_state *s = ELEK_STATE(struct karst_state, v);
    int32_t material = v->knob[MTRL];
    int32_t stiffness = v->knob[STIF], spark = v->knob[SPRK];
    int32_t inharm = v->knob[INH], bow = v->knob[BOW];
    int32_t edge = v->knob[EDGE], damp = v->knob[DAMP];
    int32_t decay = v->knob[DECY], tone = v->knob[TONE];
    int32_t hit = v->knob[HIT], noise_amt = v->knob[NOIS];
    int32_t rasp = v->knob[RASP], spread = v->knob[SPRD];
    int32_t body = v->knob[BODY], level = v->knob[LVL];
    int32_t coeff[MODES], r2[MODES], excite_scale[MODES], weight[MODES];
    uint32_t inc[MODES];
    uint8_t mode_active[MODES];
    int32_t sumw, i, m;

    if (material < 0) material = 0;
    if (material > 7) material = 7;
    if (v->start) {
        for (m = 0; m < MODES; ++m) s->y1[m] = s->y2[m] = 0;
        s->rng = 0x9e3779b9u ^ ((uint32_t)v->pitch * 0x45d9f3bu) ^ (v->track * 0x85ebca6bu);
    }

    {
        int32_t radius = 32700 + decay * 60 / 127;
        int32_t base_semi = (stiffness * 2 * 65536) / 127;
        int32_t extra = (inharm * 4 * 65536) / 127;
        for (m = 0; m < MODES; ++m) {
            int32_t semi = partial_semitones[material][m];
            int32_t partial_pitch;
            int32_t r = radius - m * (5 + damp * 40 / 127 + tone * 10 / 127);
            if (m == 1) semi += base_semi;
            if (m == 2) semi += base_semi + extra;
            if (m) semi += (spread * (m - 1) * 32768) / 127;
            partial_pitch = v->pitch + semi;
            if (partial_pitch < 0 || partial_pitch > (127 << 16)) {
                mode_active[m] = 0;
                inc[m] = 0;
                coeff[m] = r2[m] = excite_scale[m] = 0;
                s->y1[m] = s->y2[m] = 0;
            } else {
                mode_active[m] = 1;
                inc[m] = elek_phase_inc(partial_pitch);
                if (inc[m] > 0x7fffffffu) {
                    mode_active[m] = 0;
                    coeff[m] = r2[m] = excite_scale[m] = 0;
                    s->y1[m] = s->y2[m] = 0;
                } else {
                    int32_t sn = dn1_sin_q15(inc[m]);
                    int32_t c = dn1_sin_q15(inc[m] + 0x40000000u);
                    excite_scale[m] = sn < 0 ? -sn : sn;
                    coeff[m] = (2 * r * c) >> 15;
                    r2[m] = (r * r) >> 15;
                }
            }
        }
    }

    weight[0] = 96 + body * 2 - edge - tone / 2;
    weight[1] = 80 + edge / 2;
    weight[2] = 24 + edge + tone / 2 + spark / 2;
    if (weight[0] < 8) weight[0] = 8;
    sumw = weight[0] + weight[1] + weight[2];
    for (m = 0; m < MODES; ++m) weight[m] = weight[m] * Q15_ONE / sumw;
    weight[2] = Q15_ONE - weight[0] - weight[1];

    for (i = 0; i < ELEK_VOICE_FRAMES; ++i) {
        int32_t excitation = (i == 0 && v->start) ? 1800 + hit * 62 : 0;
        int32_t result = 0;
        /* The SDK does not expose key-gate state. These excitation layers run
         * while the voice is rendered; the Digitone AMP release gates them. */
        if (bow) {
            int32_t n = (int32_t)(random32(s) >> 17) - 16384;
            excitation += (n * bow) / 4800;
        }
        if (noise_amt) {
            int32_t n = (int32_t)(random32(s) >> 17) - 16384;
            excitation += (n * noise_amt) / 3600;
        }
        for (m = 0; m < MODES; ++m) {
            int32_t y, drive = excitation;
            if (!mode_active[m]) continue;
            if (m == 1) drive = drive * (80 + spark) / 160;
            if (m == 2) drive = drive * (24 + spark * 2) / 160;
            drive = drive * excite_scale[m] / 32767;
            y = ((coeff[m] * s->y1[m]) >> 15) - ((r2[m] * s->y2[m]) >> 15) + drive;
            y = clamp_state(y);
            s->y2[m] = s->y1[m];
            s->y1[m] = y;
            result += (y * weight[m]) >> 15;
        }
        /* Fixed gain reserves about 15% of Q15 peak headroom in the randomized
         * eight-lane regression; the native AMP remains the master level. */
        result = (result * 165) / 100;
        result = result * level / 127;
        if (rasp) {
            int32_t bits = 4 + (15 * (127 - rasp)) / 127;
            int32_t shift = 15 - bits;
            if (shift > 0) result = dn1_quantize_pow2(result, shift);
        }
        out[i] = elek_from_q15(clamp_q15(result));
    }
}

const struct elek_machine karst_machine = {
    .id = 100, .name = "KARST", .short_name = "KARS", ELEK_KNOBS(knobs),
    .pages = {
        { "MATTER", { [ELEK_A] = &knobs[MTRL], [ELEK_B] = &knobs[STIF],
                       [ELEK_C] = &knobs[SPRK], [ELEK_D] = &knobs[INH],
                       [ELEK_E] = &knobs[BOW], [ELEK_F] = &knobs[EDGE],
                       [ELEK_G] = &knobs[DAMP], [ELEK_H] = &knobs[DECY] } },
        { "RESON",  { [ELEK_A] = &knobs[TONE], [ELEK_B] = &knobs[HIT],
                       [ELEK_C] = &knobs[NOIS], [ELEK_D] = &knobs[RASP],
                       [ELEK_E] = &knobs[SPRD], [ELEK_F] = &knobs[BODY],
                       [ELEK_G] = &knobs[LVL] } }
    },
    .render = karst_render
};
ELEK_REGISTER_MACHINE(karst_machine);
