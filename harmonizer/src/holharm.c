/*
  holharm - a inteligent harmonizer DSSI plugin software

  holharm.c  -  Main dssi file of the plugin
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
#include <string.h>
#include <lo/lo.h>
#include "harmonizer.h"


#define holharm_INPUT_L 0
#define holharm_INPUT_R 1
#define holharm_OUTPUT_L 2
#define holharm_OUTPUT_R 3
#define holharm_WET 4
#define holharm_VOLUME 5
#define holharm_PAN 6
#define holharm_INTERVALL 7
#define holharm_INTERVALR 8
#define holharm_TYPE 9
#define holharm_NOTE 10
#define holharm_CHORDTYPE 11
#define holharm_RECOCOUNT 12

static LADSPA_Descriptor *holharmLDescriptor = NULL;
static DSSI_Descriptor *holharmDDescriptor = NULL;

static void runholharm (LADSPA_Handle instance, unsigned long sample_count,
			snd_seq_event_t * events, unsigned long EventCount);

static void cleanupholharm (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return holharmLDescriptor;
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
      return holharmDDescriptor;
    default:
      return NULL;
    }
}

static void
cleanupholharm (LADSPA_Handle instance)
{
  holharm_t *harmonizer = (holharm_t *) instance;
  free (harmonizer);
}

static void
connectPortholharm (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  holharm_t *plugin = (holharm_t *) instance;
  switch (port)
    {
    case holharm_INPUT_L:
      plugin->inputl = data;
      break;
    case holharm_INPUT_R:
      plugin->inputr = data;
      break;
    case holharm_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case holharm_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case holharm_WET:
      plugin->wet = data;
      break;
    case holharm_VOLUME:
      plugin->outvolume = data;
      break;
    case holharm_PAN:
      plugin->panning = data;
      break;
    case holharm_INTERVALL:
      plugin->intervall = data;
      break;
    case holharm_INTERVALR:
      plugin->intervalr = data;
      break;
    case holharm_TYPE:
      plugin->PSELECT = data;
      break;
    case holharm_NOTE:
      plugin->Hnote = data;
      break;
    case holharm_CHORDTYPE:
      plugin->Ptype = data;
      break;
    case holharm_RECOCOUNT:
      plugin->recocount = data;
      break;  
      
    }
}

static char *
osc_build_path (char *base_path, const char *method)
{
  char buffer[256];
  char *full_path;

  snprintf (buffer, 256, "%s%s", base_path, method);
  if (!(full_path = strdup (buffer)))
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

  holharm_t *s = (holharm_t *) instance;
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
holharmConfigure (LADSPA_Handle instance, const char *key, const char *value)
{

//  holharm_t *harmonizer = (holharm_t *) instance;


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
holharm_get_program (LADSPA_Handle handle, unsigned long index)
{
  static DSSI_Program_Descriptor pd;

//  holharm_t *harmonizer = (holharm_t *) handle;


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
holharm_select_program (LADSPA_Handle handle, unsigned long bank,
			unsigned long program)
{

//  holharm_t *harmonizer = (holharm_t *) handle;

}

int
ReadConfig ()
{

  FILE *fs;
  int Quality;
  char temp[32];
  char filename[128];
  Quality = QUALITY;
  sprintf (filename, "/home/%s/.holharm", getenv("USER"));
  if ((fs = fopen (filename, "r")) != NULL)
    {
      bzero (temp, sizeof (temp));
      fgets (temp, sizeof temp, fs);
      sscanf (temp, "%d", &Quality);
      fclose (fs);
    }
  else
    {
      fs = fopen (filename, "w");
      bzero (temp, sizeof (temp));
      sprintf (temp, "%d\n", Quality);
      fputs (temp, fs);
      fclose (fs);
    }
  return (int)Quality;

}

static LADSPA_Handle
instantiateholharm (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{

  holharm_t *harmonizer = (holharm_t *) calloc (1, sizeof (holharm_t));
  
  harmonizer->hq = ReadConfig();
  RecChord_init (harmonizer);
  harmonizer->SAMPLE_RATE = s_rate;
  Recognize_init(harmonizer, harmonizer->efxoutl, harmonizer->efxoutr);
  PitchShifter_init (harmonizer, 2048, harmonizer->hq,
		     harmonizer->SAMPLE_RATE);
  pthread_mutex_init (&harmonizer->mutex, NULL);
  Harmonizer_start (harmonizer);
  return (LADSPA_Handle) harmonizer;
}

static void
activateholharm (LADSPA_Handle instance)
{

//  holharm_t *harmonizer = (holharm_t *) instance;

}


void
holharm_deactivate (LADSPA_Handle instance)
{
  holharm_t *harmonizer = (holharm_t *) instance;
  cleanup (harmonizer);
}


static void
runholharmWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runholharm (instance, sample_count, NULL, 0);
}

static void
runholharm (LADSPA_Handle instance, unsigned long sample_count,
	    snd_seq_event_t * events, unsigned long event_count)
{

  holharm_t *har = (holharm_t *) instance;
  LADSPA_Data *const outputl = har->efxoutl;
  LADSPA_Data *const outputr = har->efxoutr;
  LADSPA_Data *const pinputl = har->inputl;
  LADSPA_Data *const pinputr = har->inputr;
  LADSPA_Data wet = *(har->wet);
  LADSPA_Data vol = *(har->outvolume);
  LADSPA_Data pan = *(har->panning);
  LADSPA_Data interl = *(har->intervall);
  LADSPA_Data interr = *(har->intervalr);
  LADSPA_Data psel = *(har->PSELECT);
  LADSPA_Data Snote = *(har->Hnote);
  LADSPA_Data Ch_Type = *(har->Ptype);  
  LADSPA_Data recocount = *(har->recocount);

  unsigned long event_pos = 0;
  unsigned long pos;
  unsigned long count;
  float v1, v2;
  int i;
  memset (har->efxoutl, 0, sizeof (LADSPA_Data) * sample_count);
  memset (har->efxoutr, 0, sizeof (LADSPA_Data) * sample_count);
  
  if (psel == 1)
   {
     har->rc.fundi = Snote;
     har->rc.ctipo = Ch_Type;
   }
   
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
		      if (har->rc.note_active[i] == 0)
			{
			  har->rc.note_active[i] = 1;
			  har->rc.rnote[i] = events[event_pos].data.note.note;
			  har->rc.gate[i] = 1;
			  if (psel == 2)
			    {
			      MiraChord (har);
			      if (har->rc.cc)
				{
				  lo_send (har->m_host,
					   har->osc_configure_path, "ss",
					   "schord", har->rc.NombreAcorde);
				  har->rc.cc = 0;
				}

			    }
			  break;
			}

		    }
		  if (events[event_pos].data.note.velocity == 0)
		    {
		      if ((har->rc.note_active[i])
			  && (har->rc.rnote[i] ==
			      events[event_pos].data.note.note))
			{
			  har->rc.note_active[i] = 0;
			  har->rc.gate[i] = 0;
			  break;
			}

		    }
		}


	      break;
	    case SND_SEQ_EVENT_NOTEOFF:

	      for (i = 0; i < POLY; i++)
		{
		  if ((har->rc.note_active[i])
		      && (har->rc.rnote[i] ==
			  events[event_pos].data.note.note))
		    {
		      har->rc.note_active[i] = 0;
		      har->rc.gate[i] = 0;
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
		  interl = (int)
		    ((float) events[event_pos].data.control.value / 5.33 -
		     12);
		  break;
		}
	      if (events[event_pos].data.control.param == 49)
		{
		  interr = (int)
		    ((float) events[event_pos].data.control.value / 5.33 -
		     12);
		  break;
		}


	    }

	  event_pos++;
	}


      count =
	(sample_count - pos) > STEP_SIZE ? STEP_SIZE : sample_count - pos;
    
      if (((psel == 1) || (psel == 2)) && (har->val_sum > -24.0))
        {    
       	  schmittFloat (har, pinputl, pinputr, count);
	  if ((har->rc.reconota != -1) && (har->rc.reconota != har->last))
	    {
	      har->candidate = har->rc.reconota;
	      if (har->candidate != har->lastcandidate) 
               {
	       har->lcount = 0;
	       har->lastcandidate = har->candidate;
	       }
	       else har->lcount++; 
	      if (har->lcount > recocount)
		{
		  Vamos (har, 1, interl);
		  Vamos (har, 2, interr);
		  har->last = har->rc.reconota;
		  har->lcount = 0;
		}
		
	    }
	}



      out_smb (har, pinputl, pinputr, count);
      if (wet < 0.5)
	{
	  v1 = 1.0;
	  v2 = wet * 2.0;
	}
      else
	{
	  v1 = (1.0 - wet) * 2.0;
	  v2 = 1.0;
	};
      
      for (i = 0; i < count; i++)
	{
	  outputl[i] =(pinputl[i]*v2) + (har->outol[i] * pan * vol * v1);
	  outputr[i] =(pinputr[i]*v2) + (har->outor[i] * (1.0 - pan) * vol * v1);
	}


    }

}


int
getControllerholharm (LADSPA_Handle instance, unsigned long port)
{

  switch ((int)port)
    {
    case holharm_VOLUME:
      return DSSI_CC(7);
    case holharm_PAN:
      return DSSI_CC(10);
    case holharm_INTERVALL:
      return DSSI_CC(48);
    case holharm_INTERVALR:
      return DSSI_CC(49);
    }

  return -1;
}


void __attribute__ ((constructor))
holharm_init()
{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  holharmLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (holharmLDescriptor)
    {
      holharmLDescriptor->UniqueID = 3982;
      holharmLDescriptor->Label = "holharm";
      holharmLDescriptor->Properties = 0;
      holharmLDescriptor->Name = "holharm";
      holharmLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      holharmLDescriptor->Copyright = "GNU General Public License version 2";
      holharmLDescriptor->PortCount = 13;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (holharmLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      holharmLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (holharmLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      holharmLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (holharmLDescriptor->PortCount, sizeof (char *));
      holharmLDescriptor->PortNames = (const char **) port_names;
      /* Parameters for Input L */
      port_descriptors[holharm_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_INPUT_L] = "Input L";
      port_range_hints[holharm_INPUT_L].HintDescriptor = 0;
      /* Parameters for Input R */
      port_descriptors[holharm_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_INPUT_R] = "Input R";
      port_range_hints[holharm_INPUT_R].HintDescriptor = 0;
      /* Parameters for Output L */
      port_descriptors[holharm_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_OUTPUT_L] = "Output L";
      port_range_hints[holharm_OUTPUT_L].HintDescriptor = 0;
      /* Parameters for Output R */
      port_descriptors[holharm_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_OUTPUT_R] = "Output R";
      port_range_hints[holharm_OUTPUT_R].HintDescriptor = 0;
      /* Parameters for Wet */
      port_descriptors[holharm_WET] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_WET] = "Wet/Dry";
      port_range_hints[holharm_WET].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_WET].LowerBound = 0.0;
      port_range_hints[holharm_WET].UpperBound = 1.0;
      /* Parameters for Volume */
      port_descriptors[holharm_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_VOLUME] = "Volume";
      port_range_hints[holharm_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_VOLUME].LowerBound = 0.0;
      port_range_hints[holharm_VOLUME].UpperBound = 1.0;
      /* Parameters for Pan */
      port_descriptors[holharm_PAN] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_PAN] = "Pan";
      port_range_hints[holharm_PAN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[holharm_PAN].LowerBound = 0.0;
      port_range_hints[holharm_PAN].UpperBound = 1.0;
      /* Parameters for Interval L */
      port_descriptors[holharm_INTERVALL] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_INTERVALL] = "Interval L";
      port_range_hints[holharm_INTERVALL].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_INTEGER;
      port_range_hints[holharm_INTERVALL].LowerBound = -12;
      port_range_hints[holharm_INTERVALL].UpperBound = 12;
      /* Parameters for Interval R */
      port_descriptors[holharm_INTERVALR] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_INTERVALR] = "Interval R";
      port_range_hints[holharm_INTERVALR].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_INTEGER;
      port_range_hints[holharm_INTERVALR].LowerBound = -12;
      port_range_hints[holharm_INTERVALR].UpperBound = 12;
      /* Parameters for Type */
      port_descriptors[holharm_TYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_TYPE] = "Type";
      port_range_hints[holharm_TYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_TYPE].LowerBound = 0;
      port_range_hints[holharm_TYPE].UpperBound = 2;
      /* Parameters for Note */
      port_descriptors[holharm_NOTE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_NOTE] = "Note";
      port_range_hints[holharm_NOTE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_NOTE].LowerBound = 0;
      port_range_hints[holharm_NOTE].UpperBound = 11;
      /* Parameters for TypeChord */
      port_descriptors[holharm_CHORDTYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_CHORDTYPE] = "Chord Type";
      port_range_hints[holharm_CHORDTYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_CHORDTYPE].LowerBound = 0;
      port_range_hints[holharm_CHORDTYPE].UpperBound = 33;
      /* Parameters for RecoCount */
      port_descriptors[holharm_RECOCOUNT] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_RECOCOUNT] = "Recognize Times";
      port_range_hints[holharm_RECOCOUNT].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;	
      port_range_hints[holharm_RECOCOUNT].LowerBound = 0;
      port_range_hints[holharm_RECOCOUNT].UpperBound = 48;



      holharmLDescriptor->instantiate = instantiateholharm;
      holharmLDescriptor->connect_port = connectPortholharm;
      holharmLDescriptor->activate = activateholharm;
      holharmLDescriptor->run = runholharmWrapper;
      holharmLDescriptor->run_adding = NULL;
      holharmLDescriptor->set_run_adding_gain = NULL;
      holharmLDescriptor->deactivate = holharm_deactivate;
      holharmLDescriptor->cleanup = cleanupholharm;
    }



  holharmDDescriptor = (DSSI_Descriptor *) malloc (sizeof (DSSI_Descriptor));
  if (holharmDDescriptor)
    {
      holharmDDescriptor->DSSI_API_Version = 1;
      holharmDDescriptor->LADSPA_Plugin = holharmLDescriptor;
      holharmDDescriptor->configure = holharmConfigure;
      holharmDDescriptor->get_program = holharm_get_program;
      holharmDDescriptor->get_midi_controller_for_port = getControllerholharm;
      holharmDDescriptor->select_program = holharm_select_program;
      holharmDDescriptor->run_synth = runholharm;
      holharmDDescriptor->run_synth_adding = NULL;
      holharmDDescriptor->run_multiple_synths = NULL;
      holharmDDescriptor->run_multiple_synths_adding = NULL;
    }
}


void __attribute__ ((destructor))
holharm_fini()
{
  if (holharmLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) holharmLDescriptor->PortDescriptors);
      free ((char **) holharmLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) holharmLDescriptor->PortRangeHints);
      free (holharmLDescriptor);
    }
  if (holharmDDescriptor)
    {
      free (holharmDDescriptor);
    }
}
