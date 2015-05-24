/*
    holharm - a inteligent harmonizer DSSI plugin software                                          
    
    harmonizer.h  -  Harmonizer header.
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

#ifndef HARMONIZER_H
#define HARMONIZER_H

#include <lo/lo.h>
#include "../config.h"
#include <ladspa.h>
#include <pthread.h>

#define QUALITY 4
#define POLY 8
#define LOG_D_NOTE      0.057762265047
#define D_NOTE_SQRT     1.029302236643
#define LOG_2           0.693147180559
#define M_PI 3.14159265358979323846
#define MAX_PEAKS 8
#define D_NOTE          1.059463094359
#define MAX_FRAME_LENGTH 2048
#define STEP_SIZE 4096
#define LOG_10 2.302585093
#define rap2dB(rap) ((20*log(rap)/LOG_10))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


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
  int reconota;
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
  int Ch[34][13];

} RChord;



// Recognize Note

typedef struct
{
  int note;
  int ultima;
  int blockSize;
  float *recinl;
  float *recinr;
  signed short int *schmittPointer;
  const char **notes;
  float nfreq, afreq, freq;
  float freqs[12];
  float lfreqs[12];
  signed short int schmittBuffer[2048];
  signed short int outi[2048];
} RNote;

// Pitch Shifter


typedef struct
{

  float ratio;
  float gInFIFO[MAX_FRAME_LENGTH];
  float gOutFIFO[MAX_FRAME_LENGTH];
  float gFFTworksp[2 * MAX_FRAME_LENGTH];
  float gLastPhase[MAX_FRAME_LENGTH / 2 + 1];
  float gSumPhase[MAX_FRAME_LENGTH / 2 + 1];
  float gOutputAccum[2 * MAX_FRAME_LENGTH];
  float gAnaFreq[MAX_FRAME_LENGTH];
  float gAnaMagn[MAX_FRAME_LENGTH];
  float gSynFreq[MAX_FRAME_LENGTH];
  float gSynMagn[MAX_FRAME_LENGTH];
  double magn, phase, tmp, window, real, imag;
  double freqPerBin, expct;
  long k, qpd, index, inFifoLatency, stepSize, fftFrameSize2, gRover;
} smbP;

// Harmonizer


typedef struct
{
  pthread_mutex_t mutex;
  lo_server osc_server;
  lo_address m_host;
  char *osc_configure_path;
  int val_sum;
  int last;
  int lcount;
  int candidate;
  int lastcandidate;
  unsigned long SAMPLE_RATE;
  unsigned long hq;
  LADSPA_Data *PSELECT;
  LADSPA_Data *inputl;
  LADSPA_Data *inputr;
  LADSPA_Data *efxoutl;
  LADSPA_Data *efxoutr;
  LADSPA_Data *outvolume;
  LADSPA_Data *wet;
  LADSPA_Data *Ptype;
  LADSPA_Data *panning;
  LADSPA_Data *intervall;
  LADSPA_Data *intervalr;
  LADSPA_Data *Hnote;
  LADSPA_Data *recocount;
  float ratiol;
  float ratior;
  float outil[2048];
  float outol[2048];
  float outir[2048];
  float outor[2048];
  RChord rc;
  RNote rn;
  smbP PSL;
  smbP PSR;
} holharm_t;



// Harmonizer


void Harmonizer_start (holharm_t * s);
void out_smb (holharm_t * s, float *smpsl, float *smpsr,
	      unsigned long sample_count);
void setpreset (holharm_t * s, unsigned char npreset);
void Harmonizer_cleanup (holharm_t * s);
int ReadConfig();

// Recognize Chord

void Recognize_init (holharm_t * s, float *efxoutl_, float *efxoutr_);
void RecChord_init (holharm_t * s);
void RecChord_destroy (holharm_t * s);
void MiraChord (holharm_t * s);
void IniciaChords (holharm_t * s);
void Vamos (holharm_t * s, int canal, int interval);
void cleanup (holharm_t * s);
int plus (holharm_t * s, int nota);

//Recognize Note


void Recognize (holharm_t * s, float *efxoutl_, float *efxoutr_);
void schmittFloat (holharm_t * s, float *indatal, float *indatar,
		   unsigned long count);
void displayFrequency (holharm_t * s, float freq);
void schmittS16LE (holharm_t * s, signed short int *indata,
		   unsigned long count);

// Pitch Shifter



void PitchShifter_init (holharm_t * s, long fftFrameSize, long osamp,
			float sampleRate);
void smbPitchShiftL (holharm_t * s, float pitchShift, long numSampsToProcess,
		     long fftFrameSize, long osamp, float sampleRate,
		     float *indata, float *outdata);

void smbPitchShiftR (holharm_t * s, float pitchShift, long numSampsToProcess,
		     long fftFrameSize, long osamp, float sampleRate,
		     float *indata, float *outdata);


void smbFft (holharm_t * s, float *fftBuffer, long fftFrameSize, long sign);
double smbAtan2 (holharm_t * s, double x, double y);



#endif
