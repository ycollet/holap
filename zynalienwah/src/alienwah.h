/*
  ZynAddSubFX - a software synthesizer
 
  Alienwah.h - "AlienWah" effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for ZynAlienwah by Josep Andreu

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

#ifndef ALIENWAH_H
#define ALIENWAH_H

#include <ladspa.h>

#define RND (rand()/(RAND_MAX+1.0))

#define MAX_ALIENWAH_DELAY 100

typedef struct 
{
  float a, b;
  
} COMPLEXTYPE;


  typedef struct
  {
  float xl, xr;
  float incx;
  float ampl1, ampl2, ampr1, ampr2;
  float lfornd;
  float lfofreq;
  float stereo;
  } EffectLFO;


  typedef struct
  {
  int oldk;
  int k;
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
  LADSPA_Data *Phase;

  float Rfb;
  float Rdelay;
  
  float indatal[4096];
  float indatar[4096];
    
  COMPLEXTYPE oldclfol, oldclfor;
  COMPLEXTYPE oldl[ MAX_ALIENWAH_DELAY];
  COMPLEXTYPE oldr[ MAX_ALIENWAH_DELAY];

  EffectLFO lfo;

  } ZAlienwah_t;



  void Alienwah_Init(ZAlienwah_t * s);
  void out (ZAlienwah_t * s, float * smpsl, float * smpsr, unsigned long count);
  void Alienwah_Cleanup(ZAlienwah_t * s);

  
  void EffectLFO_Init(ZAlienwah_t * s);
  void effectlfoout (ZAlienwah_t * s, float * outl, float * outr, unsigned long count);
  float getlfoshape (ZAlienwah_t * s, float x);








#endif
