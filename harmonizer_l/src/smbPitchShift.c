/****************************************************************************
*
* NAME: smbPitchShift.cpp
* VERSION: 1.2
* HOME URL: http://www.dspdimension.com
* KNOWN BUGS: none
*
* SYNOPSIS: Routine for doing pitch shifting while maintaining
* duration using the Short Time Fourier Transform.
*
* DESCRIPTION: The routine takes a pitchShift factor value which is between 0.5
* (one octave down) and 2. (one octave up). A value of exactly 1 does not change
* the pitch. numSampsToProcess tells the routine how many samples in indata[0...
* numSampsToProcess-1] should be pitch shifted and moved to outdata[0 ...
* numSampsToProcess-1]. The two buffers can be identical (ie. it can process the
* data in-place). fftFrameSize defines the FFT frame size used for the
* processing. Typical values are 1024, 2048 and 4096. It may be any value <=
* MAX_FRAME_LENGTH but it MUST be a power of 2. osamp is the STFT
* oversampling factor which also determines the overlap between adjacent STFT
* frames. It should at least be 4 for moderate scaling ratios. A value of 32 is
* recommended for best quality. sampleRate takes the sample rate for the signal 
* in unit Hz, ie. 44100 for 44.1 kHz audio. The data passed to the routine in 
* indata[] should be in the range [-1.0, 1.0), which is also the output range 
* for the data, make sure you scale the data accordingly (for 16bit signed integers
* you would have to divide (and multiply) by 32768). 
*
* COPYRIGHT 1999-2006 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
*
* 						The Wide Open License (WOL)
*
* Permission to use, copy, modify, distribute and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice and this license appear in all source copies. 
* THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
* ANY KIND. See http://www.dspguru.com/wol.htm for more information.
*
*****************************************************************************/

#include <string.h>
#include <math.h>
#include "harmonizer.h"

// -----------------------------------------------------------------------------------------------------------------

//void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata)
/*
	Routine smbPitchShift(). See top of file for explanation
	Purpose: doing pitch shifting while maintaining duration using the Short
	Time Fourier Transform.
	Author: (c)1999-2006 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
*/
void
PitchShifter_init (holharm_t * s, long fftFrameSize, long osamp,
		   float sampleRate)
{


  /* set up some handy variables */
  s->PSL.fftFrameSize2 = fftFrameSize / 2;
  s->PSL.stepSize = fftFrameSize / osamp;
  s->PSL.freqPerBin = sampleRate / (double) fftFrameSize;
  s->PSL.expct = 2. * M_PI * (double) s->PSL.stepSize / (double) fftFrameSize;
  s->PSL.inFifoLatency = fftFrameSize - s->PSL.stepSize;
  s->PSL.gRover = s->PSL.inFifoLatency;
  s->PSL.ratio = 1.0;

  /* initialize our static arrays */
  memset (s->PSL.gInFIFO, 0, MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSL.gOutFIFO, 0, MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSL.gFFTworksp, 0, 2 * MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSL.gLastPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof (float));
  memset (s->PSL.gSumPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof (float));
  memset (s->PSL.gOutputAccum, 0, 2 * MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSL.gAnaFreq, 0, MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSL.gAnaMagn, 0, MAX_FRAME_LENGTH * sizeof (float));



  /* set up some handy variables */
  s->PSR.fftFrameSize2 = fftFrameSize / 2;
  s->PSR.stepSize = fftFrameSize / osamp;
  s->PSR.freqPerBin = sampleRate / (double) fftFrameSize;
  s->PSR.expct = 2. * M_PI * (double) s->PSR.stepSize / (double) fftFrameSize;
  s->PSR.inFifoLatency = fftFrameSize - s->PSR.stepSize;
  s->PSR.gRover = s->PSR.inFifoLatency;
  s->PSR.ratio = 1.0;

  /* initialize our static arrays */
  memset (s->PSR.gInFIFO, 0, MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSR.gOutFIFO, 0, MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSR.gFFTworksp, 0, 2 * MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSR.gLastPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof (float));
  memset (s->PSR.gSumPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof (float));
  memset (s->PSR.gOutputAccum, 0, 2 * MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSR.gAnaFreq, 0, MAX_FRAME_LENGTH * sizeof (float));
  memset (s->PSR.gAnaMagn, 0, MAX_FRAME_LENGTH * sizeof (float));

}

