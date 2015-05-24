/*
  ZynAddSubFX - a software synthesizer
 
  EQ.C - EQ effect
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
#include "eq.h"

void
EQ_Init(ZEq3par_t * s)
{

    // Crear los filtros con las frecuencias;
    AnalogFilter_Init(s,&s->Low_l,6, 240.0, 1, 0);
    AnalogFilter_Init(s,&s->Low_r,6, 240.0, 1, 0);
    AnalogFilter_Init(s,&s->Mid_l,6, 1350.0, 1, 0);
    AnalogFilter_Init(s,&s->Mid_r,6, 1350.0, 1, 0);
    AnalogFilter_Init(s,&s->High_l,6, 3990.0, 1, 0);
    AnalogFilter_Init(s,&s->High_r,6, 3990.0, 1, 0);
   
  //default values

 EQ_Cleanup(s);

}


/*
 * Cleanup the effect
 */
void
EQ_Cleanup(ZEq3par_t * s)
{
     AnalogFilter_Cleanup(s,&s->Low_l);
     AnalogFilter_Cleanup(s,&s->Low_r);
     AnalogFilter_Cleanup(s,&s->Mid_l);
     AnalogFilter_Cleanup(s,&s->Mid_r);
     AnalogFilter_Cleanup(s,&s->High_l);
     AnalogFilter_Cleanup(s,&s->High_r);
     
}


/*
 * Effect output
 */
void
out (ZEq3par_t * s, float * smpsl, float * smpsr, unsigned long count)
{
  int i;
  float tmp;
  

  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr;
  LADSPA_Data vol = *(s->outvolume);
  LADSPA_Data lowfreq = *(s->lowfreq);
  LADSPA_Data lowgain = *(s->lowgain);
  LADSPA_Data lowq = *(s->lowq); 
  LADSPA_Data midfreq = *(s->midfreq);
  LADSPA_Data midgain = *(s->midgain);
  LADSPA_Data midq = *(s->midq); 
  LADSPA_Data highfreq = *(s->highfreq);
  LADSPA_Data highgain = *(s->highgain);
  LADSPA_Data highq = *(s->highq); 

  if (s->Rlowfreq != lowfreq)
    {
      s->Rlowfreq = lowfreq;
      setfreq(s,&s->Low_l,s->Rlowfreq);
      setfreq(s,&s->Low_r,s->Rlowfreq);
    }
  
  if (s->Rmidfreq != midfreq)
    {
      s->Rmidfreq = midfreq;
      setfreq(s,&s->Mid_l,s->Rmidfreq);
      setfreq(s,&s->Mid_r,s->Rmidfreq);
    }
    
  if (s->Rhighfreq != highfreq)
    {
      s->Rhighfreq = highfreq;
      setfreq(s,&s->High_l,s->Rhighfreq);
      setfreq(s,&s->High_r,s->Rhighfreq);
    }
    
    
  if (s->Rlowgain != lowgain)
    {
      s->Rlowgain = lowgain;
      setgain (s,&s->Low_l,s->Rlowgain*30.0);
      setgain (s,&s->Low_r,s->Rlowgain*30.0);
    }
    
    
  if (s->Rmidgain != midgain)
    {
      s->Rmidgain = midgain;
      setgain (s,&s->Mid_l,s->Rmidgain*30.0);
      setgain (s,&s->Mid_r,s->Rmidgain*30.0);
    }
    
    
  if (s->Rhighgain != highgain)
    {
      s->Rhighgain = highgain;
      setgain (s,&s->High_l,s->Rhighgain*30.0);
      setgain (s,&s->High_r,s->Rhighgain*30.0);
    }
    
    
  if (s->Rlowq != lowq)
    {
      s->Rlowq = lowq;
      tmp = powf (30.0, s->Rlowq);
      setq (s,&s->Low_l,tmp);
      setq (s,&s->Low_r,tmp);
    }
        
    
  if (s->Rmidq != midq)
    {
      s->Rmidq = midq;
      tmp = powf (30.0, s->Rmidq);
      setq (s,&s->Mid_l,tmp);
      setq (s,&s->Mid_r,tmp);
    }
        
    
  if (s->Rhighq != highq)
    {
      s->Rhighq = highq;
      tmp = powf (30.0, s->Rhighq);
      setq (s,&s->High_l,tmp);
      setq (s,&s->High_r,tmp);
    }
        

    vol = powf (0.005, (1.0 - vol)) * 10.0;      
  
  for (i = 0; i < count; i++)
    {
      outputl[i] = smpsl[i] * vol;
      outputr[i] = smpsr[i] * vol;
    }

      filterout (s,&s->Low_l,outputl,count);
      filterout (s,&s->Low_r,outputr,count);
      filterout (s,&s->Mid_l,outputl,count);
      filterout (s,&s->Mid_r,outputr,count);
      filterout (s,&s->High_l,outputl,count);
      filterout (s,&s->High_r,outputr,count);
    


}




