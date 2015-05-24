/*
  goomf - a FM mono syhth plugin software

  goomf.c  -  Main dssi file of the plugin
  Copyright (C) 2009 Josep Andreu (Holborn)
  Author: Josep Andreu

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License
(version2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/


#include <math.h>
#include <dssi.h>
#include <ladspa.h>
#include <pthread.h>
#include "goomf.h"


#define goomf_OUTPUT_L 0
#define goomf_OUTPUT_R 1
#define goomf_VOLUME 2
#define goomf_MASTERTUNE 3
#define goomf_PORTAMENTO 4
#define goomf_LFO_VOLUME 5
#define goomf_LFO_FREQUENCY 6
#define goomf_LFO_DELAY 7
#define goomf_LFO_WAVE 8
#define goomf_WAVE_0 9
#define goomf_WAVE_1 10
#define goomf_WAVE_2 11
#define goomf_WAVE_3 12
#define goomf_WAVE_4 13
#define goomf_WAVE_5 14
#define goomf_H_0 15
#define goomf_H_1 16
#define goomf_H_2 17
#define goomf_H_3 18
#define goomf_H_4 19
#define goomf_H_5 20
#define goomf_HF_0 21
#define goomf_HF_1 22
#define goomf_HF_2 23
#define goomf_HF_3 24
#define goomf_HF_4 25
#define goomf_HF_5 26
#define goomf_Vol_0 27
#define goomf_Vol_1 28
#define goomf_Vol_2 29
#define goomf_Vol_3 30
#define goomf_Vol_4 31
#define goomf_Vol_5 32
#define goomf_Attack_0 33
#define goomf_Attack_1 34
#define goomf_Attack_2 35
#define goomf_Attack_3 36
#define goomf_Attack_4 37
#define goomf_Attack_5 38
#define goomf_Decay_0 39
#define goomf_Decay_1 40
#define goomf_Decay_2 41
#define goomf_Decay_3 42
#define goomf_Decay_4 43
#define goomf_Decay_5 44
#define goomf_Sustain_0 45
#define goomf_Sustain_1 46
#define goomf_Sustain_2 47
#define goomf_Sustain_3 48
#define goomf_Sustain_4 49
#define goomf_Sustain_5 50
#define goomf_Release_0 51
#define goomf_Release_1 52
#define goomf_Release_2 53
#define goomf_Release_3 54
#define goomf_Release_4 55
#define goomf_Release_5 56
#define goomf_pLFO_0 57
#define goomf_pLFO_1 58
#define goomf_pLFO_2 59
#define goomf_pLFO_3 60
#define goomf_pLFO_4 61
#define goomf_pLFO_5 62
#define goomf_FilterType 63
#define goomf_FilterGain 64
#define goomf_FilterCutoff 65
#define goomf_FilterQ 66
#define goomf_FilterLFO 67
#define goomf_FilterADSR 68
#define goomf_FilterStages 69
#define goomf_FilterVelocity 70
#define goomf_Algo 71

static LADSPA_Descriptor *goomfLDescriptor = NULL;
static DSSI_Descriptor *goomfDDescriptor = NULL;

static void rungoomf (LADSPA_Handle instance, unsigned long sample_count,
		      snd_seq_event_t * events, unsigned long EventCount);


static void cleanupgoomf (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return goomfLDescriptor;
    default:
      return NULL;
    }
}

const DSSI_Descriptor *
dssi_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return goomfDDescriptor;
    default:
      return NULL;
    }
}

static void
cleanupgoomf (LADSPA_Handle instance)
{
  goomf_synth_t *synth = (goomf_synth_t *) instance;
  free (synth);
}

static void
connectPortgoomf (LADSPA_Handle instance, unsigned long port,
		  LADSPA_Data * data)
{
  goomf_synth_t *plugin = (goomf_synth_t *) instance;
  switch (port)
    {
    case goomf_OUTPUT_L:
      plugin->output_l = data;
      break;
    case goomf_OUTPUT_R:
      plugin->output_r = data;
      break;
    case goomf_VOLUME:
      plugin->master_volume = data;
      break;
    case goomf_MASTERTUNE:
      plugin->tune = data;
      break;
    case goomf_PORTAMENTO:
      plugin->portamento = data;
      break;
    case goomf_LFO_VOLUME:
      plugin->LFO_Volume = data;
      break;
    case goomf_LFO_FREQUENCY:
      plugin->LFO_Frequency = data;
      break;
    case goomf_LFO_DELAY:
      plugin->LFO_Delay = data;
      break;
    case goomf_LFO_WAVE:
      plugin->LFO_Wave = data;
      break;
    case goomf_WAVE_0:
      plugin->wave[0] = data;
      break;
    case goomf_WAVE_1:
      plugin->wave[1] = data;
      break;
    case goomf_WAVE_2:
      plugin->wave[2] = data;
      break;
    case goomf_WAVE_3:
      plugin->wave[3] = data;
      break;
    case goomf_WAVE_4:
      plugin->wave[4] = data;
      break;
    case goomf_WAVE_5:
      plugin->wave[5] = data;
      break;
    case goomf_H_0:
      plugin->H[0] = data;
      break;
    case goomf_H_1:
      plugin->H[1] = data;
      break;
    case goomf_H_2:
      plugin->H[2] = data;
      break;
    case goomf_H_3:
      plugin->H[3] = data;
      break;
    case goomf_H_4:
      plugin->H[4] = data;
      break;
    case goomf_H_5:
      plugin->H[5] = data;
      break;
    case goomf_HF_0:
      plugin->HF[0] = data;
      break;
    case goomf_HF_1:
      plugin->HF[1] = data;
      break;
    case goomf_HF_2:
      plugin->HF[2] = data;
      break;
    case goomf_HF_3:
      plugin->HF[3] = data;
      break;
    case goomf_HF_4:
      plugin->HF[4] = data;
      break;
    case goomf_HF_5:
      plugin->HF[5] = data;
      break;
    case goomf_Vol_0:
      plugin->Ovol[0] = data;
      break;
    case goomf_Vol_1:
      plugin->Ovol[1] = data;
      break;
    case goomf_Vol_2:
      plugin->Ovol[2] = data;
      break;
    case goomf_Vol_3:
      plugin->Ovol[3] = data;
      break;
    case goomf_Vol_4:
      plugin->Ovol[4] = data;
      break;
    case goomf_Vol_5:
      plugin->Ovol[5] = data;
      break;
    case goomf_Attack_0:
      plugin->attack[0] = data;
      break;
    case goomf_Attack_1:
      plugin->attack[1] = data;
      break;
    case goomf_Attack_2:
      plugin->attack[2] = data;
      break;
    case goomf_Attack_3:
      plugin->attack[3] = data;
      break;
    case goomf_Attack_4:
      plugin->attack[4] = data;
      break;
    case goomf_Attack_5:
      plugin->attack[5] = data;
      break;
    case goomf_Decay_0:
      plugin->decay[0] = data;
      break;
    case goomf_Decay_1:
      plugin->decay[1] = data;
      break;
    case goomf_Decay_2:
      plugin->decay[2] = data;
      break;
    case goomf_Decay_3:
      plugin->decay[3] = data;
      break;
    case goomf_Decay_4:
      plugin->decay[4] = data;
      break;
    case goomf_Decay_5:
      plugin->decay[5] = data;
      break;
    case goomf_Sustain_0:
      plugin->sustain[0] = data;
      break;
    case goomf_Sustain_1:
      plugin->sustain[1] = data;
      break;
    case goomf_Sustain_2:
      plugin->sustain[2] = data;
      break;
    case goomf_Sustain_3:
      plugin->sustain[3] = data;
      break;
    case goomf_Sustain_4:
      plugin->sustain[4] = data;
      break;
    case goomf_Sustain_5:
      plugin->sustain[5] = data;
      break;
    case goomf_Release_0:
      plugin->release[0] = data;
      break;
    case goomf_Release_1:
      plugin->release[1] = data;
      break;
    case goomf_Release_2:
      plugin->release[2] = data;
      break;
    case goomf_Release_3:
      plugin->release[3] = data;
      break;
    case goomf_Release_4:
      plugin->release[4] = data;
      break;
    case goomf_Release_5:
      plugin->release[5] = data;
      break;
    case goomf_pLFO_0:
      plugin->pLFO[0] = data;
      break;
    case goomf_pLFO_1:
      plugin->pLFO[1] = data;
      break;
    case goomf_pLFO_2:
      plugin->pLFO[2] = data;
      break;
    case goomf_pLFO_3:
      plugin->pLFO[3] = data;
      break;
    case goomf_pLFO_4:
      plugin->pLFO[4] = data;
      break;
    case goomf_pLFO_5:
      plugin->pLFO[5] = data;
      break;
    case goomf_FilterType:
      plugin->Ftype = data;
      break;
    case goomf_FilterGain:
      plugin->Fgain = data;
      break;
    case goomf_FilterCutoff:
      plugin->Fcutoff = data;
      break;
    case goomf_FilterQ:
      plugin->Fq = data;
      break;
    case goomf_FilterLFO:
      plugin->FLFO = data;
      break;
    case goomf_FilterADSR:
      plugin->FADSR = data;
      break;
    case goomf_FilterStages:
      plugin->Fstages = data;
      break;
    case goomf_FilterVelocity:
      plugin->Fvelocity = data;
      break;
    case goomf_Algo:
      plugin->algo = data;
      break;

    }

}


char *
goomfConfigure (LADSPA_Handle instance, const char *key, const char *value)
{
  int i;
  char *temp;
  goomf_synth_t *synth = (goomf_synth_t *) instance;
  
  temp=strdup(value);

  if (!strcmp (key, "names"))
    {
      for (i = 0; i < 80; i++)
	sprintf (synth->Banco[i].Name, "%s", strsep (&temp, ","));
      return NULL;
    }

  return strdup ("error: unrecognized configure key");
}


const DSSI_Program_Descriptor *
goomf_get_program (LADSPA_Handle handle, unsigned long index)
{
  static DSSI_Program_Descriptor pd;
  goomf_synth_t *synth = (goomf_synth_t *) handle;

  if (index < 80)
    {
      pd.Bank = 0;
      pd.Program = index;
      pd.Name = synth->Banco[index].Name;
      return &pd;
    }
  return NULL;
}

void
goomf_select_program (LADSPA_Handle handle, unsigned long bank,
		      unsigned long program)
{

  goomf_synth_t *synth = (goomf_synth_t *) handle;

  if (bank || program > 80)
    return;



}

static LADSPA_Handle
instantiategoomf (const LADSPA_Descriptor * descriptor, unsigned long s_rate)
{

  goomf_synth_t *synth = (goomf_synth_t *) calloc (1, sizeof (goomf_synth_t));

  synth->SAMPLE_RATE = (unsigned int) s_rate;
  init_vars(synth);
  Adjust_Audio(synth);
  pthread_mutex_init (&synth->mutex, NULL);

  return (LADSPA_Handle) synth;
}

static void
activategoomf (LADSPA_Handle instance)
{
  goomf_synth_t *synth = (goomf_synth_t *) instance;
  int i;
  for (i = 0; i < 6; i++)
    clear_synth (synth, i);


}


void
goomf_deactivate (LADSPA_Handle instance)
{
  goomf_synth_t *synth = (goomf_synth_t *) instance;
  int i;
  for (i = 0; i < 6; i++)
    clear_synth (synth, i);

}


static void
rungoomfWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  rungoomf (instance, sample_count, NULL, 0);
}

static void
rungoomf (LADSPA_Handle instance, unsigned long sample_count,
	  snd_seq_event_t * events, unsigned long event_count)
{
  goomf_synth_t *synth = (goomf_synth_t *) instance;
  LADSPA_Data *const outputl = synth->output_l;
  LADSPA_Data *const outputr = synth->output_r;
  LADSPA_Data vol = *(synth->master_volume);

  unsigned long event_pos = 0;
  int i;



  while (event_pos < event_count)
    {

      switch (events[event_pos].type)
	{
	case SND_SEQ_EVENT_PITCHBEND:
	  synth->pitch =
	    (float) events[event_pos].data.control.value / 8192.0;
	  break;

	case SND_SEQ_EVENT_PGMCHANGE:

	  if (events[event_pos].data.control.value < 80)
	    synth->preset = events[event_pos].data.control.value;
	  break;


	case SND_SEQ_EVENT_CONTROLLER:

	  if (events[event_pos].data.control.param == 1)
	    {
	      synth->modulation = (float)
		events[event_pos].data.control.value / 128;
	      break;
	    }

	  if (events[event_pos].data.control.param == 7)
	    {
	      *(synth->master_volume) =
		(LADSPA_Data) events[event_pos].data.control.value / 128.0 *
		10000.0 + 20.0;
	      break;
	    }

	  if (events[event_pos].data.control.param == 48)
	    {
	      *(synth->Fcutoff) = -1.0 +
		(LADSPA_Data) events[event_pos].data.control.value / 128.0 *
		2.0;
	      break;
	    }

	  if (events[event_pos].data.control.param == 49)
	    {
	      *(synth->Fq) =
		(LADSPA_Data) events[event_pos].data.control.value / 128.0;
	      break;
	    }
	  if (events[event_pos].data.control.param == 50)
	    {
	      *(synth->FLFO) =
		(LADSPA_Data) events[event_pos].data.control.value / 128.0;
	      break;
	    }





	  break;

	case SND_SEQ_EVENT_NOTEON:

	  if (events[event_pos].data.note.velocity != 0)
	    {
	      synth->note = events[event_pos].data.note.note;
	      synth->velocity =
		(float) events[event_pos].data.note.velocity/126.0;
	      synth->env_time = 0.0f;
	      synth->gate = 1;
              for(i=0; i<6; i++) synth->active[i]=1;
              synth->filt_cleared=0;
	      break;
	    }
	  else
	    {
	      if (events[event_pos].data.note.note == synth->note)
		{
		  synth->gate = 0;
		  synth->renv_time = 0.0f;
		}
	    }
	  break;

	case SND_SEQ_EVENT_NOTEOFF:
	  if ((int) events[event_pos].data.note.note == synth->note)
	    {
	      synth->gate = 0;
	      synth->renv_time = 0.0f;

	    }

	  break;
	}

      event_pos++;
    }


  Alg1s (synth, sample_count);
  for (i = 0; i < sample_count; i++)
    {
      outputl[i] = synth->bufl[i] * vol;
      outputr[i] = synth->bufr[i] * vol;
    }



}




int
getControllergoomf (LADSPA_Handle instance, unsigned long port)
{
  switch (port)
    {
    case goomf_VOLUME:
      return DSSI_CC (7);
    case goomf_FilterCutoff:
      return DSSI_CC (48);
    case goomf_FilterQ:
      return DSSI_CC (49);
    case goomf_FilterLFO:
      return DSSI_CC (50);
    }

  return -1;
}

void __attribute__ ((constructor)) goomf_init ()
{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;

  goomfLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));

  if (goomfLDescriptor)
    {
      goomfLDescriptor->UniqueID = 3992;
      goomfLDescriptor->Label = "goomf";
      goomfLDescriptor->Properties = 0;
      goomfLDescriptor->Name = "goomf";
      goomfLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      goomfLDescriptor->Copyright = "GNU General Public License version 2";
      goomfLDescriptor->PortCount = 72;


      port_descriptors = (LADSPA_PortDescriptor *)
	calloc (goomfLDescriptor->PortCount, sizeof (LADSPA_PortDescriptor));
      goomfLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;

      port_range_hints = (LADSPA_PortRangeHint *)
	calloc (goomfLDescriptor->PortCount, sizeof (LADSPA_PortRangeHint));
      goomfLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;

      port_names =
	(char **) calloc (goomfLDescriptor->PortCount, sizeof (char *));
      goomfLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Output L */
      port_descriptors[goomf_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[goomf_OUTPUT_L] = "Output L";
      port_range_hints[goomf_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[goomf_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[goomf_OUTPUT_R] = "Output R";
      port_range_hints[goomf_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Volume */
      port_descriptors[goomf_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_VOLUME] = "Volume";
      port_range_hints[goomf_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_VOLUME].LowerBound = 0.0;
      port_range_hints[goomf_VOLUME].UpperBound = 1.0;

      /* Parameters for MasterTune */
      port_descriptors[goomf_MASTERTUNE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_MASTERTUNE] = "Mastertune";
      port_range_hints[goomf_MASTERTUNE].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[goomf_MASTERTUNE].LowerBound = -1.0;
      port_range_hints[goomf_MASTERTUNE].UpperBound = 1.0;

      /* Parameters for Portamento */
      port_descriptors[goomf_PORTAMENTO] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_PORTAMENTO] = "Portamento";
      port_range_hints[goomf_PORTAMENTO].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_PORTAMENTO].LowerBound = 0.0;
      port_range_hints[goomf_PORTAMENTO].UpperBound = 1.0;

      /* Parameters for LFO Volume */
      port_descriptors[goomf_LFO_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_LFO_VOLUME] = "LFO Volume";
      port_range_hints[goomf_LFO_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_LFO_VOLUME].LowerBound = 0.0;
      port_range_hints[goomf_LFO_VOLUME].UpperBound = 1.0;


      /* Parameters for LFO Frequency */
      port_descriptors[goomf_LFO_FREQUENCY] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_LFO_FREQUENCY] = "LFO Frequency";
      port_range_hints[goomf_LFO_FREQUENCY].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_LFO_FREQUENCY].LowerBound = 0.0;
      port_range_hints[goomf_LFO_FREQUENCY].UpperBound = 1.0;

      /* Parameters for LFO Delay */
      port_descriptors[goomf_LFO_DELAY] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_LFO_DELAY] = "LFO Delay";
      port_range_hints[goomf_LFO_DELAY].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_LFO_DELAY].LowerBound = 0.0;
      port_range_hints[goomf_LFO_DELAY].UpperBound = 1.0;

      /* Parameters for LFO Wave */
      port_descriptors[goomf_LFO_WAVE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_LFO_WAVE] = "LFO Wave";
      port_range_hints[goomf_LFO_WAVE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_LFO_WAVE].LowerBound = 1.0;
      port_range_hints[goomf_LFO_WAVE].UpperBound = 9.0;


      /* Parameters for Wave_0 */
      port_descriptors[goomf_WAVE_0] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_WAVE_0] = "Wave_0";
      port_range_hints[goomf_WAVE_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_WAVE_0].LowerBound = 1.0;
      port_range_hints[goomf_WAVE_0].UpperBound = 9.0;

      /* Parameters for Wave_1 */
      port_descriptors[goomf_WAVE_1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_WAVE_1] = "Wave_1";
      port_range_hints[goomf_WAVE_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_WAVE_1].LowerBound = 1.0;
      port_range_hints[goomf_WAVE_1].UpperBound = 9.0;

      /* Parameters for Wave_2 */
      port_descriptors[goomf_WAVE_2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_WAVE_2] = "Wave_2";
      port_range_hints[goomf_WAVE_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_WAVE_2].LowerBound = 1.0;
      port_range_hints[goomf_WAVE_2].UpperBound = 9.0;

      /* Parameters for Wave_3 */
      port_descriptors[goomf_WAVE_3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_WAVE_3] = "Wave_3";
      port_range_hints[goomf_WAVE_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_WAVE_3].LowerBound = 1.0;
      port_range_hints[goomf_WAVE_3].UpperBound = 9.0;

      /* Parameters for Wave_4 */
      port_descriptors[goomf_WAVE_4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_WAVE_4] = "Wave_4";
      port_range_hints[goomf_WAVE_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_WAVE_4].LowerBound = 1.0;
      port_range_hints[goomf_WAVE_4].UpperBound = 9.0;

      /* Parameters for Wave_5 */
      port_descriptors[goomf_WAVE_5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_WAVE_5] = "Wave_5";
      port_range_hints[goomf_WAVE_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_WAVE_5].LowerBound = 1.0;
      port_range_hints[goomf_WAVE_5].UpperBound = 9.0;

      /* Parameters for H_0 */
      port_descriptors[goomf_H_0] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_H_0] = "H_0";
      port_range_hints[goomf_H_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_H_0].LowerBound = 0.0;
      port_range_hints[goomf_H_0].UpperBound = 21.0;


      /* Parameters for H_1 */
      port_descriptors[goomf_H_1] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_H_1] = "H_1";
      port_range_hints[goomf_H_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_H_1].LowerBound = 0.0;
      port_range_hints[goomf_H_1].UpperBound = 21.0;

      /* Parameters for H_2 */
      port_descriptors[goomf_H_2] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_H_2] = "H_2";
      port_range_hints[goomf_H_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_H_2].LowerBound = 0.0;
      port_range_hints[goomf_H_2].UpperBound = 21.0;

      /* Parameters for H_3 */
      port_descriptors[goomf_H_3] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_H_3] = "H_3";
      port_range_hints[goomf_H_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_H_3].LowerBound = 0.0;
      port_range_hints[goomf_H_3].UpperBound = 21.0;

      /* Parameters for H_4 */
      port_descriptors[goomf_H_4] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_H_4] = "H_4";
      port_range_hints[goomf_H_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_H_4].LowerBound = 0.0;
      port_range_hints[goomf_H_4].UpperBound = 21.0;

      /* Parameters for H_5 */
      port_descriptors[goomf_H_5] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_H_5] = "H_5";
      port_range_hints[goomf_H_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_H_5].LowerBound = 0.0;
      port_range_hints[goomf_H_5].UpperBound = 21.0;


      /* Parameters for HF_0 */
      port_descriptors[goomf_HF_0] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_HF_0] = "HF_0";
      port_range_hints[goomf_HF_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_HF_0].LowerBound = -0.02;
      port_range_hints[goomf_HF_0].UpperBound = 0.02;

      /* Parameters for HF_1 */
      port_descriptors[goomf_HF_1] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_HF_1] = "HF_1";
      port_range_hints[goomf_HF_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_HF_1].LowerBound = -0.02;
      port_range_hints[goomf_HF_1].UpperBound = 0.02;

      /* Parameters for HF_2 */
      port_descriptors[goomf_HF_2] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_HF_2] = "HF_2";
      port_range_hints[goomf_HF_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_HF_2].LowerBound = -0.02;
      port_range_hints[goomf_HF_2].UpperBound = 0.02;

      /* Parameters for HF_3 */
      port_descriptors[goomf_HF_3] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_HF_3] = "HF_3";
      port_range_hints[goomf_HF_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_HF_3].LowerBound = -0.02;
      port_range_hints[goomf_HF_3].UpperBound = 0.02;

      /* Parameters for HF_4 */
      port_descriptors[goomf_HF_4] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_HF_4] = "HF_4";
      port_range_hints[goomf_HF_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_HF_4].LowerBound = -0.02;
      port_range_hints[goomf_HF_4].UpperBound = 0.02;

      /* Parameters for HF_5 */
      port_descriptors[goomf_HF_5] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_HF_5] = "HF_5";
      port_range_hints[goomf_HF_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_HF_5].LowerBound = -0.02;
      port_range_hints[goomf_HF_5].UpperBound = 0.02;

      /* Parameters for Vol_0 */
      port_descriptors[goomf_Vol_0] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Vol_0] = "Vol_0";
      port_range_hints[goomf_Vol_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Vol_0].LowerBound = 0.0;
      port_range_hints[goomf_Vol_0].UpperBound = 1.0;

      /* Parameters for Vol_1 */
      port_descriptors[goomf_Vol_1] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Vol_1] = "Vol_1";
      port_range_hints[goomf_Vol_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Vol_1].LowerBound = 0.0;
      port_range_hints[goomf_Vol_1].UpperBound = 1.0;

      /* Parameters for Vol_2 */
      port_descriptors[goomf_Vol_2] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Vol_2] = "Vol_2";
      port_range_hints[goomf_Vol_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Vol_2].LowerBound = 0.0;
      port_range_hints[goomf_Vol_2].UpperBound = 1.0;

      /* Parameters for Vol_3 */
      port_descriptors[goomf_Vol_3] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Vol_3] = "Vol_3";
      port_range_hints[goomf_Vol_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Vol_3].LowerBound = 0.0;
      port_range_hints[goomf_Vol_3].UpperBound = 1.0;

      /* Parameters for Vol_4 */
      port_descriptors[goomf_Vol_4] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Vol_4] = "Vol_4";
      port_range_hints[goomf_Vol_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Vol_4].LowerBound = 0.0;
      port_range_hints[goomf_Vol_4].UpperBound = 1.0;

      /* Parameters for Vol_5 */
      port_descriptors[goomf_Vol_5] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Vol_5] = "Vol_5";
      port_range_hints[goomf_Vol_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Vol_5].LowerBound = 0.0;
      port_range_hints[goomf_Vol_5].UpperBound = 1.0;


      /* Parameters for Attack_0 */
      port_descriptors[goomf_Attack_0] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Attack_0] = "Attack_0";
      port_range_hints[goomf_Attack_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Attack_0].LowerBound = 0.01;
      port_range_hints[goomf_Attack_0].UpperBound = 4.0;

      /* Parameters for Attack_1 */
      port_descriptors[goomf_Attack_1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Attack_1] = "Attack_1";
      port_range_hints[goomf_Attack_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Attack_1].LowerBound = 0.01;
      port_range_hints[goomf_Attack_1].UpperBound = 4.0;

      /* Parameters for Attack_2 */
      port_descriptors[goomf_Attack_2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Attack_2] = "Attack_2";
      port_range_hints[goomf_Attack_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Attack_2].LowerBound = 0.01;
      port_range_hints[goomf_Attack_2].UpperBound = 4.0;

      /* Parameters for Attack_3 */
      port_descriptors[goomf_Attack_3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Attack_3] = "Attack_3";
      port_range_hints[goomf_Attack_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Attack_3].LowerBound = 0.01;
      port_range_hints[goomf_Attack_3].UpperBound = 4.0;

      /* Parameters for Attack_4 */
      port_descriptors[goomf_Attack_4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Attack_4] = "Attack_4";
      port_range_hints[goomf_Attack_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Attack_4].LowerBound = 0.01;
      port_range_hints[goomf_Attack_4].UpperBound = 4.0;

      /* Parameters for Attack_5 */
      port_descriptors[goomf_Attack_5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Attack_5] = "Attack_5";
      port_range_hints[goomf_Attack_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Attack_5].LowerBound = 0.01;
      port_range_hints[goomf_Attack_5].UpperBound = 4.0;


      /* Parameters for Decay_0 */
      port_descriptors[goomf_Decay_0] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Decay_0] = "Decay_0";
      port_range_hints[goomf_Decay_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Decay_0].LowerBound = 0.01;
      port_range_hints[goomf_Decay_0].UpperBound = 4.0;

      /* Parameters for Decay_1 */
      port_descriptors[goomf_Decay_1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Decay_1] = "Decay_1";
      port_range_hints[goomf_Decay_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Decay_1].LowerBound = 0.01;
      port_range_hints[goomf_Decay_1].UpperBound = 4.0;

      /* Parameters for Decay_2 */
      port_descriptors[goomf_Decay_2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Decay_2] = "Decay_2";
      port_range_hints[goomf_Decay_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Decay_2].LowerBound = 0.01;
      port_range_hints[goomf_Decay_2].UpperBound = 4.0;

      /* Parameters for Decay_3 */
      port_descriptors[goomf_Decay_3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Decay_3] = "Decay_3";
      port_range_hints[goomf_Decay_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Decay_3].LowerBound = 0.01;
      port_range_hints[goomf_Decay_3].UpperBound = 4.0;

      /* Parameters for Decay_4 */
      port_descriptors[goomf_Decay_4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Decay_4] = "Decay_4";
      port_range_hints[goomf_Decay_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Decay_4].LowerBound = 0.01;
      port_range_hints[goomf_Decay_4].UpperBound = 4.0;

      /* Parameters for Decay_5 */
      port_descriptors[goomf_Decay_5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Decay_5] = "Decay_5";
      port_range_hints[goomf_Decay_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Decay_5].LowerBound = 0.01;
      port_range_hints[goomf_Decay_5].UpperBound = 4.0;


      /* Parameters for Sustain_0 */
      port_descriptors[goomf_Sustain_0] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Sustain_0] = "Sustain_0";
      port_range_hints[goomf_Sustain_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Sustain_0].LowerBound = 0.0;
      port_range_hints[goomf_Sustain_0].UpperBound = 1.0;

      /* Parameters for Sustain_1 */
      port_descriptors[goomf_Sustain_1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Sustain_1] = "Sustain_1";
      port_range_hints[goomf_Sustain_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Sustain_1].LowerBound = 0.0;
      port_range_hints[goomf_Sustain_1].UpperBound = 1.0;

      /* Parameters for Sustain_2 */
      port_descriptors[goomf_Sustain_2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Sustain_2] = "Sustain_2";
      port_range_hints[goomf_Sustain_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Sustain_2].LowerBound = 0.0;
      port_range_hints[goomf_Sustain_2].UpperBound = 1.0;

      /* Parameters for Sustain_3 */
      port_descriptors[goomf_Sustain_3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Sustain_3] = "Sustain_3";
      port_range_hints[goomf_Sustain_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Sustain_3].LowerBound = 0.0;
      port_range_hints[goomf_Sustain_3].UpperBound = 1.0;

      /* Parameters for Sustain_4 */
      port_descriptors[goomf_Sustain_4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Sustain_4] = "Sustain_4";
      port_range_hints[goomf_Sustain_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Sustain_4].LowerBound = 0.0;
      port_range_hints[goomf_Sustain_4].UpperBound = 1.0;

      /* Parameters for Sustain_5 */
      port_descriptors[goomf_Sustain_5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Sustain_5] = "Sustain_5";
      port_range_hints[goomf_Sustain_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Sustain_5].LowerBound = 0.0;
      port_range_hints[goomf_Sustain_5].UpperBound = 1.0;


      /* Parameters for Release_0 */
      port_descriptors[goomf_Release_0] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Release_0] = "Release_0";
      port_range_hints[goomf_Release_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Release_0].LowerBound = 0.01;
      port_range_hints[goomf_Release_0].UpperBound = 1.0;

      /* Parameters for Release_1 */
      port_descriptors[goomf_Release_1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Release_1] = "Release_1";
      port_range_hints[goomf_Release_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Release_1].LowerBound = 0.01;
      port_range_hints[goomf_Release_1].UpperBound = 1.0;

      /* Parameters for Release_2 */
      port_descriptors[goomf_Release_2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Release_2] = "Release_2";
      port_range_hints[goomf_Release_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Release_2].LowerBound = 0.01;
      port_range_hints[goomf_Release_2].UpperBound = 1.0;

      /* Parameters for Release_3 */
      port_descriptors[goomf_Release_3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Release_3] = "Release_3";
      port_range_hints[goomf_Release_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Release_3].LowerBound = 0.01;
      port_range_hints[goomf_Release_3].UpperBound = 1.0;

      /* Parameters for Release_4 */
      port_descriptors[goomf_Release_4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Release_4] = "Release_4";
      port_range_hints[goomf_Release_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Release_4].LowerBound = 0.01;
      port_range_hints[goomf_Release_4].UpperBound = 1.0;

      /* Parameters for Release_5 */
      port_descriptors[goomf_Release_5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Release_5] = "Release_5";
      port_range_hints[goomf_Release_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_Release_5].LowerBound = 0.01;
      port_range_hints[goomf_Release_5].UpperBound = 1.0;

      /* Parameters for pLFO_0 */
      port_descriptors[goomf_pLFO_0] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_pLFO_0] = "pLFO_0";
      port_range_hints[goomf_pLFO_0].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_pLFO_0].LowerBound = 0.0;
      port_range_hints[goomf_pLFO_0].UpperBound = 1.0;

      /* Parameters for pLFO_1 */
      port_descriptors[goomf_pLFO_1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_pLFO_1] = "pLFO_1";
      port_range_hints[goomf_pLFO_1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_pLFO_1].LowerBound = 0.0;
      port_range_hints[goomf_pLFO_1].UpperBound = 1.0;

      /* Parameters for pLFO_2 */
      port_descriptors[goomf_pLFO_2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_pLFO_2] = "pLFO_2";
      port_range_hints[goomf_pLFO_2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_pLFO_2].LowerBound = 0.0;
      port_range_hints[goomf_pLFO_2].UpperBound = 1.0;

      /* Parameters for pLFO_3 */
      port_descriptors[goomf_pLFO_3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_pLFO_3] = "pLFO_3";
      port_range_hints[goomf_pLFO_3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_pLFO_3].LowerBound = 0.0;
      port_range_hints[goomf_pLFO_3].UpperBound = 1.0;

      /* Parameters for pLFO_4 */
      port_descriptors[goomf_pLFO_4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_pLFO_4] = "pLFO_4";
      port_range_hints[goomf_pLFO_4].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_pLFO_4].LowerBound = 0.0;
      port_range_hints[goomf_pLFO_4].UpperBound = 1.0;

      /* Parameters for pLFO_5 */
      port_descriptors[goomf_pLFO_5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_pLFO_5] = "pLFO_5";
      port_range_hints[goomf_pLFO_5].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_pLFO_5].LowerBound = 0.0;
      port_range_hints[goomf_pLFO_5].UpperBound = 1.0;

      /* Parameters for FilterType */
      port_descriptors[goomf_FilterType] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterType] = "Type";
      port_range_hints[goomf_FilterType].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterType].LowerBound = 0.0;
      port_range_hints[goomf_FilterType].UpperBound = 8.0;

      /* Parameters for FilterGain */
      port_descriptors[goomf_FilterGain] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterGain] = "Gain";
      port_range_hints[goomf_FilterGain].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterGain].LowerBound = 0.0;
      port_range_hints[goomf_FilterGain].UpperBound = 1.0;

      /* Parameters for FilterCutoff */
      port_descriptors[goomf_FilterCutoff] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterCutoff] = "Cutoff";
      port_range_hints[goomf_FilterCutoff].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterCutoff].LowerBound = 21.0;
      port_range_hints[goomf_FilterCutoff].UpperBound = 10020.0;

      /* Parameters for FilterQ */
      port_descriptors[goomf_FilterQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterQ] = "Resonance";
      port_range_hints[goomf_FilterQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterQ].LowerBound = -1.0;
      port_range_hints[goomf_FilterQ].UpperBound = 1.0;

      /* Parameters for FilterLFO */
      port_descriptors[goomf_FilterLFO] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterLFO] = "LFO";
      port_range_hints[goomf_FilterLFO].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterLFO].LowerBound = 0.0;
      port_range_hints[goomf_FilterLFO].UpperBound = 1.0;

      /* Parameters for FilterADSR */
      port_descriptors[goomf_FilterADSR] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterADSR] = "ADSR";
      port_range_hints[goomf_FilterADSR].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterADSR].LowerBound = 0.0;
      port_range_hints[goomf_FilterADSR].UpperBound = 6.0;

      /* Parameters for FilterStages */
      port_descriptors[goomf_FilterStages] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterStages] = "Stages";
      port_range_hints[goomf_FilterStages].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterStages].LowerBound = 1.0;
      port_range_hints[goomf_FilterStages].UpperBound = 5.0;

      /* Parameters for FilterVelocity */
      port_descriptors[goomf_FilterVelocity] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_FilterVelocity] = "Velocity";
      port_range_hints[goomf_FilterVelocity].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterVelocity].LowerBound = 0.0;
      port_range_hints[goomf_FilterVelocity].UpperBound = 1.0;

      /* Parameters for FilterStages */
      port_descriptors[goomf_Algo] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[goomf_Algo] = "Algorhytm";
      port_range_hints[goomf_Algo].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[goomf_FilterStages].LowerBound = 1.0;
      port_range_hints[goomf_FilterStages].UpperBound = 2.0;




      goomfLDescriptor->instantiate = instantiategoomf;
      goomfLDescriptor->connect_port = connectPortgoomf;
      goomfLDescriptor->activate = activategoomf;
      goomfLDescriptor->run = rungoomfWrapper;
      goomfLDescriptor->run_adding = NULL;
      goomfLDescriptor->set_run_adding_gain = NULL;
      goomfLDescriptor->deactivate = goomf_deactivate;
      goomfLDescriptor->cleanup = cleanupgoomf;


    }



  goomfDDescriptor = (DSSI_Descriptor *) malloc (sizeof (DSSI_Descriptor));
  if (goomfDDescriptor)
    {
      goomfDDescriptor->DSSI_API_Version = 1;
      goomfDDescriptor->LADSPA_Plugin = goomfLDescriptor;
      goomfDDescriptor->configure = goomfConfigure;
      goomfDDescriptor->get_program = goomf_get_program;
      goomfDDescriptor->get_midi_controller_for_port = getControllergoomf;
      goomfDDescriptor->select_program = goomf_select_program;
      goomfDDescriptor->run_synth = rungoomf;
      goomfDDescriptor->run_synth_adding = NULL;
      goomfDDescriptor->run_multiple_synths = NULL;
      goomfDDescriptor->run_multiple_synths_adding = NULL;
    }
}


void __attribute__ ((destructor)) goomf_fini ()
{
  if (goomfLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) goomfLDescriptor->PortDescriptors);
      free ((char **) goomfLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) goomfLDescriptor->PortRangeHints);
      free (goomfLDescriptor);
    }
  if (goomfDDescriptor)
    {
      free (goomfDDescriptor);
    }
}
