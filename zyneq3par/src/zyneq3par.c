/*
  zyneq3par.c - ZynAddSubFx Reverb LADSPA plugin software

  zyneq3par.c  -  Main LADSPA file of the plugin
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

rdff19 ZynEq3par_LOWFREQ 5
rdff19 ZynEq3par_LOWGAIN 6
rdff19 ZynEq3par_LOWQ 7
rdff19 ZynEq3par_MIDFREQ 8
rdff19 ZynEq3par_MIDGAIN 9
rdff19 ZynEq3par_MIDQ 10
rdff19 ZynEq3par_HIGHFREQ 11
rdff19 ZynEq3par_HIGHGAIN 12
rdff19 ZynEq3par_HIGHQ 13


*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "eq.h"


#define ZynEq3par_INPUT_L 0
#define ZynEq3par_INPUT_R 1
#define ZynEq3par_OUTPUT_L 2
#define ZynEq3par_OUTPUT_R 3
#define ZynEq3par_VOLUME 4
#define ZynEq3par_LOWFREQ 5
#define ZynEq3par_LOWGAIN 6
#define ZynEq3par_LOWQ 7
#define ZynEq3par_MIDFREQ 8
#define ZynEq3par_MIDGAIN 9
#define ZynEq3par_MIDQ 10
#define ZynEq3par_HIGHFREQ 11
#define ZynEq3par_HIGHGAIN 12
#define ZynEq3par_HIGHQ 13


static LADSPA_Descriptor *ZynEq3parLDescriptor = NULL;

static void runZynEq3par (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupZynEq3par (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return ZynEq3parLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupZynEq3par (LADSPA_Handle instance)
{
  ZEq3par_t *eq3 = (ZEq3par_t *) instance;
  free (eq3);
}

static void
connectPortZynEq3par (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  ZEq3par_t *plugin = (ZEq3par_t *) instance;
  switch (port)
    {
    case ZynEq3par_INPUT_L:
      plugin->inputl = data;
      break;
    case ZynEq3par_INPUT_R:
      plugin->inputr = data;
      break;
    case ZynEq3par_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case ZynEq3par_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case ZynEq3par_VOLUME:
      plugin->outvolume = data;
      break;
    case ZynEq3par_LOWFREQ:
      plugin->lowfreq = data;
      break;
    case ZynEq3par_LOWGAIN:
      plugin->lowgain = data;
      break;    
    case ZynEq3par_LOWQ:
      plugin->lowq = data;
      break;
    case ZynEq3par_MIDFREQ:
      plugin->midfreq = data;
      break;
    case ZynEq3par_MIDGAIN:
      plugin->midgain = data;
      break;
    case ZynEq3par_MIDQ:
      plugin->midq = data;
      break;
    case ZynEq3par_HIGHFREQ:
      plugin->highfreq = data;
      break;
    case ZynEq3par_HIGHGAIN:
      plugin->highgain = data;
      break;
    case ZynEq3par_HIGHQ:
      plugin->highq = data;
      break;

    }
}



static LADSPA_Handle
instantiateZynEq3par (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  ZEq3par_t *eq3 = (ZEq3par_t *) calloc (1, sizeof (ZEq3par_t));
  
  if (eq3)
  {
  eq3->SAMPLE_RATE = s_rate;
  EQ_Init (eq3);
  }

  return (LADSPA_Handle) eq3;
}

static void
activateZynEq3par (LADSPA_Handle instance)
{

//  ZEq3par_t *eq3 = (ZEq3par_t *) instance;


}


void
ZynEq3par_deactivate (LADSPA_Handle instance)
{

//  ZEq3par_t *eq3 = (ZEq3par_t *) instance;


}


static void
runZynEq3parWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runZynEq3par (instance, sample_count);

}

static void
runZynEq3par (LADSPA_Handle instance, unsigned long sample_count)
{

  ZEq3par_t *eq3 = (ZEq3par_t *) instance;

  LADSPA_Data *const pinputl = eq3->inputl;
  LADSPA_Data *const pinputr = eq3->inputr;


  out(eq3, pinputl,pinputr,sample_count);

}


void __attribute__ ((constructor))
zyneq3par_init ()


{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  ZynEq3parLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (ZynEq3parLDescriptor)
    {
      ZynEq3parLDescriptor->UniqueID = 3987;
      ZynEq3parLDescriptor->Label = "ZynEq3par";
      ZynEq3parLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      ZynEq3parLDescriptor->Name = "ZynEq 3 Parametric Band";
      ZynEq3parLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      ZynEq3parLDescriptor->Copyright = "GNU General Public License version 2";
      ZynEq3parLDescriptor->PortCount = 14;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (ZynEq3parLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      ZynEq3parLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (ZynEq3parLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      ZynEq3parLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (ZynEq3parLDescriptor->PortCount, sizeof (char *));
      ZynEq3parLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[ZynEq3par_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq3par_INPUT_L] = "Input L";
      port_range_hints[ZynEq3par_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[ZynEq3par_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq3par_INPUT_R] = "Input R";
      port_range_hints[ZynEq3par_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[ZynEq3par_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq3par_OUTPUT_L] = "Output L";
      port_range_hints[ZynEq3par_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[ZynEq3par_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq3par_OUTPUT_R] = "Output R";
      port_range_hints[ZynEq3par_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Volume */
      port_descriptors[ZynEq3par_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_VOLUME] = "Volume";
      port_range_hints[ZynEq3par_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_VOLUME].LowerBound = 0.0;
      port_range_hints[ZynEq3par_VOLUME].UpperBound = 1.0;

      /* Parameters for Low Freq */
      port_descriptors[ZynEq3par_LOWFREQ] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_LOWFREQ] = "Low Frequency";
      port_range_hints[ZynEq3par_LOWFREQ].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq3par_LOWFREQ].LowerBound = 20.0;
      port_range_hints[ZynEq3par_LOWFREQ].UpperBound = 500.0;

      /* Parameters for Low Gain */
      port_descriptors[ZynEq3par_LOWGAIN] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_LOWGAIN] = "Low Gain";
      port_range_hints[ZynEq3par_LOWGAIN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq3par_LOWGAIN].LowerBound = -1.0;
      port_range_hints[ZynEq3par_LOWGAIN].UpperBound = 1.0;

      /* Parameters for Low q */
      port_descriptors[ZynEq3par_LOWQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_LOWQ] = "Low Q";
      port_range_hints[ZynEq3par_LOWQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_LOWQ].LowerBound = -1.0;
      port_range_hints[ZynEq3par_LOWQ].UpperBound = 1.0;

      /* Parameters for Mid Freq */
      port_descriptors[ZynEq3par_MIDFREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_MIDFREQ] = "Mid Frequency";
      port_range_hints[ZynEq3par_MIDFREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_MIDFREQ].LowerBound = 300.0;
      port_range_hints[ZynEq3par_MIDFREQ].UpperBound = 3000.0;

      /* Parameters for Mid Gain */
      port_descriptors[ZynEq3par_MIDGAIN] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_MIDGAIN] = "Mid Gain";
      port_range_hints[ZynEq3par_MIDGAIN].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_MIDGAIN].LowerBound = -1.0;
      port_range_hints[ZynEq3par_MIDGAIN].UpperBound = 1.0;

      /* Parameters for Mid q */
      port_descriptors[ZynEq3par_MIDQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_MIDQ] = "Mid Q";
      port_range_hints[ZynEq3par_MIDQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | 
	 LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_MIDQ].LowerBound = -1.0;
      port_range_hints[ZynEq3par_MIDQ].UpperBound = 1.0;

      /* Parameters for High Frequency */
      port_descriptors[ZynEq3par_HIGHFREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_HIGHFREQ] = "High Frequency";
      port_range_hints[ZynEq3par_HIGHFREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_HIGHFREQ].LowerBound = 2000.0;
      port_range_hints[ZynEq3par_HIGHFREQ].UpperBound = 10020.0;

      /* Parameters for High Gain */
      port_descriptors[ZynEq3par_HIGHGAIN] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_HIGHGAIN] = "High Gain";
      port_range_hints[ZynEq3par_HIGHGAIN].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_HIGHGAIN].LowerBound = -1.0;
      port_range_hints[ZynEq3par_HIGHGAIN].UpperBound = 1.0;

      /* Parameters for High q */
      port_descriptors[ZynEq3par_HIGHQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq3par_HIGHQ] = "High Q";
      port_range_hints[ZynEq3par_HIGHQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq3par_HIGHQ].LowerBound = -1.0;
      port_range_hints[ZynEq3par_HIGHQ].UpperBound = 1.0;


      ZynEq3parLDescriptor->instantiate = instantiateZynEq3par;
      ZynEq3parLDescriptor->connect_port = connectPortZynEq3par;
      ZynEq3parLDescriptor->activate = activateZynEq3par;
      ZynEq3parLDescriptor->run = runZynEq3parWrapper;
      ZynEq3parLDescriptor->run_adding = NULL;
      ZynEq3parLDescriptor->set_run_adding_gain = NULL;
      ZynEq3parLDescriptor->deactivate = ZynEq3par_deactivate;
      ZynEq3parLDescriptor->cleanup = cleanupZynEq3par;
    }



}

void __attribute__ ((destructor))
zyneq3par_fini ()

{
  if (ZynEq3parLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) ZynEq3parLDescriptor->PortDescriptors);
      free ((char **) ZynEq3parLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) ZynEq3parLDescriptor->PortRangeHints);
      free (ZynEq3parLDescriptor);
    }
}



