/*
    Exciter - a MIDI helped exciter DSSI plugin software                                          
    
    exciter.h  -  Exciter header.
    Copyright (C) 2008-2009 Josep Andreu               
    Author: Josep Andreu 


 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef EXCITER_H
#define EXCITER_H

#include <lo/lo.h>
#include "../config.h"
#include <ladspa.h>


#define POLY 8
#define HARMONICS 11
#define M_PI 3.14159265358979323846
#define MAX_PEAKS 8
#define STEP_SIZE 2048
#define LOG_10 2.302585093
#define MAX_FILTER_STAGES 5

#define rap2dB(rap) ((20*log(rap)/LOG_10))


// Recognize Chord


typedef struct
{
  int tipo;
  int num;
  char Nom[16];
} TipoCh;

typedef struct
{
  char Nom[3];
} NomChord;

typedef struct
{
  int note;
  char Nom[3];
} NomChordExtended;

typedef struct
{
  int tipo;
  int fund;
  int di1;
  int di2;
  char Nom[10];
} Ch3;

typedef struct
{
  int tipo;
  int fund;
  int di1;
  int di2;
  int di3;
  char Nom[10];
} Ch4;

typedef struct
{
  int tipo;
  int fund;
  int di1;
  int di2;
  int di3;
  int di4;
  char Nom[10];
} Ch5;


typedef struct
{
  int note_active[POLY];
  int rnote[POLY];
  int gate[POLY];
  int cc;
  int fundi;
  int bass;
  int ctipo;
  int NumChord3;
  int NumChord4;
  int NumChord5;
  char NombreAcorde[20];
  Ch5 Chord5[67];
  Ch4 Chord4[62];
  Ch3 Chord3[17];
  NomChordExtended NCE[25];
  NomChord NC[13];
  TipoCh ChN[34];
  int Ch[35][13];

} RChord;



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
  float freq;		//Frequency given in Hz
  float q;			//Q factor (resonance or Q factor)
  float gain;		//the gain of the filter (if are shelf/peak) filters
  float c[3], d[3];		//coefficients
  float oldc[3], oldd[3];	//old coefficients(used only if some filter paremeters changes very fast, and it needs interpolation)
  float xd[3], yd[3];	//used if the filter is applied more times

   fstage x[6];
   fstage y[6];
   fstage oldx[6];
   fstage oldy[6];
   float ismp[2048];

} AnalogFilter;




// Exciter


typedef struct
{
  lo_server osc_server;
  lo_address m_host;
  char *osc_configure_path;
  int val_sum;
  unsigned long SAMPLE_RATE;

  float itm1l;
  float itm1r;
  float otm1l;
  float otm1r;
  
  LADSPA_Data *inputl;
  LADSPA_Data *inputr;
  LADSPA_Data *efxoutl;
  LADSPA_Data *efxoutr;
  LADSPA_Data *outvolume;
  LADSPA_Data *dry;
  LADSPA_Data *panning;
  LADSPA_Data *mag_1;
  LADSPA_Data *mag_2;
  LADSPA_Data *mag_3;
  LADSPA_Data *mag_4;
  LADSPA_Data *mag_5;
  LADSPA_Data *mag_6;
  LADSPA_Data *mag_7;
  LADSPA_Data *mag_8;
  LADSPA_Data *mag_9;
  LADSPA_Data *mag_10;
  LADSPA_Data *ffreq;


  int harmonic_flist[10];
  int harmonic_nlist[5];
  float Rmag[10];
  
  float rmag_1;
  float rmag_2;
  float rmag_3;
  float rmag_4;
  float rmag_5;
  float rmag_6;
  float rmag_7;
  float rmag_8;
  float rmag_9;
  float rmag_10;
 
  float rfreq;

  float p[HARMONICS];

  float inputbufl[2048];
  float inputbufr[2048];

  RChord rc;
  
  AnalogFilter lhpf;
  AnalogFilter rhpf;
  

} exciter_t;



// Exciter


void Harmonic_Init(exciter_t * s);
void chebpc(float c[], float d[]);
void calcula_mag(exciter_t * s);
void out_exciter(exciter_t *s, float *smpsl, float *smpsr, unsigned long count);

// Recognize Chord

void RecChord_Init (exciter_t * s);
void MiraChord (exciter_t * s);
void IniciaChords (exciter_t * s);
void RecChord_Cleanup (exciter_t * s);
int plus (exciter_t * s, int nota);


void AnalogFilter_Init(exciter_t * s, AnalogFilter *filter, unsigned char Ftype, float Ffreq, float Fq, unsigned char Fstages);
void filterout (exciter_t * s, AnalogFilter *filter, float * smp, unsigned long count);
void setfreq (exciter_t * s, AnalogFilter *filter, float frequency);
void setfreq_and_q (exciter_t * s, AnalogFilter *filter, float frequency, float q_);
void setq (exciter_t * s, AnalogFilter *filter, float q_);
void settype (exciter_t * s, AnalogFilter *filter, int type_);
void setgain (exciter_t * s, AnalogFilter *filter, float dBgain);
void setstages (exciter_t * s, AnalogFilter *filter, int stages_);
void AnalogFilter_Cleanup (exciter_t * s, AnalogFilter *filter);



#endif
