/* SPDX-License-Identifier: GPL-2.0-or-later
 * ANIMA — Local Ecology Fallback. Host prototype; ID 108 provisional.
 * Four virtual agents interact inside one voice. No shared/global voice state.
 * Cross-voice ecology remains blocked on a documented synchronized SDK API.
 */
#include <stdint.h>
#include "../common/proto_dsp.h"
enum { POPUL, COHES, MUTUAL, PRESS, DENS, TENSION, BIAS, DIFF, MEMORY, DECAY, SYNC, LOCAL, GLOBAL, SEED, MIX };
struct anima_state { uint32_t phase[4],rng; int32_t activity[4],memory[4]; uint8_t initialized; };
typedef char anima_state_must_fit[(sizeof(struct anima_state)<=ELEK_VOICE_STATE_MAX)?1:-1];
const uint32_t dn1p_anima_state_bytes=(uint32_t)sizeof(struct anima_state);
static const struct elek_knob knobs[]={
 [POPUL]={"POPUL",.max=127,.def=56},[COHES]={"COHES",.max=127,.def=78},
 [MUTUAL]={"MUTL",.max=127,.def=44},[PRESS]={"PRESS",.max=127,.def=38},
 [DENS]={"DENS",.max=127,.def=28},[TENSION]={"TENS",.max=127,.def=50},
 [BIAS]={"BIAS",.min=-64,.max=63,.def=0},[DIFF]={"DIFF",.max=127,.def=18},
 [MEMORY]={"MEM",.max=127,.def=72},[DECAY]={"DECAY",.max=127,.def=64},
 [SYNC]={"SYNC",.max=127,.def=42},[LOCAL]={"LOCAL",.max=127,.def=110},
 [GLOBAL]={"GLOB",.max=127,.def=32},[SEED]={"SEED",.max=127,.def=17},
 [MIX]={"MIX",.max=127,.def=112}
};
static void anima_render(const struct elek_voice *v,elek_sample *out)
{
 struct anima_state *s=ELEK_STATE(struct anima_state,v);
 static const int16_t intervals[4]={0,7,12,19};
 int32_t popul=v->knob[POPUL],cohes=v->knob[COHES],mutual=v->knob[MUTUAL];
 int32_t press=v->knob[PRESS],density=v->knob[DENS],tension=v->knob[TENSION];
 int32_t bias=v->knob[BIAS],diff=v->knob[DIFF],memory=v->knob[MEMORY];
 int32_t decay=v->knob[DECAY],sync=v->knob[SYNC],local=v->knob[LOCAL];
 int32_t global=v->knob[GLOBAL],seed=v->knob[SEED],mix=v->knob[MIX];
 uint32_t inc[4],i,n;
 if(v->start||!s->initialized){
  s->rng=0xa4093822u^(v->track*0x9e3779b9u)^((uint32_t)v->pitch*0x85ebca6bu)^(uint32_t)seed*0x27d4eb2du;
  for(n=0;n<4;n++){
   s->phase[n]=n*0x20000000u+(dn1p_rng(&s->rng)&0x0fffffffu);
   s->activity[n]=n==0?2600+v->velocity*20:400;s->memory[n]=s->activity[n];
  }
  s->initialized=1;
 }
 for(n=0;n<4;n++){
  int32_t p=v->pitch+(intervals[n]+bias/8+(int32_t)n*tension/48)*65536;
  if(p<0) p=0;
  if(p>(127<<16)) p=127<<16;
  inc[n]=elek_phase_inc(p);if(inc[n]>0x7fffffffu)inc[n]=0;
 }
 for(i=0;i<ELEK_VOICE_FRAMES;i++){
  int32_t prior[4],next[4],sum=0;
  for(n=0;n<4;n++)prior[n]=s->activity[n];
  for(n=0;n<4;n++){
   uint32_t left=(n+3u)&3u,right=(n+1u)&3u;
   int32_t neighbours=(prior[left]+prior[right])/2;
   int32_t pull=(neighbours-prior[n])*(mutual+cohes/2)/2048;
   int32_t retain=(s->memory[n]-prior[n])*memory/8192;
   int32_t lose=prior[n]*((127-decay)+press/3)/16000;
   int32_t stochastic=diff?dn1p_abs_noise(&s->rng,diff/18+1):0,injection=0;
   if(n==0&&i==0&&v->start)injection=popul*8;
   if(density&&((dn1p_rng(&s->rng)&0x7fffu)<(uint32_t)density*256u))injection+=density*2;
   next[n]=dn1p_clamp_i32(prior[n]+pull+retain-lose+stochastic+injection,0,8500);
  }
  for(n=0;n<4;n++){
   s->activity[n]=next[n];s->memory[n]+=(next[n]-s->memory[n])*(local+1)/1024;
   if(s->memory[n]<0) s->memory[n]=0;
   if(s->memory[n]>8500) s->memory[n]=8500;
   {
    int32_t wave=dn1_sin_q15(s->phase[n]),amp=next[n]*(64+global/2)/128;
    sum+=(wave*amp)>>15;
   }
   s->phase[n]+=inc[n];
  }
  if(sync){
   int32_t phase01=dn1p_phase_delta(s->phase[0],s->phase[1]);
   int32_t phase23=dn1p_phase_delta(s->phase[2],s->phase[3]);
   s->phase[1]+=(uint32_t)(((phase01/256)*sync)/2048);
   s->phase[3]+=(uint32_t)(((phase23/256)*sync)/2048);
  }
  if(tension)sum+=(s->activity[0]-s->activity[2])*tension/512;
  sum=(sum*3)/4;sum=dn1p_mix_q15(sum,sum/2,mix);
  out[i]=elek_from_q15(dn1p_clamp_q15(sum));
 }
}
const struct elek_machine anima_machine={
 .id=108,.name="ANIMA",.short_name="ANIM",ELEK_KNOBS(knobs),
 .pages={
  {"ECOLOGY",{[ELEK_A]=&knobs[POPUL],[ELEK_B]=&knobs[COHES],[ELEK_C]=&knobs[MUTUAL],[ELEK_D]=&knobs[PRESS],[ELEK_E]=&knobs[DENS],[ELEK_F]=&knobs[TENSION],[ELEK_G]=&knobs[BIAS],[ELEK_H]=&knobs[DIFF]}},
  {"MEMORY",{[ELEK_A]=&knobs[MEMORY],[ELEK_B]=&knobs[DECAY],[ELEK_C]=&knobs[SYNC],[ELEK_D]=&knobs[LOCAL],[ELEK_E]=&knobs[GLOBAL],[ELEK_F]=&knobs[SEED],[ELEK_G]=&knobs[MIX]}}
 },.render=anima_render
};
ELEK_REGISTER_MACHINE(anima_machine);