void
smbPitchShiftL (holharm_t * s, float pitchShift, long numSampsToProcess,
		long fftFrameSize, long osamp, float sampleRate,
		float *indata, float *outdata)
{
  int i;

  /* main processing loop */
  for (i = 0; i < numSampsToProcess; i++)
    {

      /* As long as we have not yet collected enough data just read in */
      s->PSL.gInFIFO[s->PSL.gRover] = indata[i];
      outdata[i] = s->PSL.gOutFIFO[s->PSL.gRover - s->PSL.inFifoLatency];
      s->PSL.gRover++;

      /* now we have enough data for processing */
      if (s->PSL.gRover >= fftFrameSize)
	{
	  s->PSL.gRover = s->PSL.inFifoLatency;

	  /* do windowing and re,im interleave */
	  for (s->PSL.k = 0; s->PSL.k < fftFrameSize; s->PSL.k++)
	    {
	      s->PSL.window =
		-.5 * cos (2. * M_PI * (double) s->PSL.k /
			   (double) fftFrameSize) + .5;
	      s->PSL.gFFTworksp[2 * s->PSL.k] =
		s->PSL.gInFIFO[s->PSL.k] * s->PSL.window;
	      s->PSL.gFFTworksp[2 * s->PSL.k + 1] = 0.;
	    }
	  /* ***************** ANALYSIS ******************* */
	  /* do transform */
	  smbFft (s, s->PSL.gFFTworksp, fftFrameSize, -1);

	  /* this is the analysis step */
	  for (s->PSL.k = 0; s->PSL.k <= s->PSL.fftFrameSize2; s->PSL.k++)
	    {

	      /* de-interlace FFT buffer */
	      s->PSL.real = s->PSL.gFFTworksp[2 * s->PSL.k];
	      s->PSL.imag = s->PSL.gFFTworksp[2 * s->PSL.k + 1];

	      /* compute magnitude and phase */
	      s->PSL.magn =
		2. * sqrt (s->PSL.real * s->PSL.real +
			   s->PSL.imag * s->PSL.imag);
	      s->PSL.phase = atan2 (s->PSL.imag, s->PSL.real);

	      /* compute phase difference */
	      s->PSL.tmp = s->PSL.phase - s->PSL.gLastPhase[s->PSL.k];
	      s->PSL.gLastPhase[s->PSL.k] = s->PSL.phase;

	      /* subtract expected phase difference */
	      s->PSL.tmp -= (double) s->PSL.k * s->PSL.expct;

	      /* map delta phase into +/- Pi interval */
	      s->PSL.qpd = (long) (s->PSL.tmp / M_PI);
	      if (s->PSL.qpd >= 0)
		s->PSL.qpd += s->PSL.qpd & 1;

	      else
		s->PSL.qpd -= s->PSL.qpd & 1;
	      s->PSL.tmp -= M_PI * (double) s->PSL.qpd;

	      /* get deviation from bin frequency from the +/- Pi interval */
	      s->PSL.tmp = osamp * s->PSL.tmp / (2. * M_PI);

	      /* compute the k-th partials' true frequency */
	      s->PSL.tmp =
		(double) s->PSL.k * s->PSL.freqPerBin +
		s->PSL.tmp * s->PSL.freqPerBin;

	      /* store magnitude and true frequency in analysis arrays */
	      s->PSL.gAnaMagn[s->PSL.k] = s->PSL.magn;
	      s->PSL.gAnaFreq[s->PSL.k] = s->PSL.tmp;
	    }
	  /* ***************** PROCESSING ******************* */
	  /* this does the actual pitch shifting */
	  memset (s->PSL.gSynMagn, 0, fftFrameSize * sizeof (float));
	  memset (s->PSL.gSynFreq, 0, fftFrameSize * sizeof (float));
	  for (s->PSL.k = 0; s->PSL.k <= s->PSL.fftFrameSize2; s->PSL.k++)
	    {
	      s->PSL.index = (long) (s->PSL.k * pitchShift);
	      if (s->PSL.index <= s->PSL.fftFrameSize2)
		{
		  s->PSL.gSynMagn[s->PSL.index] += s->PSL.gAnaMagn[s->PSL.k];
		  s->PSL.gSynFreq[s->PSL.index] =
		    s->PSL.gAnaFreq[s->PSL.k] * pitchShift;
		}
	    }

	  /* ***************** SYNTHESIS ******************* */
	  /* this is the synthesis step */
	  for (s->PSL.k = 0; s->PSL.k <= s->PSL.fftFrameSize2; s->PSL.k++)
	    {

	      /* get magnitude and true frequency from synthesis arrays */
	      s->PSL.magn = s->PSL.gSynMagn[s->PSL.k];
	      s->PSL.tmp = s->PSL.gSynFreq[s->PSL.k];

	      /* subtract bin mid frequency */
	      s->PSL.tmp -= (double) s->PSL.k * s->PSL.freqPerBin;

	      /* get bin deviation from freq deviation */
	      s->PSL.tmp /= s->PSL.freqPerBin;

	      /* take osamp into account */
	      s->PSL.tmp = 2. * M_PI * s->PSL.tmp / osamp;

	      /* add the overlap phase advance back in */
	      s->PSL.tmp += (double) s->PSL.k * s->PSL.expct;

	      /* accumulate delta phase to get bin phase */
	      s->PSL.gSumPhase[s->PSL.k] += s->PSL.tmp;
	      s->PSL.phase = s->PSL.gSumPhase[s->PSL.k];

	      /* get real and imag part and re-interleave */
	      s->PSL.gFFTworksp[2 * s->PSL.k] =
		s->PSL.magn * cos (s->PSL.phase);
	      s->PSL.gFFTworksp[2 * s->PSL.k + 1] =
		s->PSL.magn * sin (s->PSL.phase);
	    }
	  /* zero negative frequencies */
	  for (s->PSL.k = fftFrameSize + 2; s->PSL.k < 2 * fftFrameSize;
	       s->PSL.k++)
	    s->PSL.gFFTworksp[s->PSL.k] = 0.;

	  /* do inverse transform */
	  smbFft (s, s->PSL.gFFTworksp, fftFrameSize, 1);

	  /* do windowing and add to output accumulator */
	  for (s->PSL.k = 0; s->PSL.k < fftFrameSize; s->PSL.k++)
	    {
	      s->PSL.window =
		-.5 * cos (2. * M_PI * (double) s->PSL.k /
			   (double) fftFrameSize) + .5;
	      s->PSL.gOutputAccum[s->PSL.k] +=
		2. * s->PSL.window * s->PSL.gFFTworksp[2 * s->PSL.k] /
		(s->PSL.fftFrameSize2 * osamp);
	  } for (s->PSL.k = 0; s->PSL.k < s->PSL.stepSize; s->PSL.k++)
	    s->PSL.gOutFIFO[s->PSL.k] = s->PSL.gOutputAccum[s->PSL.k];

	  /* shift accumulator */
	  memmove (s->PSL.gOutputAccum, s->PSL.gOutputAccum + s->PSL.stepSize,
		   fftFrameSize * sizeof (float));

	  /* move input FIFO */
	  for (s->PSL.k = 0; s->PSL.k < s->PSL.inFifoLatency; s->PSL.k++)
	    s->PSL.gInFIFO[s->PSL.k] =
	      s->PSL.gInFIFO[s->PSL.k + s->PSL.stepSize];
	}
    }
}


