/*
  exciter - a MIDI helped exciter DSSI plugin software

  exciter.c  -  Main dssi file of the plugin
  Copyright (C) 2008-2009 Josep Andreu (Holborn)
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
#include <stdio.h>
#include <stdlib.h>
#include <lo/lo.h>
#include <string.h>
#include "exciter.h"


#define exciter_INPUT_L 0
#define exciter_INPUT_R 1
#define exciter_OUTPUT_L 2
#define exciter_OUTPUT_R 3
#define exciter_DRY 4
#define exciter_VOLUME 5
#define exciter_PAN 6
#define exciter_MAG_1 7
#define exciter_MAG_2 8
#define exciter_MAG_3 9
#define exciter_MAG_4 10
#define exciter_MAG_5 11
#define exciter_MAG_6 12
#define exciter_MAG_7 13
#define exciter_MAG_8 14
#define exciter_MAG_9 15
#define exciter_MAG_10 16
#define exciter_FFREQ 17



static LADSPA_Descriptor *exciterLDescriptor = NULL;
static DSSI_Descriptor *exciterDDescriptor = NULL;

static void runexciter (LADSPA_Handle instance, unsigned long sample_count,
			snd_seq_event_t * events, unsigned long EventCount);

static void cleanupexciter (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return exciterLDescriptor;
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
      return exciterDDescriptor;
    default:
      return NULL;
    }
}

static void
cleanupexciter (LADSPA_Handle instance)
{
  exciter_t *exciter = (exciter_t *) instance;
  free (exciter);
}

static void
connectPortexciter (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  exciter_t *plugin = (exciter_t *) instance;
  switch (port)
    {
    case exciter_INPUT_L:
      plugin->inputl = data;
      break;
    case exciter_INPUT_R:
      plugin->inputr = data;
      break;
    case exciter_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case exciter_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case exciter_DRY:
      plugin->dry = data;
      break;
    case exciter_VOLUME:
      plugin->outvolume = data;
      break;
    case exciter_PAN:
      plugin->panning = data;
      break;
    case exciter_MAG_1:
      plugin->mag_1 = data;
      break;
    case exciter_MAG_2:
      plugin->mag_2 = data;
      break;
    case exciter_MAG_3:
      plugin->mag_3 = data;
      break;
    case exciter_MAG_4:
      plugin->mag_4 = data;
      break;
    case exciter_MAG_5:
      plugin->mag_5 = data;
      break;
    case exciter_MAG_6:
      plugin->mag_6 = data;
      break;
    case exciter_MAG_7:
      plugin->mag_7 = data;
      break;
    case exciter_MAG_8:
      plugin->mag_8 = data;
      break;
    case exciter_MAG_9:
      plugin->mag_9 = data;
      break;
    case exciter_MAG_10:
      plugin->mag_10 = data;
      break;
    case exciter_FFREQ:
      plugin->ffreq = data;
      break;

    }
}

static char *
osc_build_path (char *base_path, const char *method)
{
  char buffer[256];
  char *full_path;

  snprintf (buffer, 256, "%s%s", base_path, method);
  if (!(full_path = strdup(buffer)))
    {
      printf ("out of memory!\n");
      exit (1);
    }
  return strdup (buffer);
}

static void
osc_error (int num, const char *msg, const char *path)
{
  printf (" error: liblo server error %d in path \"%s\": %s\n",
	  num, (path ? path : "(null)"), msg);
}

static void
create_osc (LADSPA_Handle instance, const char *value)
{

  exciter_t *s = (exciter_t *) instance;
  printf ("value = %s\n", value);

  char *temp;
  char *host = lo_url_get_hostname (value);
  char *port = lo_url_get_port (value);
  char *path = lo_url_get_path (value);

  s->osc_server = 0;
  s->m_host = lo_address_new (host, port);
  s->osc_configure_path = osc_build_path (path, "/configure");
  s->osc_server = lo_server_new (NULL, osc_error);
  lo_server_add_method (s->osc_server, s->osc_configure_path, "ss", NULL,
			NULL);
  temp = lo_server_get_url (s->osc_server);

  return;

}


char *
exciterConfigure (LADSPA_Handle instance, const char *key, const char *value)
{

  if (!strcmp (key, "load"))
    {
      return NULL;
    }

  if (!strcmp (key, "url"))
    {
      create_osc (instance, value);
      return NULL;
    }


  return strdup ("error: unrecognized configure key");
}


const DSSI_Program_Descriptor *
exciter_get_program (LADSPA_Handle handle, unsigned long index)
{
  static DSSI_Program_Descriptor pd;


  if (index < 32)
    {
      pd.Bank = 0;
      pd.Program = index + 1;
      pd.Name = "Empty";
      return &pd;
    }


  return NULL;
}

void
exciter_select_program (LADSPA_Handle handle, unsigned long bank,
			unsigned long program)
{

}


static LADSPA_Handle
instantiateexciter (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{

  exciter_t *exciter = (exciter_t *) calloc (1, sizeof (exciter_t));

  RecChord_Init (exciter);
  exciter->SAMPLE_RATE = s_rate;
  Harmonic_Init (exciter);
  return (LADSPA_Handle) exciter;
}

static void
activateexciter (LADSPA_Handle instance)
{

//  exciter_t *exciter = (exciter_t *) instance;
}


void
exciter_deactivate (LADSPA_Handle instance)
{
  exciter_t *exciter = (exciter_t *) instance;
  RecChord_Cleanup (exciter);
}


static void
runexciterWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runexciter (instance, sample_count, NULL, 0);
}

static void
runexciter (LADSPA_Handle instance, unsigned long sample_count,
	    snd_seq_event_t * events, unsigned long event_count)
{

  exciter_t *exc = (exciter_t *) instance;

  LADSPA_Data *const pinputl = exc->inputl;
  LADSPA_Data *const pinputr = exc->inputr;
//  LADSPA_Data dry = *(exc->dry);
  LADSPA_Data vol = *(exc->outvolume);
  LADSPA_Data pan = *(exc->panning);
  LADSPA_Data mag_1 = *(exc->mag_1);
  LADSPA_Data mag_2 = *(exc->mag_2);
  LADSPA_Data mag_3 = *(exc->mag_3);
  LADSPA_Data mag_4 = *(exc->mag_4);
  LADSPA_Data mag_5 = *(exc->mag_5);
  LADSPA_Data mag_6 = *(exc->mag_6);
  LADSPA_Data mag_7 = *(exc->mag_7);
  LADSPA_Data mag_8 = *(exc->mag_8);
  LADSPA_Data mag_9 = *(exc->mag_9);
  LADSPA_Data mag_10 = *(exc->mag_10);
  LADSPA_Data ffreq = *(exc->ffreq);

  unsigned long event_pos = 0;
  unsigned long pos;
  unsigned long count;

  int i;

//  memset (exc->efxoutl, 0, sizeof (LADSPA_Data) * sample_count);
//  memset (exc->efxoutr, 0, sizeof (LADSPA_Data) * sample_count);

  for (pos = 0, event_pos = 0; pos < sample_count; pos += STEP_SIZE)
    {

      while (event_pos < event_count)
	{

	  switch (events[event_pos].type)
	    {

	    case SND_SEQ_EVENT_NOTEON:

	      for (i = 0; i < POLY; i++)
		{
		  if (events[event_pos].data.note.velocity != 0)
		    {
		      if (exc->rc.note_active[i] == 0)
			{
			  exc->rc.note_active[i] = 1;
			  exc->rc.rnote[i] = events[event_pos].data.note.note;
			  exc->rc.gate[i] = 1;
			  MiraChord (exc);
			  calcula_mag (exc);
			  if (exc->rc.cc)
			    {
			      lo_send (exc->m_host,
				       exc->osc_configure_path, "ss",
				       "schord", exc->rc.NombreAcorde);
			      exc->rc.cc = 0;
			    }
			  break;
			}

		    }
		  if (events[event_pos].data.note.velocity == 0)
		    {
		      if ((exc->rc.note_active[i])
			  && (exc->rc.rnote[i] ==
			      events[event_pos].data.note.note))
			{
			  exc->rc.note_active[i] = 0;
			  exc->rc.gate[i] = 0;
			  break;
			}

		    }
		}


	      break;
	    case SND_SEQ_EVENT_NOTEOFF:

	      for (i = 0; i < POLY; i++)
		{
		  if ((exc->rc.note_active[i])
		      && (exc->rc.rnote[i] ==
			  events[event_pos].data.note.note))
		    {
		      exc->rc.note_active[i] = 0;
		      exc->rc.gate[i] = 0;
		      break;
		    }

		}

	      break;
	    case SND_SEQ_EVENT_CONTROLLER:

	      if (events[event_pos].data.control.param == 10)
		{
		  pan = (float) events[event_pos].data.control.value / 128.0;
		  break;
		}

	      if (events[event_pos].data.control.param == 7)
		{
		  vol = (float) events[event_pos].data.control.value / 128.0;
		  break;
		}

	      if (events[event_pos].data.control.param == 48)
		{
		  mag_1 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 49)
		{
		  mag_2 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 50)
		{
		  mag_3 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 51)
		{
		  mag_4 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 52)
		{
		  mag_5 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 53)
		{
		  mag_6 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 54)
		{
		  mag_7 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 55)
		{
		  mag_8 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 56)
		{
		  mag_9 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 57)
		{
		  mag_10 =
		    (float) ((events[event_pos].data.control.value -
			      64.0) / 64.0);
		  break;
		}
	      if (events[event_pos].data.control.param == 58)
		{
		  ffreq =
		    (float) 21.0 +
		    (float) (events[event_pos].data.control.value * 10000.0 /
			     127.0);
		  break;
		}

	    }

	  event_pos++;
	}


      count =
	(sample_count - pos) > STEP_SIZE ? STEP_SIZE : sample_count - pos;

      if (exc->rmag_1 != mag_1)
	{
	  exc->rmag_1 = mag_1;
	  calcula_mag (exc);
	}

      if (exc->rmag_2 != mag_2)
	{
	  exc->rmag_2 = mag_2;
	  calcula_mag (exc);
	}

      if (exc->rmag_3 != mag_3)
	{
	  exc->rmag_3 = mag_3;
	  calcula_mag (exc);
	}

      if (exc->rmag_4 != mag_4)
	{
	  exc->rmag_4 = mag_4;
	  calcula_mag (exc);
	}

      if (exc->rmag_5 != mag_5)
	{
	  exc->rmag_5 = mag_5;
	  calcula_mag (exc);
	}

      if (exc->rmag_6 != mag_6)
	{
	  exc->rmag_6 = mag_6;
	  calcula_mag (exc);
	}

      if (exc->rmag_7 != mag_7)
	{
	  exc->rmag_7 = mag_7;
	  calcula_mag (exc);
	}

      if (exc->rmag_8 != mag_8)
	{
	  exc->rmag_8 = mag_8;
	  calcula_mag (exc);
	}

      if (exc->rmag_9 != mag_9)
	{
	  exc->rmag_9 = mag_9;
	  calcula_mag (exc);
	}

      if (exc->rmag_10 != mag_10)
	{
	  exc->rmag_10 = mag_10;
	  calcula_mag (exc);
	}


      if (exc->rfreq != ffreq)
	{
	  exc->rfreq = ffreq;
	  setfreq (exc, &exc->lhpf, exc->rfreq);
	  setfreq (exc, &exc->rhpf, exc->rfreq);
	}

      out_exciter (exc, pinputl, pinputr, count);

    }

}
int 
getControllerexciter (LADSPA_Handle instance, unsigned long port)
  {
    switch ((int) port)
      {
      case exciter_VOLUME:
	return DSSI_CC (7);
      case exciter_PAN:
	return DSSI_CC (10);
      case exciter_MAG_1:
	return DSSI_CC (48);
      case exciter_MAG_2:
	return DSSI_CC (49);
      case exciter_MAG_3:
	return DSSI_CC (50);
      case exciter_MAG_4:
	return DSSI_CC (51);
      case exciter_MAG_5:
	return DSSI_CC (52);
      case exciter_MAG_6:
	return DSSI_CC (53);
      case exciter_MAG_7:
	return DSSI_CC (54);
      case exciter_MAG_8:
	return DSSI_CC (55);
      case exciter_MAG_9:
	return DSSI_CC (56);
      case exciter_MAG_10:
	return DSSI_CC (57);
      case exciter_FFREQ:
	return DSSI_CC (58);

      }

    return -1;
  }


  void __attribute__ ((constructor)) exciter_init ()
  {


    char **port_names;
    LADSPA_PortDescriptor *port_descriptors;
    LADSPA_PortRangeHint *port_range_hints;
    exciterLDescriptor =
      (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
    if (exciterLDescriptor)
      {
	exciterLDescriptor->UniqueID = 3991;
	exciterLDescriptor->Label = "exciter";
	exciterLDescriptor->Properties = 0;
	exciterLDescriptor->Name = "exciter";
	exciterLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
	exciterLDescriptor->Copyright =
	  "GNU General Public License version 2";
	exciterLDescriptor->PortCount = 18;
	port_descriptors =
	  (LADSPA_PortDescriptor *)
	  calloc (exciterLDescriptor->PortCount,
		  sizeof (LADSPA_PortDescriptor));
	exciterLDescriptor->PortDescriptors =
	  (const LADSPA_PortDescriptor *) port_descriptors;
	port_range_hints =
	  (LADSPA_PortRangeHint *) calloc (exciterLDescriptor->PortCount,
					   sizeof (LADSPA_PortRangeHint));
	exciterLDescriptor->PortRangeHints =
	  (const LADSPA_PortRangeHint *) port_range_hints;
	port_names =
	  (char **) calloc (exciterLDescriptor->PortCount, sizeof (char *));
	exciterLDescriptor->PortNames = (const char **) port_names;
	/* Parameters for Input L */
	port_descriptors[exciter_INPUT_L] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
	port_names[exciter_INPUT_L] = "Input L";
	port_range_hints[exciter_INPUT_L].HintDescriptor = 0;
	/* Parameters for Input R */
	port_descriptors[exciter_INPUT_R] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
	port_names[exciter_INPUT_R] = "Input R";
	port_range_hints[exciter_INPUT_R].HintDescriptor = 0;
	/* Parameters for Output L */
	port_descriptors[exciter_OUTPUT_L] =
	  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
	port_names[exciter_OUTPUT_L] = "Output L";
	port_range_hints[exciter_OUTPUT_L].HintDescriptor = 0;
	/* Parameters for Output R */
	port_descriptors[exciter_OUTPUT_R] =
	  LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
	port_names[exciter_OUTPUT_R] = "Output R";
	port_range_hints[exciter_OUTPUT_R].HintDescriptor = 0;
	/* Parameters for Dry */
	port_descriptors[exciter_DRY] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_DRY] = "Dry";
	port_range_hints[exciter_DRY].HintDescriptor =
	  LADSPA_HINT_DEFAULT_MINIMUM |
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
	port_range_hints[exciter_DRY].LowerBound = 0.0;
	port_range_hints[exciter_DRY].UpperBound = 1.0;
	/* Parameters for Volume */
	port_descriptors[exciter_VOLUME] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_VOLUME] = "Volume";
	port_range_hints[exciter_VOLUME].HintDescriptor =
	  LADSPA_HINT_DEFAULT_MAXIMUM |
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
	port_range_hints[exciter_VOLUME].LowerBound = 0.0;
	port_range_hints[exciter_VOLUME].UpperBound = 1.0;
	/* Parameters for Pan */
	port_descriptors[exciter_PAN] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_PAN] = "Pan";
	port_range_hints[exciter_PAN].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_MIDDLE;
	port_range_hints[exciter_PAN].LowerBound = 0.0;
	port_range_hints[exciter_PAN].UpperBound = 1.0;

	/* Parameters for Har 1 */
	port_descriptors[exciter_MAG_1] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_1] = "Harmonic 1";
	port_range_hints[exciter_MAG_1].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_1].LowerBound = -1;
	port_range_hints[exciter_MAG_1].UpperBound = +1;

	/* Parameters for Har 2 */
	port_descriptors[exciter_MAG_2] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_2] = "Harmonic 2";
	port_range_hints[exciter_MAG_2].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_2].LowerBound = -1;
	port_range_hints[exciter_MAG_2].UpperBound = +1;

	/* Parameters for Har 3 */
	port_descriptors[exciter_MAG_3] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_3] = "Harmonic 3";
	port_range_hints[exciter_MAG_3].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_3].LowerBound = -1;
	port_range_hints[exciter_MAG_3].UpperBound = +1;

	/* Parameters for Har 4 */
	port_descriptors[exciter_MAG_4] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_4] = "Harmonic 4";
	port_range_hints[exciter_MAG_4].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_4].LowerBound = -1;
	port_range_hints[exciter_MAG_4].UpperBound = +1;

	/* Parameters for Har 5 */
	port_descriptors[exciter_MAG_5] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_5] = "Harmonic 5";
	port_range_hints[exciter_MAG_5].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_5].LowerBound = -1;
	port_range_hints[exciter_MAG_5].UpperBound = +1;

	/* Parameters for Har 6 */
	port_descriptors[exciter_MAG_6] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_6] = "Harmonic 6";
	port_range_hints[exciter_MAG_6].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_6].LowerBound = -1;
	port_range_hints[exciter_MAG_6].UpperBound = +1;

	/* Parameters for Har 7 */
	port_descriptors[exciter_MAG_7] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_7] = "Harmonic 7";
	port_range_hints[exciter_MAG_7].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_7].LowerBound = -1;
	port_range_hints[exciter_MAG_7].UpperBound = +1;

	/* Parameters for Har 8 */
	port_descriptors[exciter_MAG_8] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_8] = "Harmonic 8";
	port_range_hints[exciter_MAG_8].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_8].LowerBound = -1;
	port_range_hints[exciter_MAG_8].UpperBound = +1;
	/* Parameters for Har 9 */
	port_descriptors[exciter_MAG_9] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_9] = "Harmonic 9";
	port_range_hints[exciter_MAG_9].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_9].LowerBound = -1;
	port_range_hints[exciter_MAG_9].UpperBound = +1;

	/* Parameters for Har 10 */
	port_descriptors[exciter_MAG_10] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_MAG_10] = "Harmonic 10";
	port_range_hints[exciter_MAG_10].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_0;
	port_range_hints[exciter_MAG_10].LowerBound = -1;
	port_range_hints[exciter_MAG_10].UpperBound = +1;

	/* Parameters for Filter Frequency */
	port_descriptors[exciter_FFREQ] =
	  LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
	port_names[exciter_FFREQ] = "Filter Frequency";
	port_range_hints[exciter_FFREQ].HintDescriptor =
	  LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	  LADSPA_HINT_DEFAULT_MINIMUM;
	port_range_hints[exciter_FFREQ].LowerBound = 21.0;
	port_range_hints[exciter_FFREQ].UpperBound = 10020.0;


	exciterLDescriptor->instantiate = instantiateexciter;
	exciterLDescriptor->connect_port = connectPortexciter;
	exciterLDescriptor->activate = activateexciter;
	exciterLDescriptor->run = runexciterWrapper;
	exciterLDescriptor->run_adding = NULL;
	exciterLDescriptor->set_run_adding_gain = NULL;
	exciterLDescriptor->deactivate = exciter_deactivate;
	exciterLDescriptor->cleanup = cleanupexciter;
      }



    exciterDDescriptor =
      (DSSI_Descriptor *) malloc (sizeof (DSSI_Descriptor));
    if (exciterDDescriptor)
      {
	exciterDDescriptor->DSSI_API_Version = 1;
	exciterDDescriptor->LADSPA_Plugin = exciterLDescriptor;
	exciterDDescriptor->configure = exciterConfigure;
	exciterDDescriptor->get_program = exciter_get_program;
	exciterDDescriptor->get_midi_controller_for_port =
	  getControllerexciter;
	exciterDDescriptor->select_program = exciter_select_program;
	exciterDDescriptor->run_synth = runexciter;
	exciterDDescriptor->run_synth_adding = NULL;
	exciterDDescriptor->run_multiple_synths = NULL;
	exciterDDescriptor->run_multiple_synths_adding = NULL;
      }
  }


  void __attribute__ ((destructor)) exciter_fini ()
  {
    if (exciterLDescriptor)
      {
	free ((LADSPA_PortDescriptor *) exciterLDescriptor->PortDescriptors);
	free ((char **) exciterLDescriptor->PortNames);
	free ((LADSPA_PortRangeHint *) exciterLDescriptor->PortRangeHints);
	free (exciterLDescriptor);
      }
    if (exciterDDescriptor)
      {
	free (exciterDDescriptor);
      }
  }
