/*
  goomf - a FM mono synth plugin software
  
  goomf.h  -  Variable Definitions and functions
  Copyright (C) 2009 Josep Andreu (Holborn)
  Author: Josep Andreu
        
 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.
         
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.
                     
 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
                            
*/


#ifndef DXEMU_H
#define DXEMU_H
#include <alsa/asoundlib.h>
#include "../config.h"
#include <ladspa.h>
#define D_PI 6.283185
#define F2I(x)  ( lrintf(x) )
#define MAX_FILTER_STAGES 5

typedef struct
{
  float phi;
  float dphi;
  float phi2;
  float dphi2;
} freqVarios;


typedef struct
{
  float f1, f2, f3;
} Menoscalculos;


typedef struct
{
  char Name[64];
} Todolo;


typedef struct
{
  float c1, c2;
} fstage;


typedef struct
{
  int order;			//the order of the filter (number of poles)
  int type;			//The type of the filter (LPF1,HPF1,LPF2,HPF2...)
  int stages;			//how many times the filter is applied (0->1,1->2,etc.)
  int needsinterpolation, firsttime;
  int abovenq;			//this is 1 if the frequency is above the nyquist
  int oldabovenq;		//if the last time was above nyquist (used to see if it needs interpolation)

  float outgain;
  float freq;			//Frequency given in Hz
  float q;			//Q factor (resonance or Q factor)
  float gain;			//the gain of the filter (if are shelf/peak) filters
  float c[3], d[3];		//coefficients
  float oldc[3], oldd[3];	//old coefficients(used only if some filter paremeters changes very fast, and it needs interpolation)
  float xd[3], yd[3];		//used if the filter is applied more times

  fstage x[6];
  fstage y[6];
  fstage oldx[6];
  fstage oldy[6];
  float ismp[2048];

} AnalogFilter;



typedef struct
{
  pthread_mutex_t mutex;

  LADSPA_Data *output_l;
  LADSPA_Data *output_r;
  LADSPA_Data *master_volume;
  LADSPA_Data *tune;
  LADSPA_Data *algo;
  LADSPA_Data *portamento;
  LADSPA_Data *LFO_Volume;
  LADSPA_Data *LFO_Frequency;
  LADSPA_Data *LFO_Delay;
  LADSPA_Data *LFO_Wave;
  LADSPA_Data *wave[6];
  LADSPA_Data *H[6];
  LADSPA_Data *HF[6];
  LADSPA_Data *Ovol[6];
  LADSPA_Data *attack[6];
  LADSPA_Data *decay[6];
  LADSPA_Data *sustain[6];
  LADSPA_Data *release[6];
  LADSPA_Data *pLFO[6];
  LADSPA_Data *Ftype;
  LADSPA_Data *Fgain;
  LADSPA_Data *Fcutoff;
  LADSPA_Data *Fq;
  LADSPA_Data *FLFO;
  LADSPA_Data *FADSR;
  LADSPA_Data *Fstages;
  LADSPA_Data *Fvelocity;

  int Rtype;
  LADSPA_Data Rgain;
  LADSPA_Data Rcutoff;
  LADSPA_Data Rq;
  int Rstages;
  char Name[64];
  freqVarios f[6];
  Menoscalculos h[192];
  float lsin[6290];
  float nsin[6290];
  float msin[6290];
  float psin[6290];
  float qsin[6290];
  float rsin[6290];
  float tsin[6290];
  float ssin[6290];
  float usin[6290];
  float bufl[8192];
  float bufr[8192];
  unsigned int SAMPLE_RATE;
  float Envelope_Volume[6];
  float Env_Vol[6];
  float FEnv_Vol;
  float lfol;
  float pitch;
  float velocity;
  float env_time;
  float renv_time;
  float modulation;
  int note;
  int lastnote;
  float apartial;
  float lpartial;
  float ppartial;
  int gate;
  float lasfreq[22];
  float increment;
  float D_PI_to_SAMPLE_RATE;
  int preset;
  int active[6];
  int filt_cleared;
  Todolo Banco[80];

  AnalogFilter Fl;
  AnalogFilter Fr;

} goomf_synth_t;

long int lrintf (float x);

float NFsin (goomf_synth_t * s, int i, float x);
void Alg1s (goomf_synth_t * s, int nframes);
float Jenvelope (goomf_synth_t * s, int op);
float Fenvelope (goomf_synth_t * s, int op);
float Get_Partial (goomf_synth_t * s);
float Pitch_LFO (goomf_synth_t * s, float t, int type);
float pitch_Operator (goomf_synth_t * s, int i);
float pitch_Operator2 (goomf_synth_t * s, int i);
void init_vars (goomf_synth_t * s);
void Adjust_Audio(goomf_synth_t * s);
void clear_synth (goomf_synth_t * s, int op);

void AnalogFilter_Init (goomf_synth_t * s, AnalogFilter * filter,
			unsigned char Ftype, float Ffreq, float Fq,
			unsigned char Fstages);
void filterout (goomf_synth_t * s, AnalogFilter * filter, float *smp,
		unsigned long count);
void setfreq (goomf_synth_t * s, AnalogFilter * filter, float frequency);
void setfreq_and_q (goomf_synth_t * s, AnalogFilter * filter, float frequency,
		    float q_);
void setq (goomf_synth_t * s, AnalogFilter * filter, float q_);
void settype (goomf_synth_t * s, AnalogFilter * filter, int type_);
void setgain (goomf_synth_t * s, AnalogFilter * filter, float dBgain);
void setstages (goomf_synth_t * s, AnalogFilter * filter, int stages_);
void AnalogFilter_Cleanup (goomf_synth_t * s, AnalogFilter * filter);
float H (goomf_synth_t * s, AnalogFilter * filter, float freq);	//Obtains the response for a given frequency
void singlefilterout (goomf_synth_t * s, AnalogFilter * filter, float *smp,
		      fstage * x, fstage * y, float *c, float *d,
		      unsigned long count);
void computefiltercoefs (goomf_synth_t * s, AnalogFilter * filter);


#endif
