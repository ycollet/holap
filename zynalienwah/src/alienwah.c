/*
  ZynAddSubFX - a software synthesizer
 
  Alienwah.C - "AlienWah" effect
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

#include <math.h>
#include <stdio.h>
#include "alienwah.h"

void
Alienwah_Init(ZAlienwah_t * s)
{

  Alienwah_Cleanup (s);

  s->k=127;
  s->oldclfol.a = 0.0;
  s->oldclfol.b = 0.0;
  s->oldclfor.a = 0.0;
  s->oldclfor.b = 0.0;
};


/*
 * Apply the effect
 */
void
out (ZAlienwah_t * s, float * smpsl, float * smpsr, unsigned long count)
{
  int i;
  float lfol, lfor;
  COMPLEXTYPE clfol, clfor, out, tmp;
  
  LADSPA_Data depth =  *(s->depth);
  LADSPA_Data phase =  *(s->Phase) * M_PI;
  LADSPA_Data fb = *(s->fb);
  LADSPA_Data panning = *(s->panning);
  LADSPA_Data Pdelay =  *(s->delay);
  LADSPA_Data lrcross = *(s->lrcross);
  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr; 
  LADSPA_Data vol = *(s->outvolume);

  if (s->Rdelay != Pdelay)
  {
    s->Rdelay = Pdelay;
    Alienwah_Cleanup(s);    
  } 

  if (s->Rfb != fb)
  {
    s->Rfb = fb;
    fb = sqrt(fabsf(fb));
    if (fb < .4) fb = .4;
    if (s->Rfb < 0 ) fb = -fb; 
  }

  effectlfoout(s,&lfol,&lfor,count);

  lfol *= depth * M_PI * 2.0;
  lfor *= depth * M_PI * 2.0;
  clfol.a = cosf (lfol + phase) * fb;
  clfol.b = sinf (lfol + phase) * fb;
  clfor.a = cosf (lfor + phase) * fb;
  clfor.b = sinf (lfor + phase) * fb;
 
  
  for (i = 0; i < count; i++)
    {
      float x = ((float) i) / (float) count;
      float x1 = 1.0 - x;
      //left  
      tmp.a = clfol.a * x + s->oldclfol.a * x1;
      tmp.b = clfol.b * x + s->oldclfol.b * x1;

      out.a = tmp.a * s->oldl[s->oldk].a - tmp.b * s->oldl[s->oldk].b
	+ (1 - fabsf (fb)) * smpsl[i] * panning;
      out.b = tmp.a * s->oldl[s->oldk].b + tmp.b * s->oldl[s->oldk].a;
      s->oldl[s->oldk].a = out.a;
      s->oldl[s->oldk].b = out.b;
      float l = out.a * 10.0 * (fb + 0.1);

      //right
      tmp.a = clfor.a * x + s->oldclfor.a * x1;
      tmp.b = clfor.b * x + s->oldclfor.b * x1;

      out.a = tmp.a * s->oldr[s->oldk].a - tmp.b * s->oldr[s->oldk].b
	+ (1 - fabsf (fb)) * smpsr[i] * (1.0 - panning);
      out.b = tmp.a * s->oldr[s->oldk].b + tmp.b * s->oldr[s->oldk].a;
      s->oldr[s->oldk].a = out.a;
      s->oldr[s->oldk].b = out.b;
      float r = out.a * 10.0 * (fb + 0.1);


      if (++s->oldk >= Pdelay)
	s->oldk = 0;
      //LRcross
      outputl[i] = l * (1.0 - lrcross) + r * lrcross;
      outputr[i] = r * (1.0 - lrcross) + l * lrcross;
      outputl[i] *= vol;
      outputr[i] *= vol;


    };

  s->oldclfol.a = clfol.a;
  s->oldclfol.b = clfol.b;
  s->oldclfor.a = clfor.a;
  s->oldclfor.b = clfor.b;

};

/*
 * Cleanup the effect
 */
void
Alienwah_Cleanup(ZAlienwah_t * s)
{
  int i;
  
  for (i = 0; i < MAX_ALIENWAH_DELAY; i++)
    {
      s->oldl[i].a = 0.0;
      s->oldl[i].b = 0.0;
      s->oldr[i].a = 0.0;
      s->oldr[i].b = 0.0;
    };

  s->oldk = 0;

};




