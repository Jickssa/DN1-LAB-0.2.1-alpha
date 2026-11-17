/* SPDX-License-Identifier: GPL-2.0-or-later
 * MORPHANT — Formant Organism. Shared machine SDK alpha / Digitone MKI OS 1.44.
 */
#include <stdint.h>
#include "elekloader/sdk_alpha_machine.h"
#include "../common/phase_sine.h"

enum { VOWL, FORM, FINE, HARM, BRETH, NOIS, DRFT, GLID,
       NASL, AIR, SUB, OCT, BITE, FORM2, MIX };
enum { PARTIALS = 6, Q15_ONE = 32767 };

struct morphant_state {
    uint32_t phase;
    uint32_t vibrato;
    uint32_t breath_rng;
    int32_t morph;
    int32_t breath_lp;
    int32_t air_lp;
    uint32_t initialized;
};

static const uint32_t harmonic_phase_limit[PARTIALS] = {
    0x7fffffffu, 0x3fffffffu, 0x2aaaaaaau,
    0x1fffffffu, 0x19999999u, 0x15555555u
};

static const uint16_t vowel[5][PARTIALS] = {
    { 3500,  1700, 22000, 28000, 10500,  3000 },
    { 6000, 24000,  6500, 18500,  6500,  1800 },
    { 26000, 5000, 14500,  4500,  2500,   800 },
    { 22500, 8000, 1800, 13000, 6200, 1800 },
    { 26000, 2200, 900, 3200, 12500, 2800 }
};

static uint32_t random32(struct morphant_state *s)
{
    uint32_t x = s->breath_rng;
    if (!x) x = 0x243f6a88u;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s->breath_rng = x;
    return x;
}

static int32_t clamp_q15(int32_t x)
{
    if (x > Q15_ONE) { DN1_LAB_CLIP_HIT(x); return Q15_ONE; }
    if (x < -Q15_ONE) { DN1_LAB_CLIP_HIT(x); return -Q15_ONE; }
    return x;
}


static const struct elek_knob knobs[] = {
    [VOWL]  = { "VOWL",  .min = 0, .max = 4, .def = 0 },
    [FORM]  = { "FORM",  .max = 127, .def = 63 },
    [FINE]  = { "FINE",  .min = -100, .max = 100, .def = 2 },
    [HARM]  = { "HARM",  .max = 127, .def = 64 },
    [BRETH] = { "BRETH", .max = 127, .def = 0 },
    [NOIS]  = { "NOIS",  .max = 127, .def = 18 },
    [DRFT]  = { "DRFT",  .max = 127, .def = 12 },
    [GLID]  = { "GLID",  .max = 127, .def = 24 },
    [NASL]  = { "NASL",  .max = 127, .def = 64 },
    [AIR]   = { "AIR",   .max = 127, .def = 12 },
    [SUB]   = { "SUB",   .max = 127, .def = 16 },
    [OCT]   = { "OCT",   .min = -12, .max = 12, .def = 0 },
    [BITE]  = { "BITE",  .max = 127, .def = 18 },
    [FORM2] = { "FM2",   .max = 127, .def = 64 },
    [MIX]   = { "MIX",   .max = 127, .def = 112 }
};

