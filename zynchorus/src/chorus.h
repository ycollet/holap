/*
  ZynAddSubFX - a software synthesizer
 
  Chorus.h - Chorus and Flange effects
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for ZynChorus by Josep Andreu

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef CHORUS_H
#define CHORUS_H

#include <ladspa.h>

#define F2I(f,i) (i)=((f>0) ? ( (int)(f) ) :( (int)(f-1.0) ));
#define RND (rand()/(RAND_MAX+1.0))


#define MAX_CHORUS_DELAY 250.0	//ms


  typedef struct
  {
  float xl, xr;
  float incx;
  float ampl1, ampl2, ampr1, ampr2;
  float lfointensity;
  float lfornd;
  float lfofreq;
  float stereo;
  } EffectLFO;


  typedef struct
  { 
  int dlk, drk, dlhi, dlhi2;
  int maxdelay;
  unsigned long SAMPLE_RATE;
  LADSPA_Data *efxoutl;
  LADSPA_Data *efxoutr;
  LADSPA_Data *inputl;
  LADSPA_Data *inputr;
  LADSPA_Data *outvolume;	
  LADSPA_Data *dry; 
  LADSPA_Data *depth;
  LADSPA_Data *panning;
  LADSPA_Data *fb;
  LADSPA_Data *lrcross;
  LADSPA_Data *delay;
  LADSPA_Data *lfofreq;
  LADSPA_Data *lfornd;
  LADSPA_Data *PLFOtype;
  LADSPA_Data *Pstereo;
  LADSPA_Data *Poutsub;
  EffectLFO lfo;		
  float dl1, dl2, dr1, dr2, lfol, lfor;
  float Rdelay;
  float Rdepth;
  float indatal[4096];
  float indatar[4096];
  float dllo, mdel;
  float delayl[24000];
  float delayr[24000];
  } ZChorus_t;
  
  

  void Chorus_Init (ZChorus_t * s);
  void out (ZChorus_t * s, float * smpsl, float * smpsr, unsigned long count);
  void Chorus_cleanup(ZChorus_t * s);
  float getdelay (ZChorus_t * s, float xlfo);

  void EffectLFO_Init(ZChorus_t * s);
  void effectlfoout (ZChorus_t * s, float * outl, float * outr, unsigned long count);
  float getlfoshape (ZChorus_t * s, float x);


#endif

