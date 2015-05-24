/*
  ZynAddSubFX - a software synthesizer
 
  EQ.C based - EQ effect
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
#include "eq10.h"


void
EQ_Init(ZEq10ban_t * s)
{

    // Crear los filtros con las frecuencias;
    AnalogFilter_Init(s,&s->b1_l,6, 31.0, 1, 0);
    AnalogFilter_Init(s,&s->b1_r,6, 31.0, 1, 0);
    AnalogFilter_Init(s,&s->b2_l,6, 63.0, 1, 0);
    AnalogFilter_Init(s,&s->b2_r,6, 63.0, 1, 0);
    AnalogFilter_Init(s,&s->b3_l,6, 125.0, 1, 0);
    AnalogFilter_Init(s,&s->b3_r,6, 125.0, 1, 0);
    AnalogFilter_Init(s,&s->b4_l,6, 250.0, 1, 0);
    AnalogFilter_Init(s,&s->b4_r,6, 250.0, 1, 0);
    AnalogFilter_Init(s,&s->b5_l,6, 500.0, 1, 0);
    AnalogFilter_Init(s,&s->b5_r,6, 500.0, 1, 0);
    AnalogFilter_Init(s,&s->b6_l,6, 1000.0, 1, 0);
    AnalogFilter_Init(s,&s->b6_r,6, 1000.0, 1, 0);
    AnalogFilter_Init(s,&s->b7_l,6, 2000.0, 1, 0);
    AnalogFilter_Init(s,&s->b7_r,6, 2000.0, 1, 0);
    AnalogFilter_Init(s,&s->b8_l,6, 4000.0, 1, 0);
    AnalogFilter_Init(s,&s->b8_r,6, 4000.0, 1, 0);
    AnalogFilter_Init(s,&s->b9_l,6, 8000.0, 1, 0);
    AnalogFilter_Init(s,&s->b9_r,6, 8000.0, 1, 0);
    AnalogFilter_Init(s,&s->b10_l,6, 16000.0, 1, 0);
    AnalogFilter_Init(s,&s->b10_r,6, 16000.0, 1, 0);
   
  //default values

 EQ_Cleanup(s);

}


/*
 * Cleanup the effect
 */
void
EQ_Cleanup(ZEq10ban_t * s)
{
     AnalogFilter_Cleanup(s,&s->b1_l);
     AnalogFilter_Cleanup(s,&s->b1_r);
     AnalogFilter_Cleanup(s,&s->b2_l);
     AnalogFilter_Cleanup(s,&s->b2_r);
     AnalogFilter_Cleanup(s,&s->b3_l);
     AnalogFilter_Cleanup(s,&s->b3_r);
     AnalogFilter_Cleanup(s,&s->b4_l);
     AnalogFilter_Cleanup(s,&s->b4_r);
     AnalogFilter_Cleanup(s,&s->b5_l);
     AnalogFilter_Cleanup(s,&s->b5_r);
     AnalogFilter_Cleanup(s,&s->b6_l);
     AnalogFilter_Cleanup(s,&s->b6_r);
     AnalogFilter_Cleanup(s,&s->b7_l);
     AnalogFilter_Cleanup(s,&s->b7_r);
     AnalogFilter_Cleanup(s,&s->b8_l);
     AnalogFilter_Cleanup(s,&s->b8_r);
     AnalogFilter_Cleanup(s,&s->b9_l);
     AnalogFilter_Cleanup(s,&s->b9_r);
     AnalogFilter_Cleanup(s,&s->b10_l);
     AnalogFilter_Cleanup(s,&s->b10_r);
     
}


/*
 * Effect output
 */
