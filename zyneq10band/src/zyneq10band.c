/*
  zyneq10band.c - ZynAddSubFx Reverb LADSPA plugin software

  zyneq10ban.c  -  Main LADSPA file of the plugin
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

rdff19 ZynEq10ban_VOLUME 4
rdff19 ZynEq10ban_Q 5
rdff19 ZynEq10ban_GAIN1 6
rdff19 ZynEq10ban_GAIN2 7
rdff19 ZynEq10ban_GAIN3 8
rdff19 ZynEq10ban_GAIN4 9
rdff19 ZynEq10ban_GAIN5 10
rdff19 ZynEq10ban_GAIN6 11
rdff19 ZynEq10ban_GAIN7 12
rdff19 ZynEq10ban_GAIN8 13
rdff19 ZynEq10ban_GAIN9 14
rdff19 ZynEq10ban_GAIN10 15



*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "eq10.h"


#define ZynEq10ban_INPUT_L 0
#define ZynEq10ban_INPUT_R 1
#define ZynEq10ban_OUTPUT_L 2
#define ZynEq10ban_OUTPUT_R 3
#define ZynEq10ban_VOLUME 4
#define ZynEq10ban_Q 5
#define ZynEq10ban_GAIN1 6
#define ZynEq10ban_GAIN2 7
#define ZynEq10ban_GAIN3 8
#define ZynEq10ban_GAIN4 9
#define ZynEq10ban_GAIN5 10
#define ZynEq10ban_GAIN6 11
#define ZynEq10ban_GAIN7 12
#define ZynEq10ban_GAIN8 13
#define ZynEq10ban_GAIN9 14
#define ZynEq10ban_GAIN10 15


static LADSPA_Descriptor *ZynEq10banLDescriptor = NULL;

static void runZynEq10ban (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupZynEq10ban (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return ZynEq10banLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupZynEq10ban (LADSPA_Handle instance)
{
  ZEq10ban_t *eq10 = (ZEq10ban_t *) instance;
  free (eq10);
}

static void
connectPortZynEq10ban (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  ZEq10ban_t *plugin = (ZEq10ban_t *) instance;
  switch (port)
    {
    case ZynEq10ban_INPUT_L:
      plugin->inputl = data;
      break;
    case ZynEq10ban_INPUT_R:
      plugin->inputr = data;
      break;
    case ZynEq10ban_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case ZynEq10ban_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case ZynEq10ban_VOLUME:
      plugin->outvolume = data;
      break;
    case ZynEq10ban_Q:
      plugin->q = data;
      break;
    case ZynEq10ban_GAIN1:
      plugin->gain1 = data;
      break;    
    case ZynEq10ban_GAIN2:
      plugin->gain2 = data;
      break;
    case ZynEq10ban_GAIN3:
      plugin->gain3 = data;
      break;
    case ZynEq10ban_GAIN4:
      plugin->gain4 = data;
      break;
    case ZynEq10ban_GAIN5:
      plugin->gain5 = data;
      break;
    case ZynEq10ban_GAIN6:
      plugin->gain6 = data;
      break;
    case ZynEq10ban_GAIN7:
      plugin->gain7 = data;
      break;
    case ZynEq10ban_GAIN8:
      plugin->gain8 = data;
      break;
    case ZynEq10ban_GAIN9:
      plugin->gain9 = data;
      break;
    case ZynEq10ban_GAIN10:
      plugin->gain10 = data;
      break;

    }
}



static LADSPA_Handle
instantiateZynEq10ban (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  ZEq10ban_t *eq10 = (ZEq10ban_t *) calloc (1, sizeof (ZEq10ban_t));
  
  if (eq10)
  {
  eq10->SAMPLE_RATE = s_rate;
  EQ_Init (eq10);
  }

  return (LADSPA_Handle) eq10;
}

static void
activateZynEq10ban (LADSPA_Handle instance)
{

//  ZEq10ban_t *eq10 = (ZEq10ban_t *) instance;


}


void
ZynEq10ban_deactivate (LADSPA_Handle instance)
{

//  ZEq10ban_t *eq10 = (ZEq10ban_t *) instance;


}


static void
runZynEq10banWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runZynEq10ban (instance, sample_count);

}

static void
runZynEq10ban (LADSPA_Handle instance, unsigned long sample_count)
{

  ZEq10ban_t *eq10 = (ZEq10ban_t *) instance;

  LADSPA_Data *const pinputl = eq10->inputl;
  LADSPA_Data *const pinputr = eq10->inputr;


  out(eq10, pinputl,pinputr,sample_count);

}


void __attribute((constructor))
zyneq10ban_init()
{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  ZynEq10banLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (ZynEq10banLDescriptor)
    {
      ZynEq10banLDescriptor->UniqueID = 3988;
      ZynEq10banLDescriptor->Label = "ZynEq10ban";
      ZynEq10banLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      ZynEq10banLDescriptor->Name = "ZynEq 10 Parametric Bands";
      ZynEq10banLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      ZynEq10banLDescriptor->Copyright = "GNU General Public License version 2";
      ZynEq10banLDescriptor->PortCount = 16;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (ZynEq10banLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      ZynEq10banLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (ZynEq10banLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      ZynEq10banLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (ZynEq10banLDescriptor->PortCount, sizeof (char *));
      ZynEq10banLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[ZynEq10ban_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq10ban_INPUT_L] = "Input L";
      port_range_hints[ZynEq10ban_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[ZynEq10ban_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq10ban_INPUT_R] = "Input R";
      port_range_hints[ZynEq10ban_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[ZynEq10ban_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq10ban_OUTPUT_L] = "Output L";
      port_range_hints[ZynEq10ban_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[ZynEq10ban_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynEq10ban_OUTPUT_R] = "Output R";
      port_range_hints[ZynEq10ban_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Volume */
      port_descriptors[ZynEq10ban_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_VOLUME] = "Volume";
      port_range_hints[ZynEq10ban_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynEq10ban_VOLUME].LowerBound = 0.0;
      port_range_hints[ZynEq10ban_VOLUME].UpperBound = 1.0;

      /* Parameters for Q */
      port_descriptors[ZynEq10ban_Q] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_Q] = "Q";
      port_range_hints[ZynEq10ban_Q].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_Q].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_Q].UpperBound = 1.0;

      /* Parameters for Gain1 */
      port_descriptors[ZynEq10ban_GAIN1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN1] = "31 Hz";
      port_range_hints[ZynEq10ban_GAIN1].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN1].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN1].UpperBound = 1.0;

      /* Parameters for Gain2 */
      port_descriptors[ZynEq10ban_GAIN2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN2] = "63 Hz";
      port_range_hints[ZynEq10ban_GAIN2].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN2].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN2].UpperBound = 1.0;

      /* Parameters for Gain3 */
      port_descriptors[ZynEq10ban_GAIN3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN3] = "125 Hz";
      port_range_hints[ZynEq10ban_GAIN3].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN3].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN3].UpperBound = 1.0;

      /* Parameters for Gain4 */
      port_descriptors[ZynEq10ban_GAIN4] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN4] = "250 Hz";
      port_range_hints[ZynEq10ban_GAIN4].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN4].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN4].UpperBound = 1.0;

      /* Parameters for Gain5 */
      port_descriptors[ZynEq10ban_GAIN5] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN5] = "500 Hz";
      port_range_hints[ZynEq10ban_GAIN5].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN5].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN5].UpperBound = 1.0;

      /* Parameters for Gain6 */
      port_descriptors[ZynEq10ban_GAIN6] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN6] = "1 KHz";
      port_range_hints[ZynEq10ban_GAIN6].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN6].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN6].UpperBound = 1.0;

      /* Parameters for Gain7 */
      port_descriptors[ZynEq10ban_GAIN7] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN7] = "2 KHz";
      port_range_hints[ZynEq10ban_GAIN7].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN7].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN7].UpperBound = 1.0;

      /* Parameters for Gain8 */
      port_descriptors[ZynEq10ban_GAIN8] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN8] = "4 KHz";
      port_range_hints[ZynEq10ban_GAIN8].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN8].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN8].UpperBound = 1.0;

      /* Parameters for Gain9 */
      port_descriptors[ZynEq10ban_GAIN9] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN9] = "8 KHz";
      port_range_hints[ZynEq10ban_GAIN9].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN9].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN9].UpperBound = 1.0;

      /* Parameters for Gain10 */
      port_descriptors[ZynEq10ban_GAIN10] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynEq10ban_GAIN10] = "16 KHz";
      port_range_hints[ZynEq10ban_GAIN10].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynEq10ban_GAIN10].LowerBound = -1.0;
      port_range_hints[ZynEq10ban_GAIN10].UpperBound = 1.0;



      ZynEq10banLDescriptor->instantiate = instantiateZynEq10ban;
      ZynEq10banLDescriptor->connect_port = connectPortZynEq10ban;
      ZynEq10banLDescriptor->activate = activateZynEq10ban;
      ZynEq10banLDescriptor->run = runZynEq10banWrapper;
      ZynEq10banLDescriptor->run_adding = NULL;
      ZynEq10banLDescriptor->set_run_adding_gain = NULL;
      ZynEq10banLDescriptor->deactivate = ZynEq10ban_deactivate;
      ZynEq10banLDescriptor->cleanup = cleanupZynEq10ban;
    }



}


void __attribute__ ((destructor))
zyneq10ban_fini ()
{
  if (ZynEq10banLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) ZynEq10banLDescriptor->PortDescriptors);
      free ((char **) ZynEq10banLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) ZynEq10banLDescriptor->PortRangeHints);
      free (ZynEq10banLDescriptor);
    }
}


