/*
  horgand-dssi - a synth organ plugin software
  
  Holrgan.h  -  Variable Definitions and functions
  Copyright (C) 2004-2008 Josep Andreu (Holborn)
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
#define POLY 16
#define D_PI 6.283185
#define F2I(x)  ( lrintf(x) )


typedef struct
{
  float phi[POLY];
  float dphi;
  float phi2[POLY];
  float dphi2;
} freqVarios;


typedef struct
{
  float harmonic_fine;
  float volumen;
  float con1;
  int harmonic;
  int marimba;
  int wave;
} OperatorPar;


typedef struct
{

  OperatorPar Operator[11];

  char Name[36];

  float Normalize[24];
  float modulation;
  float Delay_Volume;
  float Pitch_LFO_Speed;
  float Pitch_LFO_Delay;
  float Rotary_LFO_Speed;
  float LFOpitch;
  float attack;
  float decay;
  float sustain;
  float p_attack;
  float p_decay;
  float c_attack;
  float c_decay;
  float Click_Vol;
  float Click2_Vol;
  float Click_Freq;
  float Click_Freq2;
  float Click_Vol1;
  float Click_Vol2;
  float detune;
  float Organ_Master_Volume;
  float Delay_Delay;
  float Chorus_Delay;
  float Chorus_LFO_Amplitude;
  float Chorus_LFO_Speed;
  float Chorus_Volume;
  float Rotary_LFO_Amplitude;
  int Speed_Sync;
  int Click;
  int E_Reverb_On;
  int organ_transpose;
  int E_Rotary_On;
  int E_Delay_On;
  int scaling;
  int E_Chorus_On;
  int Reverb_Preset;
  int LFO_Wave;
  int Chorus_Wave;
  int Rotary_Wave;
  char *nombre;

} Todolo;


typedef struct
{
  float f1, f2, f3;
} Menoscalculos;


typedef struct
{

  LADSPA_Data *output_l;
  LADSPA_Data *output_r;
  LADSPA_Data *vol;
  LADSPA_Data *tune;

  Todolo a;
  freqVarios f[11];
  Menoscalculos h[192];
  int k[11];
  float total_vol;
  float Click_sFreq;
  float Click_2sFreq;
  float p_op[11];
  float p_op2[11];
  float organ_master;
  float ldelay, rdelay;
  float lsin[6290];
  float nsin[6290];
  float msin[6290];
  float psin[6290];
  float qsin[6290];
  float rsin[6290];
  float tsin[6290];
  float ssin[6290];
  float usin[6290];
  float bufl[4096];
  float bufr[4096];
  float history[131200];
  float cldelay[8192];
  float crdelay[8192];
  float release;
  float p_release;
  float u_attack;
  float u_p_attack;
  float u_decay;
  float u_p_decay;
  float u_release;
  float u_p_release;
  float u_c_attack;
  float u_c_decay;
  float dcphi[POLY];
  float dcphi2[POLY];
  unsigned int SAMPLE_RATE;
  float velocity[POLY];
  float pitch;
  float env_time[POLY];
  int note[POLY];
  int gate[POLY];
  float Envelope_Volume[POLY];
  float Perc_Volume[POLY];
  int note_active[POLY];
  float lasfreq[24];
  float Master_Volume;
  int pedal;
  int rperhis;
  int transpose;
  float Stereo_Side;
  int To_Stereo_Side;
  float LFO_Volume;
  float LFO_Frequency;
  float Rotary_LFO_Frequency;
  float Rotary_X;
  float Chorus_X_R;
  float Chorus_X_L;
  float Chorus_LFO_Frequency;
  long int cl_counter;
  float increment;
  float D_PI_to_SAMPLE_RATE;
  int Reverb_Time;
  float Reverb_Diffussion;
  float Reverb_Volume;
  int combl[16];
  int combr[16];
  int a_combl[16];
  int a_combr[16];
  int apsg[16];
  float ready_apsg[16];
  int capsg;
  int apss;
  int preset;
  char BankFilename[128];
  int Nums;
  Todolo Banco[33];

} horgand_synth_t;

float NFsin (horgand_synth_t * s, int i, float x);
void Alg1s (horgand_synth_t * s, int nframes);
float Jenvelope (horgand_synth_t * s, int *note_active, int gate, float t,
		 int nota);
float Penvelope (horgand_synth_t * s, int *note_active, int gate, float t,
		 int nota);
float Cenvelope (horgand_synth_t * s, int *note_active, int gate, float t,
		 int nota);
float Get_Partial (horgand_synth_t * s, int note);
float Chorus_LFO (horgand_synth_t * s, float *t);
float Rotary_LFO (horgand_synth_t * s, float t);
float Pitch_LFO (horgand_synth_t * s, float t);
void Calc_LFO_Frequency (horgand_synth_t * s);
void Calc_Chorus_LFO_Frequency (horgand_synth_t * s);
void Write_Buffer_Effects (horgand_synth_t * s, int nframes);
void midievents (horgand_synth_t * s, snd_seq_event_t * events);
float pitch_Operator (horgand_synth_t * s, int i, int note);
float pitch_Operator2 (horgand_synth_t * s, int i, int note);
void volume_Operator (horgand_synth_t * s, int i, int note);
float Get_Keyb_Level_Scaling (horgand_synth_t * s, int nota);
void Effect_Delay (horgand_synth_t * s, int nframes);
void Effect_Reverb (horgand_synth_t * s, int nframes);
void Clean_Buffer_Effects (horgand_synth_t * s);
void Effect_Rotary (horgand_synth_t * s, int nframes);
void Effect_Chorus (horgand_synth_t * s, int nframes);
void New (horgand_synth_t * s);
void New_Bank (horgand_synth_t * s);
void panic (horgand_synth_t * s);
int loadbank (horgand_synth_t * s, const char *filename);
void Put_Combi_t (horgand_synth_t * s, int i);
void syncadsr (horgand_synth_t * s);
void Adjust_Audio (horgand_synth_t * s);
void PutReverb (horgand_synth_t * s, int i);
void init_vars (horgand_synth_t * s);

#endif

