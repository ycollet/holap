/*
  ZynAddSubFX - a software synthesizer
 
  Echo.C - Echo effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Josep Andreu
  
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mdelay.h"

void
MusicDelay_Init(MDelay_t * s)
{

MusicDelay_Cleanup(s);

}

/*
 * Cleanup the effect
 */
void
MusicDelay_Cleanup(MDelay_t * s)
{
  int i;
  for (i = 0; i < DELAY_LENGTH; i++)
    s->ldelay1[i] = 0.0;
  for (i = 0; i < DELAY_LENGTH; i++)
    s->rdelay1[i] = 0.0;
  for (i = 0; i < DELAY_LENGTH; i++)
    s->ldelay2[i] = 0.0;
  for (i = 0; i < DELAY_LENGTH; i++)
    s->rdelay2[i] = 0.0;

  s->oldl1 = 0.0;
  s->oldr1 = 0.0;
  s->oldl2 = 0.0;
  s->oldr2 = 0.0;
}


/*
 * Initialize the delays
 */
void
MusicDelay_initdelays (MDelay_t * s)
{

  s->kl1 = 0;
  s->kr1 = 0;
  s->dl1 = s->delay1;
  if (s->dl1 < 1)
    s->dl1 = 1;
  s->dr1 = s->delay1;
  if (s->dr1 < 1)
    s->dr1 = 1;
  s->kl2 = 0;
  s->kr2 = 0;
  s->dl2 = s->delay2 + s->lrdelay;
  if (s->dl2 < 1)
    s->dl2 = 1;
  s->dr2 = s->delay2 + s->lrdelay;
  if (s->dr2 < 1)
    s->dr2 = 1;

  MusicDelay_Cleanup (s);
}

/*
 * Effect output
 */
void
out (MDelay_t * s, float * smpsl, float * smpsr, unsigned long count)
{
  int i;
  float l1, r1, ldl1, rdl1, l2, r2, ldl2, rdl2;

  LADSPA_Data gain1 = *(s->gain1);
  LADSPA_Data gain2 = *(s->gain2);
  LADSPA_Data panning1 = *(s->Ppanning1);
  LADSPA_Data panning2 = *(s->Ppanning2);
  LADSPA_Data fb1 = *(s->Pfb1);
  LADSPA_Data fb2 = *(s->Pfb2);
  LADSPA_Data *const efxoutl = s->efxoutl;
  LADSPA_Data *const efxoutr = s->efxoutr;  
  LADSPA_Data hidamp = *(s->Phidamp);
  LADSPA_Data vol = *(s->outvolume);
  LADSPA_Data tempo = *(s->Ptempo);
  LADSPA_Data Pdelay1 = *(s->Pdelay1);
  LADSPA_Data Pdelay2 = *(s->Pdelay2);
  LADSPA_Data Plrdelay = *(s->Plrdelay);
    

  if (s->Rtempo != tempo)
     {
       s->Rtempo = tempo;
       MusicDelay_settempo(s,s->Rtempo);
     }  

  if (s->RPdelay1 != Pdelay1)
     {
       s->RPdelay1 = Pdelay1;
       MusicDelay_setdelay(s);
     }  
 if (s->RPdelay2 != Pdelay2)
     {
       s->RPdelay2 = Pdelay2;
       MusicDelay_setdelay(s);
     }  
 if (s->RPlrdelay != Plrdelay)
     {
       s->RPlrdelay = Plrdelay;
       MusicDelay_setdelay(s);
     }  


  for (i = 0; i < count; i++)
    {
      ldl1 = s->ldelay1[s->kl1];
      rdl1 = s->rdelay1[s->kr1];
      l1 = ldl1 * (1.0 - s->lrcross) + rdl1 * s->lrcross;
      r1 = rdl1 * (1.0 - s->lrcross) + ldl1 * s->lrcross;
      ldl1 = l1;
      rdl1 = r1;

      ldl2 = s->ldelay2[s->kl2];
      rdl2 = s->rdelay2[s->kr2];
      l2 = ldl2 * (1.0 - s->lrcross) + rdl2 * s->lrcross;
      r2 = rdl2 * (1.0 - s->lrcross) + ldl2 * s->lrcross;
      ldl2 = l2;
      rdl2 = r2;

      ldl1 = smpsl[i] * gain1 * panning1 - ldl1 * fb1;
      rdl1 = smpsr[i] * gain1 * (1.0 - panning1) - rdl1 * fb1;

      ldl2 = smpsl[i] * gain2 * panning2 - ldl2 * fb2;
      rdl2 = smpsr[i] * gain2 * (1.0 - panning2) - rdl2 * fb2;

      efxoutl[i] = (ldl1 + ldl2) * 2.0 * vol;
      efxoutr[i] = (rdl1 + rdl2) * 2.0 * vol;



      //LowPass Filter
      s->ldelay1[s->kl1] = ldl1 = ldl1 * hidamp + s->oldl1 * hidamp;
      s->rdelay1[s->kr1] = rdl1 = rdl1 * hidamp + s->oldr1 * hidamp;
      s->oldl1 = ldl1;
      s->oldr1 = rdl1;

      s->ldelay2[s->kl2] = ldl2 = ldl2 * hidamp + s->oldl2 * hidamp;
      s->rdelay2[s->kr2] = rdl2 = rdl2 * hidamp + s->oldr2 * hidamp;
      s->oldl2 = ldl2;
      s->oldr2 = rdl2;

      if (++s->kl1 >= s->dl1)
	s->kl1 = 0;
      if (++s->kr1 >= s->dr1)
	s->kr1 = 0;

      if (++s->kl2 >= s->dl2)
	s->kl2 = 0;
      if (++s->kr2 >= s->dr2)
	s->kr2 = 0;

    }

}



void
MusicDelay_setdelay (MDelay_t * s)
{

  LADSPA_Data Ptempo = *(s->Ptempo);  
  LADSPA_Data Plrdelay = *(s->Plrdelay);
  LADSPA_Data Pdelay1 = *(s->Pdelay1);
  LADSPA_Data Pdelay2 = *(s->Pdelay2);

  float ntem = 60.0 / (float) Ptempo;
  float coef;


  s->delay1 = (int) ((ntem / Pdelay1) * s->SAMPLE_RATE);

  if (Plrdelay != 0)
    coef = ntem / Plrdelay;
  else
    coef = 0;
  s->delay2 = (int) ((coef + (ntem / Pdelay2)) * s->SAMPLE_RATE);


  MusicDelay_initdelays (s);

}



void
MusicDelay_settempo (MDelay_t *s, int tempo)
{

  LADSPA_Data Plrdelay = *(s->Plrdelay);
  LADSPA_Data Pdelay1 = *(s->Pdelay1);
  LADSPA_Data Pdelay2 = *(s->Pdelay2);

  float coef = 0.0;

  float ntem = 60.0 / (float) tempo;


  s->delay1 = (int) ((ntem / Pdelay1) * s->SAMPLE_RATE);
  if (Plrdelay != 0)
    coef = ntem / Plrdelay;
  else
    coef = 0;
  s->delay2 = (int) ((coef + (ntem / Pdelay2)) * s->SAMPLE_RATE);

  MusicDelay_initdelays (s);



}




