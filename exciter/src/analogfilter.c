/*
  ZynAddSubFX - a software synthesizer
 
  AnalogFilter.C - Several analog filters (lowpass, highpass...)
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for zynreverb by Josep Andreu

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
#include "exciter.h"

#define dB2rap(dB) ((expf((dB)*LOG_10/20.0)))
#define LOG_10 2.302585093



void
AnalogFilter_Init (exciter_t * s, AnalogFilter * filter, unsigned char Ftype,
		   float Ffreq, float Fq, unsigned char Fstages)
{


  int i;

  filter->stages = Fstages;
  for (i = 0; i < 3; i++)
    {
      filter->oldc[i] = 0.0;
      filter->oldd[i] = 0.0;
      filter->c[i] = 0.0;
      filter->d[i] = 0.0;
    };
  filter->type = Ftype;
  filter->freq = Ffreq;
  filter->q = Fq;
  filter->gain = 1.0;
  if (filter->stages >= MAX_FILTER_STAGES)
    filter->stages = MAX_FILTER_STAGES;
  AnalogFilter_Cleanup (s, filter);
  filter->firsttime = 0;
  filter->abovenq = 0;
  filter->oldabovenq = 0;
  setfreq_and_q (s, filter, Ffreq, Fq);
  filter->firsttime = 1;
  filter->d[0] = 0;		//this is not used
  filter->outgain = 1.0;


};


void
AnalogFilter_Cleanup (exciter_t * s, AnalogFilter * filter)
{
  int i;
  for (i = 0; i < MAX_FILTER_STAGES + 1; i++)
    {
      filter->x[i].c1 = 0.0;
      filter->x[i].c2 = 0.0;
      filter->y[i].c1 = 0.0;
      filter->y[i].c2 = 0.0;
      filter->oldx[i] = filter->x[i];
      filter->oldy[i] = filter->y[i];
    };
  filter->needsinterpolation = 0;


};

void
computefiltercoefs (exciter_t * s, AnalogFilter * filter)
{
  float tmp;
  float omega, sn, cs, alpha, beta;
  int zerocoefs = 0;		//this is used if the freq is too high

  //do not allow frequencies bigger than samplerate/2
  float freq = filter->freq;
  if (freq > (s->SAMPLE_RATE / 2 - 500.0))
    {
      freq = s->SAMPLE_RATE / 2 - 500.0;
      zerocoefs = 1;
    };
  if (freq < 0.1)
    freq = 0.1;
  //do not allow bogus Q
  if (filter->q < 0.0)
    filter->q = 0.0;
  float tmpq, tmpgain;
  if (filter->stages == 0)
    {
      tmpq = filter->q;
      tmpgain = filter->gain;
    }
  else
    {
      tmpq =
	(filter->q >
	 1.0 ? powf (filter->q, 1.0 / (filter->stages + 1)) : filter->q);
      tmpgain = powf (filter->gain, 1.0 / (filter->stages + 1));
    };

  //most of theese are implementations of 
  //the "Cookbook formulae for audio EQ" by Robert Bristow-Johnson
  //The original location of the Cookbook is:
  //http://www.harmony-central.com/Computer/Programming/Audio-EQ-Cookbook.txt
  switch (filter->type)
    {
    case 0:			//LPF 1 pole         
      if (zerocoefs == 0)
	tmp = expf (-2.0 * M_PI * freq / s->SAMPLE_RATE);
      else
	tmp = 0.0;
      filter->c[0] = 1.0 - tmp;
      filter->c[1] = 0.0;
      filter->c[2] = 0.0;
      filter->d[1] = tmp;
      filter->d[2] = 0.0;
      filter->order = 1;
      break;
    case 1:			//HPF 1 pole
      if (zerocoefs == 0)
	tmp = expf (-2.0 * M_PI * freq / s->SAMPLE_RATE);
      else
	tmp = 0.0;
      filter->c[0] = (1.0 + tmp) / 2.0;
      filter->c[1] = -(1.0 + tmp) / 2.0;
      filter->c[2] = 0.0;
      filter->d[1] = tmp;
      filter->d[2] = 0.0;
      filter->order = 1;
      break;
    case 2:			//LPF 2 poles 
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  alpha = sn / (2 * tmpq);
	  tmp = 1 + alpha;
	  filter->c[0] = (1.0 - cs) / 2.0 / tmp;
	  filter->c[1] = (1.0 - cs) / tmp;
	  filter->c[2] = (1.0 - cs) / 2.0 / tmp;
	  filter->d[1] = -2 * cs / tmp * (-1);
	  filter->d[2] = (1 - alpha) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = 1.0;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    case 3:			//HPF 2 poles 
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  alpha = sn / (2 * tmpq);
	  tmp = 1 + alpha;
	  filter->c[0] = (1.0 + cs) / 2.0 / tmp;
	  filter->c[1] = -(1.0 + cs) / tmp;
	  filter->c[2] = (1.0 + cs) / 2.0 / tmp;
	  filter->d[1] = -2 * cs / tmp * (-1);
	  filter->d[2] = (1 - alpha) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = 0.0;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    case 4:			//BPF 2 poles 
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  alpha = sn / (2 * tmpq);
	  tmp = 1 + alpha;
	  filter->c[0] = alpha / tmp * sqrtf (tmpq + 1);
	  filter->c[1] = 0;
	  filter->c[2] = -alpha / tmp * sqrtf (tmpq + 1);
	  filter->d[1] = -2 * cs / tmp * (-1);
	  filter->d[2] = (1 - alpha) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = 0.0;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    case 5:			//NOTCH 2 poles 
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  alpha = sn / (2 * sqrtf (tmpq));
	  tmp = 1 + alpha;
	  filter->c[0] = 1 / tmp;
	  filter->c[1] = -2 * cs / tmp;
	  filter->c[2] = 1 / tmp;
	  filter->d[1] = -2 * cs / tmp * (-1);
	  filter->d[2] = (1 - alpha) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = 1.0;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    case 6:			//PEAK (2 poles)
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  tmpq *= 3.0;
	  alpha = sn / (2 * tmpq);
	  tmp = 1 + alpha / tmpgain;
	  filter->c[0] = (1.0 + alpha * tmpgain) / tmp;
	  filter->c[1] = (-2.0 * cs) / tmp;
	  filter->c[2] = (1.0 - alpha * tmpgain) / tmp;
	  filter->d[1] = -2 * cs / tmp * (-1);
	  filter->d[2] = (1 - alpha / tmpgain) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = 1.0;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    case 7:			//Low Shelf - 2 poles
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  tmpq = sqrtf (tmpq);
	  alpha = sn / (2 * tmpq);
	  beta = sqrtf (tmpgain) / tmpq;
	  tmp = (tmpgain + 1.0) + (tmpgain - 1.0) * cs + beta * sn;

	  filter->c[0] =
	    tmpgain * ((tmpgain + 1.0) - (tmpgain - 1.0) * cs +
		       beta * sn) / tmp;
	  filter->c[1] =
	    2.0 * tmpgain * ((tmpgain - 1.0) - (tmpgain + 1.0) * cs) / tmp;
	  filter->c[2] =
	    tmpgain * ((tmpgain + 1.0) - (tmpgain - 1.0) * cs -
		       beta * sn) / tmp;
	  filter->d[1] =
	    -2.0 * ((tmpgain - 1.0) + (tmpgain + 1.0) * cs) / tmp * (-1);
	  filter->d[2] =
	    ((tmpgain + 1.0) + (tmpgain - 1.0) * cs - beta * sn) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = tmpgain;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    case 8:			//High Shelf - 2 poles
      if (zerocoefs == 0)
	{
	  omega = 2 * M_PI * freq / s->SAMPLE_RATE;
	  sn = sinf (omega);
	  cs = cosf (omega);
	  tmpq = sqrtf (tmpq);
	  alpha = sn / (2 * tmpq);
	  beta = sqrtf (tmpgain) / tmpq;
	  tmp = (tmpgain + 1.0) - (tmpgain - 1.0) * cs + beta * sn;

	  filter->c[0] =
	    tmpgain * ((tmpgain + 1.0) + (tmpgain - 1.0) * cs +
		       beta * sn) / tmp;
	  filter->c[1] =
	    -2.0 * tmpgain * ((tmpgain - 1.0) + (tmpgain + 1.0) * cs) / tmp;
	  filter->c[2] =
	    tmpgain * ((tmpgain + 1.0) + (tmpgain - 1.0) * cs -
		       beta * sn) / tmp;
	  filter->d[1] =
	    2.0 * ((tmpgain - 1.0) - (tmpgain + 1.0) * cs) / tmp * (-1);
	  filter->d[2] =
	    ((tmpgain + 1.0) - (tmpgain - 1.0) * cs - beta * sn) / tmp * (-1);
	}
      else
	{
	  filter->c[0] = 1.0;
	  filter->c[1] = 0.0;
	  filter->c[2] = 0.0;
	  filter->d[1] = 0.0;
	  filter->d[2] = 0.0;
	};
      filter->order = 2;
      break;
    default:			//wrong type
      filter->type = 0;
      computefiltercoefs (s, filter);
      break;
    };
};


void
setfreq (exciter_t * s, AnalogFilter * filter, float frequency)
{
  int i;
  if (frequency < 0.1)
    frequency = 0.1;
  float rap = filter->freq / frequency;
  if (rap < 1.0)
    rap = 1.0 / rap;

  filter->oldabovenq = filter->abovenq;
  filter->abovenq = frequency > (s->SAMPLE_RATE / 2 - 500.0);

  int nyquistthresh = (filter->abovenq ^ filter->oldabovenq);


  if ((rap > 3.0) || (nyquistthresh != 0))
    {				//if the frequency is changed fast, it needs interpolation (now, filter and coeficients backup)
      for (i = 0; i < 3; i++)
	{
	  filter->oldc[i] = filter->c[i];
	  filter->oldd[i] = filter->d[i];
	};
      for (i = 0; i < MAX_FILTER_STAGES + 1; i++)
	{
	  filter->oldx[i] = filter->x[i];
	  filter->oldy[i] = filter->y[i];
	};
      if (filter->firsttime == 0)
	filter->needsinterpolation = 1;
    };
  filter->freq = frequency;
  computefiltercoefs (s, filter);
  filter->firsttime = 0;

};

void
setfreq_and_q (exciter_t * s, AnalogFilter * filter, float frequency,
	       float q_)
{
  filter->q = q_;
  setfreq (s, filter, frequency);
};

void
setq (exciter_t * s, AnalogFilter * filter, float q_)
{
  filter->q = q_;
  computefiltercoefs (s, filter);
};

void
settype (exciter_t * s, AnalogFilter * filter, int type_)
{
  filter->type = type_;
  computefiltercoefs (s, filter);
};

void
setgain (exciter_t * s, AnalogFilter * filter, float dBgain)
{
  filter->gain = dB2rap (dBgain);
  computefiltercoefs (s, filter);
};

void
setstages (exciter_t * s, AnalogFilter * filter, int stages_)
{
  if (stages_ >= MAX_FILTER_STAGES)
    stages_ = MAX_FILTER_STAGES - 1;
  filter->stages = stages_;
  AnalogFilter_Cleanup (s, filter);
  computefiltercoefs (s, filter);
};

void
singlefilterout (exciter_t * s, AnalogFilter * filter, float *smp, fstage * x,
		 fstage * y, float *c, float *d, unsigned long count)
{
  int i;
  float y0;
  if (filter->order == 1)
    {				//First order filter
      for (i = 0; i < count; i++)
	{
	  y0 =
	    smp[i] * filter->c[0] + x->c1 * filter->c[1] +
	    y->c1 * filter->d[1];
	  y->c1 = y0;
	  x->c1 = smp[i];
	  //output
	  smp[i] = y0;
	};
    };
  if (filter->order == 2)
    {				//Second order filter
      for (i = 0; i < count; i++)
	{
	  y0 =
	    (smp[i] * filter->c[0]) + (x->c1 * filter->c[1]) +
	    (x->c2 * filter->c[2]) + (y->c1 * filter->d[1]) +
	    (y->c2 * filter->d[2]);
	  y->c2 = y->c1;
	  y->c1 = y0;
	  x->c2 = x->c1;
	  x->c1 = smp[i];
	  //output
	  smp[i] = y0;
	};
    };
};

void
filterout (exciter_t * s, AnalogFilter * filter, float *smp,
	   unsigned long count)
{
  int i;
  if (filter->needsinterpolation != 0)
    {
      for (i = 0; i < count; i++)
	filter->ismp[i] = smp[i];
      for (i = 0; i < filter->stages + 1; i++)
	singlefilterout (s, filter, filter->ismp, &filter->oldx[i],
			 &filter->oldy[i], filter->oldc, filter->oldd, count);
    };

  for (i = 0; i < filter->stages + 1; i++)
    singlefilterout (s, filter, smp, &filter->x[i], &filter->y[i], filter->c,
		     filter->d, count);

  if (filter->needsinterpolation != 0)
    {
      for (i = 0; i < count; i++)
	{
	  float x = i / (float) count;
	  smp[i] = filter->ismp[i] * (1.0 - x) + smp[i] * x;
	};
      filter->needsinterpolation = 0;
    };

  for (i = 0; i < count; i++)
    smp[i] *= filter->outgain;
};

float
H (exciter_t * s, AnalogFilter * filter, float freq)
{
  int n;

  float fr = freq / s->SAMPLE_RATE * M_PI * 2.0;
  float x = filter->c[0], y = 0.0;
  for (n = 1; n < 3; n++)
    {
      x += cosf (n * fr) * filter->c[n];
      y -= sinf (n * fr) * filter->c[n];
    };
  float h = x * x + y * y;
  x = 1.0;
  y = 0.0;
  for (n = 1; n < 3; n++)
    {
      x -= cosf (n * fr) * filter->d[n];
      y += sinf (n * fr) * filter->d[n];
    };
  h = h / (x * x + y * y);
  return (powf (h, (filter->stages + 1.0) / 2.0));
};
