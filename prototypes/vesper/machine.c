/* SPDX-License-Identifier: GPL-2.0-or-later
 * VESPER — Resonant Time Loom. Host prototype; ID 103 is provisional.
 * A short 32-cell signed excitation stencil feeds two damped resonant poles.
 * No sample asset, heap, file I/O or cross-voice state.
 */
#include <stdint.h>
#include "../common/proto_dsp.h"

enum { TRACE, GRAIN, DRIFT, REV, FREEZ, MODES, DECAY, FRAY,
       BOW, STRIKE, TILT, WIDTH, FEED, DAMP, MIX };
struct vesper_state {
    int8_t stencil[32]; int32_t y1[2], y2[2];
    uint32_t read_q16, rng; uint8_t write_pos, initialized;
};
typedef char vesper_state_must_fit[(sizeof(struct vesper_state) <= ELEK_VOICE_STATE_MAX) ? 1 : -1];
const uint32_t dn1p_vesper_state_bytes=(uint32_t)sizeof(struct vesper_state);
static const struct elek_knob knobs[] = {
 [TRACE]={"TRACE",.max=127,.def=82}, [GRAIN]={"GRAIN",.max=127,.def=38},
 [DRIFT]={"DRIFT",.max=127,.def=22}, [REV]={"REV",.max=1,.def=0},
 [FREEZ]={"FREEZ",.max=127,.def=0}, [MODES]={"MODES",.min=1,.max=2,.def=2},
 [DECAY]={"DECAY",.max=127,.def=104}, [FRAY]={"FRAY",.max=127,.def=18},
 [BOW]={"BOW",.max=127,.def=0}, [STRIKE]={"STRIK",.max=127,.def=96},
 [TILT]={"TILT",.min=-64,.max=63,.def=0}, [WIDTH]={"WIDTH",.max=127,.def=64},
 [FEED]={"FEED",.max=127,.def=28}, [DAMP]={"DAMP",.max=127,.def=38},
 [MIX]={"MIX",.max=127,.def=104}
};
static void vesper_render(const struct elek_voice *v, elek_sample *out)
{
    struct vesper_state *s=ELEK_STATE(struct vesper_state,v);
    const int32_t trace=v->knob[TRACE], grain=v->knob[GRAIN], drift=v->knob[DRIFT];
    const int32_t reverse=v->knob[REV], freeze=v->knob[FREEZ], modes=v->knob[MODES];
    const int32_t decay=v->knob[DECAY], fray=v->knob[FRAY], bow=v->knob[BOW];
    const int32_t strike=v->knob[STRIKE], tilt=v->knob[TILT], width=v->knob[WIDTH];
    const int32_t feed=v->knob[FEED], damp=v->knob[DAMP], mix=v->knob[MIX];
    int32_t radius=29400+decay*25-damp*7, r2, coeff[2], weights[2];
    uint32_t speed, i;
    if (radius>32300) radius=32300;
    if (radius<28000) radius=28000;
    r2=(radius*radius)>>15;
    {
        int32_t semi0=(tilt*2*65536)/127;
        uint32_t inc0=dn1p_phase_at(v->pitch,semi0);
        uint32_t inc1=dn1p_phase_at(v->pitch,semi0+(4+width/12)*65536);
        int32_t c0=dn1_sin_q15(inc0+0x40000000u), c1=dn1_sin_q15(inc1+0x40000000u);
        coeff[0]=(2*radius*c0)>>15; coeff[1]=(2*radius*c1)>>15;
    }
    weights[0]=127-width/2; weights[1]=width/2;
    if (weights[0]<1) weights[0]=1;
    speed=16384u+(uint32_t)grain*114688u/127u;
    if (v->start || !s->initialized) {
        uint32_t j;
        for (j=0;j<32;j++) s->stencil[j]=0;
        s->y1[0]=s->y1[1]=s->y2[0]=s->y2[1]=0;
        s->read_q16=0; s->write_pos=0;
        s->rng=0x91e10da5u^((uint32_t)v->pitch*0x45d9f3bu)^(v->track*0x9e3779b9u);
        s->initialized=1;
    }
    for (i=0;i<ELEK_VOICE_FRAMES;i++) {
        int32_t excitation=0, stored, delayed, mode_sum=0;
        uint32_t ix=(s->read_q16>>16)&31u, frac=s->read_q16&0xffffu;
        int32_t a=s->stencil[ix], b=s->stencil[(ix+1u)&31u];
        if (i==0 && v->start) excitation+=(v->velocity*(16+strike))/8;
        if (bow) excitation+=dn1p_abs_noise(&s->rng,bow/2+1);
        if (trace) excitation+=dn1p_abs_noise(&s->rng,trace/3+1);
        stored=dn1p_clamp_i32((excitation+(a*128*feed)/127)/32,-127,127);
        if (freeze<127) {
            int32_t old=s->stencil[s->write_pos];
            s->stencil[s->write_pos]=(int8_t)dn1p_clamp_i32((old*freeze+stored*(127-freeze))/127,-127,127);
            s->write_pos=(uint8_t)((s->write_pos+1u)&31u);
        }
        delayed=(a*(int32_t)(65536u-frac)+b*(int32_t)frac)/65536;
        delayed*=128;
        if (fray) delayed=dn1_quantize_pow2(delayed,1+(fray*4)/127);
        {
            uint32_t m;
            for (m=0;m<2;m++) {
                int32_t branch=(m ? 127-weights[0]+width/4 : weights[0]+width/4);
                int32_t drive=(delayed*branch)/160;
                int32_t y=((coeff[m]*s->y1[m])>>15)-((r2*s->y2[m])>>15)+drive;
                y=dn1p_clamp_i32(y,-22000,22000);
                s->y2[m]=s->y1[m]; s->y1[m]=y;
                if (m==0 || modes>1) mode_sum+=y*weights[m]/127;
            }
        }
        {
            int32_t modal=mode_sum/(modes>1?2:1), dry=delayed/3;
            int32_t sample=dn1p_mix_q15(dry,modal,mix);
            int32_t wander=(int32_t)((dn1p_rng(&s->rng)>>27)&31u)-16;
            int32_t step=(int32_t)speed+wander*drift*8;
            if (step<4096) step=4096;
            if (step>147456) step=147456;
            s->read_q16+=(uint32_t)((reverse?-1:1)*step);
            s->read_q16&=0x1fffffu;
            out[i]=elek_from_q15(dn1p_clamp_q15(sample));
        }
    }
}
const struct elek_machine vesper_machine={
 .id=103,.name="VESPER",.short_name="VESP",ELEK_KNOBS(knobs),
 .pages={
  {"MEMORY",{[ELEK_A]=&knobs[TRACE],[ELEK_B]=&knobs[GRAIN],[ELEK_C]=&knobs[DRIFT],[ELEK_D]=&knobs[REV],[ELEK_E]=&knobs[FREEZ],[ELEK_F]=&knobs[MODES],[ELEK_G]=&knobs[DECAY],[ELEK_H]=&knobs[FRAY]}},
  {"EXCITE",{[ELEK_A]=&knobs[BOW],[ELEK_B]=&knobs[STRIKE],[ELEK_C]=&knobs[TILT],[ELEK_D]=&knobs[WIDTH],[ELEK_E]=&knobs[FEED],[ELEK_F]=&knobs[DAMP],[ELEK_G]=&knobs[MIX]}}
 },.render=vesper_render
};
ELEK_REGISTER_MACHINE(vesper_machine);
