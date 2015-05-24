/*
  horgand-dssi - a syhth-organ plugin software

  horgand.c  -  Main dssi file of the plugin
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
(version2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/


#include "Holrgan.h"
#include <math.h>
#include <dssi.h>
#include <ladspa.h>

#define horgand_OUTPUT_L 0
#define horgand_OUTPUT_R 1
#define horgand_VOLUME 2
#define horgand_MASTERTUNE 3


static LADSPA_Descriptor *horgandLDescriptor = NULL;
static DSSI_Descriptor *horgandDDescriptor = NULL;

static void runhorgand (LADSPA_Handle instance, unsigned long sample_count,
			snd_seq_event_t * events, unsigned long EventCount);


static void cleanuphorgand (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return horgandLDescriptor;
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
      return horgandDDescriptor;
    default:
      return NULL;
    }
}

static void
cleanuphorgand (LADSPA_Handle instance)
{
  horgand_synth_t *synth = (horgand_synth_t *) instance;
  free (synth);

}

static void
connectPorthorgand (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  horgand_synth_t *plugin = (horgand_synth_t *) instance;
  switch (port)
    {
    case horgand_OUTPUT_L:
      plugin->output_l = data;
      break;
    case horgand_OUTPUT_R:
      plugin->output_r = data;
      break;
    case horgand_VOLUME:
      plugin->vol = data;
      break;
    case horgand_MASTERTUNE:
      plugin->tune = data;
      break;
    }

}



char *
horgandConfigure (LADSPA_Handle instance, const char *key, const char *value)
{
  int i = 0;
  horgand_synth_t *synth = (horgand_synth_t *) instance;

  if (!strcmp (key, "load"))
    {
      i = loadbank (synth, value);
      if (i)
	return strdup ("Error loading file");
      else
	return NULL;
    }

  return strdup ("error: unrecognized configure key");
}

const DSSI_Program_Descriptor *
horgand_get_program (LADSPA_Handle handle, unsigned long index)
{
  static DSSI_Program_Descriptor pd;
  horgand_synth_t *synth = (horgand_synth_t *) handle;

  if (index < 32)
    {
      pd.Bank = 0;
      pd.Program = index + 1;
      pd.Name = synth->Banco[index + 1].Name;
      return &pd;
    }
  return NULL;
}

void
horgand_select_program (LADSPA_Handle handle, unsigned long bank,
			unsigned long program)
{

  horgand_synth_t *synth = (horgand_synth_t *) handle;
  if (program > 32)
    return;
  Put_Combi_t (synth, program);

}





static LADSPA_Handle
instantiatehorgand (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{

  horgand_synth_t *synth =
    (horgand_synth_t *) calloc (1, sizeof (horgand_synth_t));

  synth->SAMPLE_RATE = (unsigned int) s_rate;
  init_vars (synth);
  Adjust_Audio (synth);
  sprintf (synth->BankFilename, "%s/Default.horeb", DATADIR);
  loadbank(synth, synth->BankFilename);

  return (LADSPA_Handle) synth;
}

static void
activatehorgand (LADSPA_Handle instance)
{

  horgand_synth_t *synth = (horgand_synth_t *) instance;
  panic (synth);

}


void
horgand_deactivate (LADSPA_Handle instance)
{
  horgand_synth_t *synth = (horgand_synth_t *) instance;
  panic (synth);

}


static void
runhorgandWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runhorgand (instance, sample_count, NULL, 0);
  
}



static void
runhorgand (LADSPA_Handle instance, unsigned long sample_count,
	    snd_seq_event_t * events, unsigned long event_count)
{
  horgand_synth_t *synth = (horgand_synth_t *) instance;

  LADSPA_Data *const outl = synth->output_l;
  LADSPA_Data *const outr = synth->output_r;
  LADSPA_Data vol = *(synth->vol);

  unsigned long event_pos = 0;
  int i, l1;
   
      while (event_pos < event_count)
	{
	  switch (events[event_pos].type)
	    {
	    case SND_SEQ_EVENT_PITCHBEND:
	      synth->pitch =
		(float) events[event_pos].data.control.value / 8192.0f;
	      break;

	    case SND_SEQ_EVENT_PGMCHANGE:

	      if ((events[event_pos].data.control.value > 0)
		  && (events[event_pos].data.control.value < 33))
		synth->preset = events[event_pos].data.control.value;
	      break;


	    case SND_SEQ_EVENT_CONTROLLER:

	      if (events[event_pos].data.control.param == 1)
		{
		  synth->a.modulation =(float)
		    events[event_pos].data.control.value / 128;
		  Calc_LFO_Frequency (synth);
		  Calc_Chorus_LFO_Frequency (synth);
		  break;
		}

	      if (events[event_pos].data.control.param == 91)
		{
		  synth->Reverb_Volume =
		    (float) events[event_pos].data.control.value / 256.0f;
		  break;
		}
	      if (events[event_pos].data.control.param == 93)
		{
		  synth->a.Chorus_Volume =
		    (float) events[event_pos].data.control.value / 128.0f;
		  break;
		}
	      if (events[event_pos].data.control.param == 7)
		{
		  synth->Master_Volume =
		    (float) events[event_pos].data.control.value / 128.0f;
		  break;
		}
	      if (events[event_pos].data.control.param == 64)
		{
		  if (events[event_pos].data.control.value < 64)
		    {
		      synth->pedal = 0;
		      for (l1 = 0; l1 < POLY; l1++)
			if (synth->gate[l1] == 0)
			  synth->env_time[l1] = 0;
		    }
		  if (events[event_pos].data.control.value > 63)
		    synth->pedal = 1;
		}
	      break;

	    case SND_SEQ_EVENT_NOTEON:

	      if (events[event_pos].data.note.velocity != 0)
		{
		  for (l1 = 0; l1 < POLY; l1++)
		    {
		      if (!synth->note_active[l1])
			{
			  synth->note[l1] = events[event_pos].data.note.note;
			  synth->velocity[l1] =
			    events[event_pos].data.note.velocity / 126.0f;
			  if (synth->a.scaling)
			    synth->velocity[l1] =
			      Get_Keyb_Level_Scaling (synth, l1);
			  if (synth->velocity[l1] > 1.0f)
			    synth->velocity[l1] = 1.0f;
			  synth->env_time[l1] = 0;
			  synth->gate[l1] = 1;
			  synth->note_active[l1] = 1;
			  break;
			}
		    }
		  break;
		}
	      else
		{
		  for (l1 = 0; l1 < POLY; l1++)
		    {
		      if (synth->gate[l1] && synth->note_active[l1]
			  && (synth->note[l1] ==
			      events[event_pos].data.note.note))
			{
			  synth->gate[l1] = 0;
			  if (synth->pedal == 0)
			    synth->env_time[l1] = 0;
                          break;
			}

		    }
		}
	      break;
	    case SND_SEQ_EVENT_NOTEOFF:
	      for (l1 = 0; l1 < POLY; l1++)
		{
		  if (synth->gate[l1] && synth->note_active[l1]
		      && (synth->note[l1] ==
			  events[event_pos].data.note.note))
		    {
		      synth->gate[l1] = 0;
		      if (synth->pedal == 0)
			synth->env_time[l1] = 0;
			break;
		    }

		}
	      break;
	    }

	  event_pos++;
	}

      Alg1s(synth, sample_count);
       
      for (i=0; i<sample_count; i++)
	{
	  outl[i] = synth->bufl[i] * vol;
	  outr[i] = synth->bufr[i] * vol;
	}





}



int
getControllerhorgand (LADSPA_Handle instance, unsigned long port)
{
  switch (port)
    {
    case horgand_VOLUME:
      return DSSI_CC (7);
    }
  return -1;
}

void __attribute__((constructor))
horgand_init()
{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;

  horgandLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));

  if (horgandLDescriptor)
    {
      horgandLDescriptor->UniqueID = 3981;
      horgandLDescriptor->Label = "horgand";
      horgandLDescriptor->Properties = 0;
      horgandLDescriptor->Name = "horgand";
      horgandLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      horgandLDescriptor->Copyright = "GNU General Public License version 2";
      horgandLDescriptor->PortCount = 4;


      port_descriptors = (LADSPA_PortDescriptor *)
	calloc (horgandLDescriptor->PortCount, sizeof
		(LADSPA_PortDescriptor));
      horgandLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;

      port_range_hints = (LADSPA_PortRangeHint *)
	calloc (horgandLDescriptor->PortCount, sizeof (LADSPA_PortRangeHint));
      horgandLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;

      port_names =
	(char **) calloc (horgandLDescriptor->PortCount, sizeof (char *));
      horgandLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Output L */
      port_descriptors[horgand_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[horgand_OUTPUT_L] = "Output L";
      port_range_hints[horgand_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[horgand_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[horgand_OUTPUT_R] = "Output R";
      port_range_hints[horgand_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Volume */
      port_descriptors[horgand_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[horgand_VOLUME] = "Volume";
      port_range_hints[horgand_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[horgand_VOLUME].LowerBound = 0.0;
      port_range_hints[horgand_VOLUME].UpperBound = 1.0;

      /* Parameters for MasterTune */
      port_descriptors[horgand_MASTERTUNE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[horgand_MASTERTUNE] = "Mastertune";
      port_range_hints[horgand_MASTERTUNE].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[horgand_MASTERTUNE].LowerBound = -1.0;
      port_range_hints[horgand_MASTERTUNE].UpperBound = 1.0;


      horgandLDescriptor->instantiate = instantiatehorgand;
      horgandLDescriptor->connect_port = connectPorthorgand;
      horgandLDescriptor->activate = activatehorgand;
      horgandLDescriptor->run = runhorgandWrapper;
      horgandLDescriptor->run_adding = NULL;
      horgandLDescriptor->set_run_adding_gain = NULL;
      horgandLDescriptor->deactivate = horgand_deactivate;
      horgandLDescriptor->cleanup = cleanuphorgand;



    }



  horgandDDescriptor = (DSSI_Descriptor *) malloc (sizeof (DSSI_Descriptor));
  if (horgandDDescriptor)
    {
      horgandDDescriptor->DSSI_API_Version = 1;
      horgandDDescriptor->LADSPA_Plugin = horgandLDescriptor;
      horgandDDescriptor->configure = horgandConfigure;
      horgandDDescriptor->get_program = horgand_get_program;
      horgandDDescriptor->get_midi_controller_for_port = getControllerhorgand;
      horgandDDescriptor->select_program = horgand_select_program;
      horgandDDescriptor->run_synth = runhorgand;
      horgandDDescriptor->run_synth_adding = NULL;
      horgandDDescriptor->run_multiple_synths = NULL;
      horgandDDescriptor->run_multiple_synths_adding = NULL;
    }
}


void __attribute__ ((destructor))
horgand_fini()
{
  if (horgandLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) horgandLDescriptor->PortDescriptors);
      free ((char **) horgandLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) horgandLDescriptor->PortRangeHints);
      free (horgandLDescriptor);
    }
  if (horgandDDescriptor)
    {
      free (horgandDDescriptor);
    }
}