void
smbPitchShiftR (holharm_t * s, float pitchShift, long numSampsToProcess,
		long fftFrameSize, long osamp, float sampleRate,
		float *indata, float *outdata)
{
  int i;

  /* main processing loop */
  for (i = 0; i < numSampsToProcess; i++)
    {

      /* As long as we have not yet collected enough data just read in */
      s->PSR.gInFIFO[s->PSR.gRover] = indata[i];
      outdata[i] = s->PSR.gOutFIFO[s->PSR.gRover - s->PSR.inFifoLatency];
      s->PSR.gRover++;

      /* now we have enough data for processing */
      if (s->PSR.gRover >= fftFrameSize)
	{
	  s->PSR.gRover = s->PSR.inFifoLatency;

	  /* do windowing and re,im interleave */
	  for (s->PSR.k = 0; s->PSR.k < fftFrameSize; s->PSR.k++)
	    {
	      s->PSR.window =
		-.5 * cos (2. * M_PI * (double) s->PSR.k /
			   (double) fftFrameSize) + .5;
	      s->PSR.gFFTworksp[2 * s->PSR.k] =
		s->PSR.gInFIFO[s->PSR.k] * s->PSR.window;
	      s->PSR.gFFTworksp[2 * s->PSR.k + 1] = 0.;
	    }
	  /* ***************** ANALYSIS ******************* */
	  /* do transform */
	  smbFft (s, s->PSR.gFFTworksp, fftFrameSize, -1);

	  /* this is the analysis step */
	  for (s->PSR.k = 0; s->PSR.k <= s->PSR.fftFrameSize2; s->PSR.k++)
	    {

	      /* de-interlace FFT buffer */
	      s->PSR.real = s->PSR.gFFTworksp[2 * s->PSR.k];
	      s->PSR.imag = s->PSR.gFFTworksp[2 * s->PSR.k + 1];

	      /* compute magnitude and phase */
	      s->PSR.magn =
		2. * sqrt (s->PSR.real * s->PSR.real +
			   s->PSR.imag * s->PSR.imag);
	      s->PSR.phase = atan2 (s->PSR.imag, s->PSR.real);

	      /* compute phase difference */
	      s->PSR.tmp = s->PSR.phase - s->PSR.gLastPhase[s->PSR.k];
	      s->PSR.gLastPhase[s->PSR.k] = s->PSR.phase;

	      /* subtract expected phase difference */
	      s->PSR.tmp -= (double) s->PSR.k * s->PSR.expct;

	      /* map delta phase into +/- Pi interval */
	      s->PSR.qpd = (long) (s->PSR.tmp / M_PI);
	      if (s->PSR.qpd >= 0)
		s->PSR.qpd += s->PSR.qpd & 1;

	      else
		s->PSR.qpd -= s->PSR.qpd & 1;
	      s->PSR.tmp -= M_PI * (double) s->PSR.qpd;

	      /* get deviation from bin frequency from the +/- Pi interval */
	      s->PSR.tmp = osamp * s->PSR.tmp / (2. * M_PI);

	      /* compute the k-th partials' true frequency */
	      s->PSR.tmp =
		(double) s->PSR.k * s->PSR.freqPerBin +
		s->PSR.tmp * s->PSR.freqPerBin;

	      /* store magnitude and true frequency in analysis arrays */
	      s->PSR.gAnaMagn[s->PSR.k] = s->PSR.magn;
	      s->PSR.gAnaFreq[s->PSR.k] = s->PSR.tmp;
	    }
	  /* ***************** PROCESSING ******************* */
	  /* this does the actual pitch shifting */
	  memset (s->PSR.gSynMagn, 0, fftFrameSize * sizeof (float));
	  memset (s->PSR.gSynFreq, 0, fftFrameSize * sizeof (float));
	  for (s->PSR.k = 0; s->PSR.k <= s->PSR.fftFrameSize2; s->PSR.k++)
	    {
	      s->PSR.index = (long) (s->PSR.k * pitchShift);
	      if (s->PSR.index <= s->PSR.fftFrameSize2)
		{
		  s->PSR.gSynMagn[s->PSR.index] += s->PSR.gAnaMagn[s->PSR.k];
		  s->PSR.gSynFreq[s->PSR.index] =
		    s->PSR.gAnaFreq[s->PSR.k] * pitchShift;
		}
	    }

	  /* ***************** SYNTHESIS ******************* */
	  /* this is the synthesis step */
	  for (s->PSR.k = 0; s->PSR.k <= s->PSR.fftFrameSize2; s->PSR.k++)
	    {

	      /* get magnitude and true frequency from synthesis arrays */
	      s->PSR.magn = s->PSR.gSynMagn[s->PSR.k];
	      s->PSR.tmp = s->PSR.gSynFreq[s->PSR.k];

	      /* subtract bin mid frequency */
	      s->PSR.tmp -= (double) s->PSR.k * s->PSR.freqPerBin;

	      /* get bin deviation from freq deviation */
	      s->PSR.tmp /= s->PSR.freqPerBin;

	      /* take osamp into account */
	      s->PSR.tmp = 2. * M_PI * s->PSR.tmp / osamp;

	      /* add the overlap phase advance back in */
	      s->PSR.tmp += (double) s->PSR.k * s->PSR.expct;

	      /* accumulate delta phase to get bin phase */
	      s->PSR.gSumPhase[s->PSR.k] += s->PSR.tmp;
	      s->PSR.phase = s->PSR.gSumPhase[s->PSR.k];

	      /* get real and imag part and re-interleave */
	      s->PSR.gFFTworksp[2 * s->PSR.k] =
		s->PSR.magn * cos (s->PSR.phase);
	      s->PSR.gFFTworksp[2 * s->PSR.k + 1] =
		s->PSR.magn * sin (s->PSR.phase);
	    }
	  /* zero negative frequencies */
	  for (s->PSR.k = fftFrameSize + 2; s->PSR.k < 2 * fftFrameSize;
	       s->PSR.k++)
	    s->PSR.gFFTworksp[s->PSR.k] = 0.;

	  /* do inverse transform */
	  smbFft (s, s->PSR.gFFTworksp, fftFrameSize, 1);

	  /* do windowing and add to output accumulator */
	  for (s->PSR.k = 0; s->PSR.k < fftFrameSize; s->PSR.k++)
	    {
	      s->PSR.window =
		-.5 * cos (2. * M_PI * (double) s->PSR.k /
			   (double) fftFrameSize) + .5;
	      s->PSR.gOutputAccum[s->PSR.k] +=
		2. * s->PSR.window * s->PSR.gFFTworksp[2 * s->PSR.k] /
		(s->PSR.fftFrameSize2 * osamp);
	  } for (s->PSR.k = 0; s->PSR.k < s->PSR.stepSize; s->PSR.k++)
	    s->PSR.gOutFIFO[s->PSR.k] = s->PSR.gOutputAccum[s->PSR.k];

	  /* shift accumulator */
	  memmove (s->PSR.gOutputAccum, s->PSR.gOutputAccum + s->PSR.stepSize,
		   fftFrameSize * sizeof (float));

	  /* move input FIFO */
	  for (s->PSR.k = 0; s->PSR.k < s->PSR.inFifoLatency; s->PSR.k++)
	    s->PSR.gInFIFO[s->PSR.k] =
	      s->PSR.gInFIFO[s->PSR.k + s->PSR.stepSize];
	}
    }
}





