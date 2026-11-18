#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elekloader/sdk_alpha_machine.h"
const char sdk_alpha_abi_v1[] = "host-prototype-demo-sdk";
extern const struct elek_machine vesper_machine, mycel_machine, orbital_machine;
extern const struct elek_machine lithos_machine, photon_machine, anima_machine;
struct fixture { int32_t knobs[ELEK_UI_KNOB_MAX]; uint8_t state[ELEK_VOICE_STATE_MAX]; struct elek_voice v; };
static const struct elek_machine *const machines[]={&vesper_machine,&mycel_machine,&orbital_machine,&lithos_machine,&photon_machine,&anima_machine};
static const int notes[]={36,48,60,72};
static void w16(FILE *f,uint16_t x){fputc(x&255,f);fputc((x>>8)&255,f);}
static void w32(FILE *f,uint32_t x){w16(f,(uint16_t)x);w16(f,(uint16_t)(x>>16));}
static void init(struct fixture *f,const struct elek_machine *m,int note,uint32_t track){uint32_t k;memset(f,0,sizeof(*f));for(k=0;k<m->knob_count;k++)f->knobs[k]=m->knobs[k].def;f->v.knob=f->knobs;f->v.track=track;f->v.start=1;f->v.pitch=note<<16;f->v.velocity=104;f->v.state=f->state;}
static int render_one(const struct elek_machine *m,FILE *fp,int note){
 struct fixture f;int32_t block[32];uint32_t pos,i;const uint32_t note_frames=24000,fade_frames=4800,tail_frames=2400;
 init(&f,m,note,0);
 for(pos=0;pos<note_frames;pos+=32){m->render(&f.v,block);f.v.start=0;for(i=0;i<32;i++){uint32_t at=pos+i;int32_t q=block[i];if(at>=note_frames-fade_frames)q=(q*(int32_t)(note_frames-at))/(int32_t)fade_frames;w16(fp,(uint16_t)(int16_t)(q>>16));}}
 for(pos=0;pos<tail_frames;pos++)w16(fp,0);
 return !ferror(fp);
}
static int write_demo(const struct elek_machine *m,const char *dir){
 char path[512];FILE *fp;const uint32_t frames=4u*(24000u+2400u);uint32_t n;
 if(snprintf(path,sizeof(path),"%s/%s-host-preview.wav",dir,m->name)>=(int)sizeof(path))return 0;
 fp=fopen(path,"wb");if(!fp){perror(path);return 0;}
 fwrite("RIFF",1,4,fp);w32(fp,36u+frames*2u);fwrite("WAVE",1,4,fp);
 fwrite("fmt ",1,4,fp);w32(fp,16);w16(fp,1);w16(fp,1);w32(fp,48000);w32(fp,96000);w16(fp,2);w16(fp,16);
 fwrite("data",1,4,fp);w32(fp,frames*2u);
 for(n=0;n<4;n++)if(!render_one(m,fp,notes[n])){fclose(fp);return 0;}
 if(fclose(fp)!=0){perror(path);return 0;}printf("WAV %s\n",path);return 1;
}
int main(int argc,char **argv){unsigned i;if(argc!=2){fprintf(stderr,"usage: %s output-directory\n",argv[0]);return 2;}for(i=0;i<6;i++)if(!write_demo(machines[i],argv[1]))return 1;return 0;}