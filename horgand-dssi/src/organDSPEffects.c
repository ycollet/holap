/*
  horgand.dssi - a synth-organ plugin software

 organDSPEffects.c  -  DSP Effects functions
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
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/


#include "Holrgan.h"
#include <math.h>


// LFO for chorus

float
Chorus_LFO (horgand_synth_t * s, float *Chorus_X)
{

  float out;

  *Chorus_X += s->a.Chorus_LFO_Speed * s->increment;

  if (*Chorus_X > 1)
{
    *Chorus_X = 0.0f;
}
  out =
    NFsin (s, s->a.Chorus_Wave, *Chorus_X * D_PI) * s->Chorus_LFO_Frequency;

  return (out);


};


// Chorus Effect

void
Calc_Chorus_LFO_Frequency (horgand_synth_t * s)
{
  
  s->Chorus_LFO_Frequency = s->a.modulation * s->a.Chorus_LFO_Amplitude*.5;

};



void
Effect_Chorus (horgand_synth_t * s, int nframes)
{

  long int elkel, elkel2;
  float ldelay1, rdelay1, dell, valorl;
  int i = 0;
  float chor_vol = s->a.Chorus_Volume * .5f;
  float dllo;
  float real_delay = (s->a.Chorus_Delay*s->SAMPLE_RATE *.0003f) + ( s->SAMPLE_RATE * .001f);

  for (i = 0; i < nframes; i++)

    {
      ldelay1 = s->ldelay;
      rdelay1 = s->rdelay;

      // L Channel

      s->ldelay = real_delay + Chorus_LFO(s, &s->Chorus_X_L);

      dell = (ldelay1 * (nframes - i) + s->ldelay * i) / nframes;
      elkel=s->cl_counter - F2I(dell+1.5f);
      if (elkel<0) elkel +=8192;
      if (elkel>=8192) elkel -=8192;
      elkel2=elkel-1;
      if (elkel2<0) elkel2 +=8192;
      if (elkel2>=8192) elkel2 -=8192;
      dllo = 1.0f - fmodf (dell, 1.0f);
      valorl = (dllo * s->cldelay[elkel]) + (s->cldelay[elkel2] * (1 - dllo));
      s->bufl[i] += valorl * chor_vol;
      s->cldelay[s->cl_counter] = s->bufl[i];


      // R Channel
      s->rdelay =real_delay + Chorus_LFO (s, &s->Chorus_X_R);
      
      dell = (rdelay1 * (nframes - i) + s->rdelay * i) / nframes;
      elkel=s->cl_counter - F2I(dell+1.5f);
      if (elkel<0) elkel +=8192;
      if (elkel>=8192) elkel -=8192;
      elkel2=elkel-1;
      if (elkel2<0) elkel2 +=8192;
      if (elkel2>=8192) elkel2 -=8192;

      dllo = 1.0f - fmodf (dell, 1.0f);
      valorl = (dllo * s->crdelay[elkel]) + (s->crdelay[elkel2] * (1 - dllo));
      s->bufr[i] += valorl * chor_vol;
      s->crdelay[s->cl_counter] = s->bufr[i];

      if (++s->cl_counter >= 8192)
	s->cl_counter = 0;


    }


};


// LFO for Rotary


float
Rotary_LFO (horgand_synth_t * s, float t)
{

  float out;

  s->Rotary_X += s->a.Rotary_LFO_Speed * s->increment*.5f;

  if (s->Rotary_X > 1)
    s->Rotary_X = 0.0f;

  out =
    NFsin (s, s->a.Rotary_Wave,
	   s->Rotary_X * D_PI) * s->Rotary_LFO_Frequency;

  return (out);

};


// Rotary Effect


void
Effect_Rotary (horgand_synth_t * s, int nframes)
{
  int i = 0;
  float val, l, r;
  
  s->Rotary_LFO_Frequency =
    s->a.modulation * s->a.Rotary_LFO_Amplitude * s->D_PI_to_SAMPLE_RATE;


  for (i = 0; i < nframes; i++)
    {


      val = Rotary_LFO (s, s->Rotary_X)*.5f;

      l = s->bufl[i];
      r = s->bufr[i];

      s->bufl[i] -= (l * val);
      s->bufr[i] += (r * val);

    }


};


// Reverb Effect

void
Effect_Reverb (horgand_synth_t * s, int nframes)
{
  int i = 0;
  int j;
  int elke, elke1;
  float efxoutl;
  float efxoutr;
  float stmp[16];
  float tmprvol;
  int a_rperhis = s->rperhis;
  float accum = 0;
  memset (stmp, 0, sizeof (stmp));

  for (i = 0; i < 16; i++)
    {
      stmp[i] += (accum + s->Reverb_Diffussion * s->ready_apsg[s->capsg]);
      if (++s->capsg > 14)
	s->capsg = 0;
      accum = stmp[i];
    }

  tmprvol = accum * s->Reverb_Volume * 2.2f;


  for (i = 0; i < nframes; i++)

    {


      efxoutl = 0.0f;
      efxoutr = 0.0f;

      for (j = 0; j < 16; j += 2)
	{
	  elke = a_rperhis - s->a_combl[j];
	  if (elke < 0)
	    elke += 131200;
	  elke1 = a_rperhis - s->a_combr[j];
	  if (elke1 < 0)
	    elke1 += 131200;
	  efxoutl += s->history[elke] * stmp[j];
	  efxoutr += s->history[elke1] * stmp[j + 1];
	}

      s->bufl[i] += (efxoutl * tmprvol);
      s->bufr[i] += (efxoutr * tmprvol);

      a_rperhis += 2;
      if (a_rperhis > 131200)
	a_rperhis -= 131200;

    }
};


// Delay Effect

void
Effect_Delay (horgand_synth_t * s, int nframes)
{
  int i = 0;
  int elke, elke1;
  int delay = (int) s->a.Delay_Delay;
  float voll, volr;
  float Delay_Volumer, Delay_Volumel;
  int a_rperhis = s->rperhis;


  voll = 1 - s->Stereo_Side;
  volr = 1 - voll;
  Delay_Volumel = voll * s->a.Delay_Volume * .5f;
  Delay_Volumer = volr * s->a.Delay_Volume * .5f;


  for (i = 0; i < nframes; i++)

    {
      elke = a_rperhis - delay;
      if (elke % 2 != 0)
	elke++;
      if (elke < 0)
	elke = 131200 + elke;
      elke1 = elke + 1;
      if (elke1 < 0)
	elke1 = 131200 + elke1;

      s->bufl[i] += (s->history[elke] * Delay_Volumel);
      s->bufr[i] += (s->history[elke1] * Delay_Volumer);
      a_rperhis += 2;
      if (a_rperhis > 131200)
	a_rperhis -= 131200;

    }

  switch (s->To_Stereo_Side)
    {
    case 0:
      s->Stereo_Side += 0.01f;
      if (s->Stereo_Side > 1)
	s->To_Stereo_Side = 1;
      break;
    case 1:
      s->Stereo_Side -= 0.01f;
      if (s->Stereo_Side < 0)
	s->To_Stereo_Side = 0;
      break;
    }


};


// Reverb Clean Buffers

void
Clean_Buffer_Effects (horgand_synth_t * s)
{
  memset (s->f, 0, sizeof s->f);
  memset (s->dcphi, 0, sizeof s->dcphi);
  memset (s->history, 0, sizeof (float) * 131200);
  memset (s->cldelay, 0, sizeof (float) * 8192);
  memset (s->crdelay, 0, sizeof (float) * 8192);
};


void
Write_Buffer_Effects (horgand_synth_t * s, int nframes)
{

  int i;

  for (i = 0; i < nframes; i++)
    {
      s->history[s->rperhis] = s->bufl[i];
      if (++s->rperhis > 131200)
	s->rperhis = 0;
      s->history[s->rperhis] = s->bufr[i];
      if (++s->rperhis > 131200)
	s->rperhis = 0;

    }


};