// -----------------------------------------------------------------------------------------------------------------
void
smbFft (holharm_t * s, float *fftBuffer, long fftFrameSize, long sign)
/* 
	FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
	Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	time domain data in fftBuffer[0...2*fftFrameSize-1]. The FFT array takes
	and returns the cosine and sine parts in an interleaved manner, ie.
	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. fftFrameSize
	must be a power of 2. It expects a complex input signal (see footnote 2),
	ie. when working with 'common' audio signals our input signal has to be
	passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. In that case, the transform
	of the frequencies of interest is in fftBuffer[0...fftFrameSize].
*/
{
  float wr, wi, arg, *p1, *p2, temp;
  float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
  long i, bitm, j, le, le2, k;
  for (i = 2; i < 2 * fftFrameSize - 2; i += 2)
    {
      for (bitm = 2, j = 0; bitm < 2 * fftFrameSize; bitm <<= 1)
	{
	  if (i & bitm)
	    j++;
	  j <<= 1;
	}
      if (i < j)
	{
	  p1 = fftBuffer + i;
	  p2 = fftBuffer + j;
	  temp = *p1;
	  *(p1++) = *p2;
	  *(p2++) = temp;
	  temp = *p1;
	  *p1 = *p2;
	  *p2 = temp;
	}
    }
  for (k = 0, le = 2; k < (long) (log (fftFrameSize) / log (2.) + .5); k++)
    {
      le <<= 1;
      le2 = le >> 1;
      ur = 1.0;
      ui = 0.0;
      arg = M_PI / (le2 >> 1);
      wr = cos (arg);
      wi = sign * sin (arg);
      for (j = 0; j < le2; j += 2)
	{
	  p1r = fftBuffer + j;
	  p1i = p1r + 1;
	  p2r = p1r + le2;
	  p2i = p2r + 1;
	  for (i = j; i < 2 * fftFrameSize; i += le)
	    {
	      tr = *p2r * ur - *p2i * ui;
	      ti = *p2r * ui + *p2i * ur;
	      *p2r = *p1r - tr;
	      *p2i = *p1i - ti;
	      *p1r += tr;
	      *p1i += ti;
	      p1r += le;
	      p1i += le;
	      p2r += le;
	      p2i += le;
	    }
	  tr = ur * wr - ui * wi;
	  ui = ur * wi + ui * wr;
	  ur = tr;
	}
    }
}


// -----------------------------------------------------------------------------------------------------------------

/*

    12/12/02, smb
    
    PLEASE NOTE:
    
    There have been some reports on domain errors when the atan2() function was used
    as in the above code. Usually, a domain error should not interrupt the program flow
    (maybe except in Debug mode) but rather be handled "silently" and a global variable
    should be set according to this error. However, on some occasions people ran into
    this kind of scenario, so a replacement atan2() function is provided here.
    
    If you are experiencing domain errors and your program stops, simply replace all
    instances of atan2() with calls to the smbAtan2() function below.
    
*/
double
smbAtan2 (holharm_t * s, double x, double y)
{
  double signx;
  if (x > 0.)
    signx = 1.;

  else
    signx = -1.;
  if (x == 0.)
    return 0.;
  if (y == 0.)
    return signx * M_PI / 2.;
  return atan2 (x, y);
}


// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
