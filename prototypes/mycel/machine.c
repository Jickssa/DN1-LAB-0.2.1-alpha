/* SPDX-License-Identifier: GPL-2.0-or-later
 * MYCEL — Distributed Coupling Synth. Host prototype; ID 104 is provisional.
 * Four resonant nodes exchange bounded energy across a fixed six-edge graph.
 */
#include <stdint.h>
#include "../common/proto_dsp.h"
enum { TOPO, LINK, MASS, LOSS, CHAOS, EXCIT, BIAS, CROSS, TENSION, FRIC, DENS, SPREAD, DECAY, BRIGHT, MIX };
struct mycel_state { uint32_t phase[4],rng; int32_t energy[4]; uint8_t initialized; };
typedef char mycel_state_must_fit[(sizeof(struct mycel_state)<=ELEK_VOICE_STATE_MAX)?1:-1];
const uint32_t dn1p_mycel_state_bytes=(uint32_t)sizeof(struct mycel_state);
static const struct elek_knob knobs[]={
 [TOPO]={"TOPO",.max=127,.def=54},[LINK]={"LINK",.max=127,.def=72},
 [MASS]={"MASS",.max=127,.def=64},[LOSS]={"LOSS",.max=127,.def=32},
 [CHAOS]={"CHAOS",.max=127,.def=8},[EXCIT]={"EXCIT",.max=127,.def=54},
 [BIAS]={"BIAS",.min=-64,.max=63,.def=0},[CROSS]={"CROSS",.max=127,.def=82},
 [TENSION]={"TENS",.max=127,.def=64},[FRIC]={"FRIC",.max=127,.def=16},
 [DENS]={"DENS",.max=127,.def=18},[SPREAD]={"SPRD",.max=127,.def=48},
 [DECAY]={"DECAY",.max=127,.def=90},[BRIGHT]={"BRITE",.max=127,.def=62},
 [MIX]={"MIX",.max=127,.def=112}
};
static void mycel_render(const struct elek_voice *v,elek_sample *out)
{
 struct mycel_state *s=ELEK_STATE(struct mycel_state,v);
 static const int16_t ratios[4]={0,7,12,19};
 static const uint8_t ea[6]={0,1,2,3,0,1},eb[6]={1,2,3,0,2,3};
 int32_t topo=v->knob[TOPO],link=v->knob[LINK],mass=v->knob[MASS];
 int32_t loss=v->knob[LOSS],chaos=v->knob[CHAOS],excit=v->knob[EXCIT],bias=v->knob[BIAS];
 int32_t cross=v->knob[CROSS],tension=v->knob[TENSION],fric=v->knob[FRIC];
 int32_t density=v->knob[DENS],spread=v->knob[SPREAD],decay=v->knob[DECAY];
 int32_t bright=v->knob[BRIGHT],mix=v->knob[MIX];
 uint32_t inc[4],i,n; int32_t edge[6],coupling,damp_q15;
 if(v->start||!s->initialized){
   uint32_t seed=0x7f4a7c15u^(v->track*0x9e3779b9u)^(uint32_t)v->pitch;
   s->rng=seed;
   for(n=0;n<4;n++){s->phase[n]=dn1p_rng(&s->rng)+(n*0x18000000u);s->energy[n]=0;}
   s->energy[0]=3000+v->velocity*32;s->energy[1]=s->energy[0]/3;
   s->energy[2]=s->energy[1]/2;s->energy[3]=s->energy[2]/2;s->initialized=1;
 }
 for(n=0;n<4;n++){
   int32_t semis=(int32_t)ratios[n]+(n&1?bias/16:0)+((int32_t)n-1)*spread/48;
   semis+=(n==0?0:((mass-64)*(int32_t)n)/96);
   inc[n]=dn1p_phase_at(v->pitch,semis*65536);
 }
 /* Chain -> ring -> fully connected graph. Weights stay non-negative. */
 edge[0]=127-topo/3;edge[1]=127;edge[2]=127-topo/3;edge[3]=topo;
 edge[4]=topo>64?(topo-64)*2:0;edge[5]=topo>64?(topo-64)*2:0;
 coupling=(link*4096)/127;
 damp_q15=(127-decay)*8+fric*20+loss*12+(127-mass)*6;
 if(damp_q15>6000)damp_q15=6000;
 for(i=0;i<ELEK_VOICE_FRAMES;i++){
   int32_t delta[4]={0,0,0,0},old[4],sum=0;
   for(n=0;n<4;n++)old[n]=s->energy[n];
   for(n=0;n<6;n++){
     int32_t a=ea[n],b=eb[n],coeff=(coupling*edge[n])/127;
     int32_t exchange=((old[b]-old[a])*coeff)>>15;
     delta[a]+=exchange;delta[b]-=exchange;
   }
   for(n=0;n<4;n++){
     int32_t e=old[n]+delta[n];
     e-=(e*damp_q15)>>15;
     if(i==0&&n==0&&v->start)e+=excit*14;
     if(density)e+=dn1p_abs_noise(&s->rng,density/8+1);
     if(chaos)e+=dn1p_abs_noise(&s->rng,chaos/16+1);
     e-=(e*(127-mass)*(n+1))/(127*2048);
     s->energy[n]=dn1p_clamp_i32(e,0,11000);
   }
   for(n=0;n<4;n++){
     int32_t node=dn1_sin_q15(s->phase[n]),weight=64+(bright*(int32_t)n)/3;
     sum+=(node*s->energy[n]>>15)*weight/320;
     s->phase[n]+=inc[n];
   }
   if(cross){
     int32_t coupled=(s->energy[0]+s->energy[3])*cross/127;
     sum+=(dn1_sin_q15(s->phase[1]-s->phase[2])*coupled)>>17;
   }
   if(tension)sum+=((s->energy[0]-s->energy[2])*tension/512)/4;
   sum*=4;
   {
     int32_t noise=dn1p_abs_noise(&s->rng,1+(density/5));
     out[i]=elek_from_q15(dn1p_clamp_q15(dn1p_mix_q15(sum,sum/2+noise,mix)));
   }
 }
}
const struct elek_machine mycel_machine={
 .id=104,.name="MYCEL",.short_name="MYCL",ELEK_KNOBS(knobs),
 .pages={
  {"NETWORK",{[ELEK_A]=&knobs[TOPO],[ELEK_B]=&knobs[LINK],[ELEK_C]=&knobs[MASS],[ELEK_D]=&knobs[LOSS],[ELEK_E]=&knobs[CHAOS],[ELEK_F]=&knobs[EXCIT],[ELEK_G]=&knobs[BIAS],[ELEK_H]=&knobs[CROSS]}},
  {"MATTER",{[ELEK_A]=&knobs[TENSION],[ELEK_B]=&knobs[FRIC],[ELEK_C]=&knobs[DENS],[ELEK_D]=&knobs[SPREAD],[ELEK_E]=&knobs[DECAY],[ELEK_F]=&knobs[BRIGHT],[ELEK_G]=&knobs[MIX]}}
 },.render=mycel_render
};
ELEK_REGISTER_MACHINE(mycel_machine);