void
out (ZEq10ban_t * s, float * smpsl, float * smpsr, unsigned long count)
{
  int i;
  float tmp;
  

  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr;
  LADSPA_Data vol = *(s->outvolume);
  LADSPA_Data q = *(s->q);
  LADSPA_Data gain1 = *(s->gain1);
  LADSPA_Data gain2 = *(s->gain2);
  LADSPA_Data gain3 = *(s->gain3);
  LADSPA_Data gain4 = *(s->gain4);
  LADSPA_Data gain5 = *(s->gain5);
  LADSPA_Data gain6 = *(s->gain6);
  LADSPA_Data gain7 = *(s->gain7);
  LADSPA_Data gain8 = *(s->gain8);
  LADSPA_Data gain9 = *(s->gain9);
  LADSPA_Data gain10 = *(s->gain10);

  if (s->Rq != q)
    {
      s->Rq = q;
      tmp = powf (30.0, s->Rq);
      setq (s,&s->b1_l,tmp);
      setq (s,&s->b1_r,tmp);
      setq (s,&s->b2_l,tmp);
      setq (s,&s->b2_r,tmp);
      setq (s,&s->b3_l,tmp);
      setq (s,&s->b3_r,tmp);
      setq (s,&s->b4_l,tmp);
      setq (s,&s->b4_r,tmp);
      setq (s,&s->b5_l,tmp);
      setq (s,&s->b5_r,tmp);
      setq (s,&s->b6_l,tmp);
      setq (s,&s->b6_r,tmp);
      setq (s,&s->b7_l,tmp);
      setq (s,&s->b7_r,tmp);
      setq (s,&s->b8_l,tmp);
      setq (s,&s->b8_r,tmp);
      setq (s,&s->b9_l,tmp);
      setq (s,&s->b9_r,tmp);
      setq (s,&s->b10_l,tmp);
      setq (s,&s->b10_r,tmp);
    }
   
  if (s->Rgain1 != gain1)
    {
      s->Rgain1 = gain1;
      setgain (s,&s->b1_l,s->Rgain1*30.0);
      setgain (s,&s->b1_r,s->Rgain1*30.0);
    }
   
  if (s->Rgain2 != gain2)
    {
      s->Rgain2 = gain2;
      setgain (s,&s->b2_l,s->Rgain2*30.0);
      setgain (s,&s->b2_r,s->Rgain2*30.0);
    }
   
  if (s->Rgain3 != gain3)
    {
      s->Rgain3 = gain3;
      setgain (s,&s->b3_l,s->Rgain3*30.0);
      setgain (s,&s->b3_r,s->Rgain3*39.0);
    }
   
  if (s->Rgain4 != gain4)
    {
      s->Rgain4 = gain4;
      setgain (s,&s->b4_l,s->Rgain4*15.0);
      setgain (s,&s->b4_r,s->Rgain4*15.0);
    }
   
  if (s->Rgain5 != gain5)
    {
      s->Rgain5 = gain5;
      setgain (s,&s->b5_l,s->Rgain5*15.0);
      setgain (s,&s->b5_r,s->Rgain5*15.0);
    }
   
  if (s->Rgain6 != gain6)
    {
      s->Rgain6 = gain6;
      setgain (s,&s->b6_l,s->Rgain6*15.0);
      setgain (s,&s->b6_r,s->Rgain6*15.0);
    }
   
  if (s->Rgain7 != gain7)
    {
      s->Rgain7 = gain7;
      setgain (s,&s->b7_l,s->Rgain7*15.0);
      setgain (s,&s->b7_r,s->Rgain7*15.0);
    }
   
  if (s->Rgain8 != gain8)
    {
      s->Rgain8 = gain8;
      setgain (s,&s->b8_l,s->Rgain8*15.0);
      setgain (s,&s->b8_r,s->Rgain8*15.0);
    }
   
  if (s->Rgain9 != gain9)
    {
      s->Rgain9 = gain9;
      setgain (s,&s->b9_l,s->Rgain9*15.0);
      setgain (s,&s->b9_r,s->Rgain9*15.0);
    }
   
  if (s->Rgain10 != gain10)
    {
      s->Rgain10 = gain10;
      setgain (s,&s->b10_l,s->Rgain10*15.0);
      setgain (s,&s->b10_r,s->Rgain10*15.0);
    }

    vol = powf (0.005, (1.0 - vol)) * 10.0;      
  
  for (i = 0; i < count; i++)
    {
      outputl[i] = smpsl[i] * vol;
      outputr[i] = smpsr[i] * vol;
    }

      filterout (s,&s->b1_l,outputl,count);
      filterout (s,&s->b1_r,outputr,count);
      filterout (s,&s->b2_l,outputl,count);
      filterout (s,&s->b2_r,outputr,count);
      filterout (s,&s->b3_l,outputl,count);
      filterout (s,&s->b3_r,outputr,count);
      filterout (s,&s->b4_l,outputl,count);
      filterout (s,&s->b4_r,outputr,count);
      filterout (s,&s->b5_l,outputl,count);
      filterout (s,&s->b5_r,outputr,count);
      filterout (s,&s->b6_l,outputl,count);
      filterout (s,&s->b6_r,outputr,count);
      filterout (s,&s->b7_l,outputl,count);
      filterout (s,&s->b7_r,outputr,count);
      filterout (s,&s->b8_l,outputl,count);
      filterout (s,&s->b8_r,outputr,count);
      filterout (s,&s->b9_l,outputl,count);
      filterout (s,&s->b9_r,outputr,count);
      filterout (s,&s->b10_l,outputl,count);
      filterout (s,&s->b10_r,outputr,count);
    
}