static void morphant_render(const struct elek_voice *v, elek_sample *out)
{
    struct morphant_state *s = ELEK_STATE(struct morphant_state, v);
    int32_t target = v->knob[VOWL] * 128;
    int32_t form = v->knob[FORM], fine = v->knob[FINE];
    int32_t harmonics = v->knob[HARM], breath = v->knob[BRETH];
    int32_t noise = v->knob[NOIS], drift = v->knob[DRFT];
    int32_t glide = v->knob[GLID], nasal = v->knob[NASL];
    int32_t air = v->knob[AIR], sub = v->knob[SUB];
    int32_t octave = v->knob[OCT], bite = v->knob[BITE];
    int32_t form2 = v->knob[FORM2], mix = v->knob[MIX];
    int32_t vowel_a, vowel_b, frac, weights[PARTIALS], total = 0;
    int32_t sub_scale_q8, mix_scale_q8, mix_inverse_q8;
    int32_t semi;
    uint32_t inc, p, max_inc;
    int32_t partial_count = PARTIALS;
    int32_t i, h;

    if (v->start || !s->initialized) {
        s->phase = 0;
        s->vibrato = 0;
        s->morph = target;
        s->breath_lp = 0;
        s->air_lp = 0;
        s->breath_rng = 0xb5297a4du ^ (v->track * 0x68e31da4u) ^ (uint32_t)v->pitch;
        s->initialized = 1;
    } else {
        int32_t step = 1 + ((127 - glide) * 12 / 127);
        if (s->morph < target) s->morph += (target - s->morph < step) ? target - s->morph : step;
        else if (s->morph > target) s->morph -= (s->morph - target < step) ? s->morph - target : step;
    }

    sub_scale_q8 = (sub * 256) / (127 * 5);
    mix_scale_q8 = (mix * 256) / 127;
    mix_inverse_q8 = 256 - mix_scale_q8;

    vowel_a = s->morph >> 7;
    frac = s->morph & 127;
    if (vowel_a >= 4) { vowel_a = 4; vowel_b = 4; frac = 0; }
    else vowel_b = vowel_a + 1;

    for (h = 0; h < PARTIALS; ++h) {
        int32_t a = vowel[vowel_a][h], b = vowel[vowel_b][h];
        int32_t val = a + ((b - a) * frac) / 128;
        int32_t tilt = 64 + (harmonics * (h + 1)) / 6;
        int32_t center = 1 + (form * 4) / 127;
        int32_t distance = (h + 1) > center ? (h + 1) - center : center - (h + 1);
        int32_t ridge = 127 - distance * (8 + form * 28 / 127);
        if (ridge < 24) ridge = 24;
        val = val * tilt / 127;
        val = val * ridge / 127;
        if (h == 2 || h == 3) val += val * nasal / 200;
        if (form2 && h == 4) val += val * form2 / 180;
        if (val < 0) val = 0;
        weights[h] = val;
        total += val;
    }
    if (total < 1) total = 1;
    for (h = 0; h < PARTIALS; ++h) weights[h] = ((weights[h] * 16383) / total) * 2;
    total = 0;
    for (h = 0; h < PARTIALS; ++h) total += weights[h];
    weights[0] += Q15_ONE - total;

    semi = octave * 65536 + (fine * 65536) / 100;
    inc = elek_phase_inc(v->pitch + semi);
    max_inc = inc + (uint32_t)dn1_vibrato_delta_q16(inc, 255, drift);
    /* A partial whose highest possible increment exceeds Nyquist is omitted.
     * Resolve this once per block: no variable divide in the sample loop. */
    for (h = 0; h < PARTIALS; ++h) {
        if (max_inc > harmonic_phase_limit[h]) {
            partial_count = h;
            break;
        }
    }
    p = s->phase;

    for (i = 0; i < ELEK_VOICE_FRAMES; ++i) {
        int32_t sample = 0;
        int32_t vib = dn1_sin_q15(s->vibrato) / 128;
        int32_t delta = dn1_vibrato_delta_q16(inc, vib, drift);
        uint32_t sample_inc = inc + (uint32_t)delta;
        for (h = 0; h < partial_count; ++h) {
            int32_t part;
            uint32_t ph = p * (uint32_t)(h + 1);
            part = dn1_sin_q15(ph);
            sample += (part * weights[h]) >> 15;
        }
        if (sub_scale_q8)
            sample += (dn1_sin_q15(p >> 1) * sub_scale_q8) / 256;
        {
            int32_t texture = 0;
            if (breath || noise || air) {
                int32_t n = (int32_t)(random32(s) >> 17) - 16384;
                s->breath_lp += (n - s->breath_lp) / 16;
                s->air_lp += (n - s->air_lp) / 4;
                if (breath) texture += (s->breath_lp * breath) / (127 * 8);
                if (noise) texture += (n * noise) / (127 * 8);
                if (air) texture += ((n - s->air_lp) * air) / (127 * 16);
            }
            sample = (sample * mix_scale_q8 + texture * mix_inverse_q8) / 256;
        }
        if (bite) {
            int32_t gain = 127 + bite * 3;
            sample = (sample * gain) >> 7;
            sample = dn1_fold_q15(sample);
        }
        /* LVL was intentionally removed: overall level belongs to the native
         * Digitone AMP page. A fixed half-scale headroom avoids a hot default. */
        sample /= 2;
        out[i] = elek_from_q15(clamp_q15(sample));
        p += sample_inc;
        s->vibrato += 268435u; /* 3 Hz at 48 kHz */
    }
    s->phase = p;
}

const struct elek_machine morphant_machine = {
    .id = 101, .name = "MORPHANT", .short_name = "MORP", ELEK_KNOBS(knobs),
    .pages = {
        { "VOICE", { [ELEK_A] = &knobs[VOWL], [ELEK_B] = &knobs[FORM],
                      [ELEK_C] = &knobs[FINE], [ELEK_D] = &knobs[HARM],
                      [ELEK_E] = &knobs[BRETH], [ELEK_F] = &knobs[NOIS],
                      [ELEK_G] = &knobs[DRFT], [ELEK_H] = &knobs[GLID] } },
        { "SPECT", { [ELEK_A] = &knobs[NASL], [ELEK_B] = &knobs[AIR],
                      [ELEK_C] = &knobs[SUB], [ELEK_D] = &knobs[OCT],
                      [ELEK_E] = &knobs[BITE], [ELEK_F] = &knobs[FORM2],
                      [ELEK_G] = &knobs[MIX] } }
    },
    .render = morphant_render
};
ELEK_REGISTER_MACHINE(morphant_machine);
