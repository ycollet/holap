/*
  musicaldelay.c - ZynAddSubFx Delay meassured in musical values LADSPA plugin software

  musicaldelay.c  -  Main LADSPA file of the plugin
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

rdff19 MusicalDelay_DRY 4
rdff19 MusicalDelay_WET 5
rdff19 MusicalDelay_TEMPO 6
rdff19 MusicalDelay_PAN1 7
rdff19 MusicalDelay_PAN2 8
rdff19 MusicalDelay_DELAY1 9
rdff19 MusicalDelay_DELAY2 10
rdff19 MusicalDelay_DELAY3 11
rdff19 MusicalDelay_GAIN1 12
rdff19 MusicalDelay_GAIN2 13
rdff19 MusicalDelay_FB1 14
rdff19 MusicalDelay_FB2 15
rdff19 MusicalDelay_DAMP 16
rdff19 MusicalDelay_LRCROSS 17



*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "mdelay.h"


#define MusicalDelay_INPUT_L 0
#define MusicalDelay_INPUT_R 1
#define MusicalDelay_OUTPUT_L 2
#define MusicalDelay_OUTPUT_R 3
#define MusicalDelay_DRY 4
#define MusicalDelay_WET 5
#define MusicalDelay_TEMPO 6
#define MusicalDelay_PAN1 7
#define MusicalDelay_PAN2 8
#define MusicalDelay_DELAY1 9
#define MusicalDelay_DELAY2 10
#define MusicalDelay_DELAY3 11
#define MusicalDelay_GAIN1 12
#define MusicalDelay_GAIN2 13
#define MusicalDelay_FB1 14
#define MusicalDelay_FB2 15
#define MusicalDelay_DAMP 16
#define MusicalDelay_LRCROSS 17

static LADSPA_Descriptor *MusicalDelayLDescriptor = NULL;

static void runMusicalDelay (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupMusicalDelay (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return MusicalDelayLDescriptor;
    default:
      return NULL;
    }
}



static void
cleanupMusicalDelay (LADSPA_Handle instance)
{
  MDelay_t *mdelay = (MDelay_t *) instance;
  free (mdelay);
}

static void
connectPortMusicalDelay (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  MDelay_t *plugin = (MDelay_t *) instance;
  switch (port)
    {
    case MusicalDelay_INPUT_L:
      plugin->inputl = data;
      break;
    case MusicalDelay_INPUT_R:
      plugin->inputr = data;
      break;
    case MusicalDelay_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case MusicalDelay_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case MusicalDelay_DRY:
      plugin->dry = data;
      break;
    case MusicalDelay_WET:
      plugin->outvolume = data;
      break;
    case MusicalDelay_TEMPO:
      plugin->Ptempo = data;
      break;
    case MusicalDelay_PAN1:
      plugin->Ppanning1 = data;
      break;
    case MusicalDelay_PAN2:
      plugin->Ppanning2 = data;
      break;
    case MusicalDelay_DELAY1:
      plugin->Pdelay1 = data;
      break;
    case MusicalDelay_DELAY2:
      plugin->Pdelay2 = data;
      break;
    case MusicalDelay_DELAY3:
      plugin->Plrdelay = data;
      break;
    case MusicalDelay_GAIN1:
      plugin->gain1 = data;
      break;
    case MusicalDelay_GAIN2:
      plugin->gain2 = data;
      break;
    case MusicalDelay_FB1:
      plugin->Pfb1 = data;
      break;
    case MusicalDelay_FB2:
      plugin->Pfb2 = data;
      break;
    case MusicalDelay_DAMP:
      plugin->Phidamp = data;
      break;
    case MusicalDelay_LRCROSS:
      plugin->Plrcross = data;
      break;      
    }
}



static LADSPA_Handle
instantiateMusicalDelay (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  MDelay_t *mdelay = (MDelay_t *) calloc (1, sizeof (MDelay_t));
  
  if (mdelay)
  {
  mdelay->SAMPLE_RATE = s_rate;
  MusicDelay_Init(mdelay);
  
  }

  return (LADSPA_Handle) mdelay;
}

static void
activateMusicalDelay (LADSPA_Handle instance)
{

//  MDelay_t *mdelay = (MDelay_t *) instance;


}


void
MusicalDelay_deactivate (LADSPA_Handle instance)
{

//  MDelay_t *mdelay = (MDelay_t *) instance;

}


static void
runMusicalDelayWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runMusicalDelay (instance, sample_count);

}

static void
runMusicalDelay (LADSPA_Handle instance, unsigned long sample_count)
{

  MDelay_t *mdelay = (MDelay_t *) instance;

  LADSPA_Data *const outputl = mdelay->efxoutl;
  LADSPA_Data *const outputr = mdelay->efxoutr;
  LADSPA_Data *const pinputl = mdelay->inputl;
  LADSPA_Data *const pinputr = mdelay->inputr;
  LADSPA_Data dry = *(mdelay->dry);

  int i;


       for (i=0; i<sample_count; i++)
         {
           mdelay->indatal[i]=pinputl[i];
           mdelay->indatar[i]=pinputr[i];
         }  

         out(mdelay, pinputl,pinputr,sample_count);

          for (i=0; i<sample_count; i++)
           
        {
          outputl[i] +=mdelay->indatal[i]*dry;
          outputr[i] +=mdelay->indatar[i]*dry;
        }
  


}


void __attribute__((constructor))
musicaldelay_init()
{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  MusicalDelayLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (MusicalDelayLDescriptor)
    {
      MusicalDelayLDescriptor->UniqueID = 3989;
      MusicalDelayLDescriptor->Label = "MusicalDelay";
      MusicalDelayLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      MusicalDelayLDescriptor->Name = "MusicalDelay";
      MusicalDelayLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      MusicalDelayLDescriptor->Copyright = "GNU General Public License version 2";
      MusicalDelayLDescriptor->PortCount = 18;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (MusicalDelayLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      MusicalDelayLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (MusicalDelayLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      MusicalDelayLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (MusicalDelayLDescriptor->PortCount, sizeof (char *));
      MusicalDelayLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[MusicalDelay_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[MusicalDelay_INPUT_L] = "Input L";
      port_range_hints[MusicalDelay_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[MusicalDelay_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[MusicalDelay_INPUT_R] = "Input R";
      port_range_hints[MusicalDelay_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[MusicalDelay_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[MusicalDelay_OUTPUT_L] = "Output L";
      port_range_hints[MusicalDelay_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[MusicalDelay_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[MusicalDelay_OUTPUT_R] = "Output R";
      port_range_hints[MusicalDelay_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Dry */
      port_descriptors[MusicalDelay_DRY] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_DRY] = "Dry";
      port_range_hints[MusicalDelay_DRY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE; 
      port_range_hints[MusicalDelay_DRY].LowerBound = 0.0;
      port_range_hints[MusicalDelay_DRY].UpperBound = 1.0;

      /* Parameters for Wet */
      port_descriptors[MusicalDelay_WET] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_WET] = "Wet";
      port_range_hints[MusicalDelay_WET].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_WET].LowerBound = 0.0;
      port_range_hints[MusicalDelay_WET].UpperBound = 1.0;

      /* Parameters for Tempo */
      port_descriptors[MusicalDelay_TEMPO] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_TEMPO] = "Tempo";
      port_range_hints[MusicalDelay_TEMPO].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[MusicalDelay_TEMPO].LowerBound = 15.0;
      port_range_hints[MusicalDelay_TEMPO].UpperBound = 240.0;

      /* Parameters for Pan 1 */
      port_descriptors[MusicalDelay_PAN1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_PAN1] = "Pan 1";
      port_range_hints[MusicalDelay_PAN1].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[MusicalDelay_PAN1].LowerBound = 0.0;
      port_range_hints[MusicalDelay_PAN1].UpperBound = 1.0;

      /* Parameters for Pan 2 */
      port_descriptors[MusicalDelay_PAN2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_PAN2] = "Pan 2";
      port_range_hints[MusicalDelay_PAN2].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[MusicalDelay_PAN2].LowerBound = 0.0;
      port_range_hints[MusicalDelay_PAN2].UpperBound = 1.0;

      /* Parameters for Delay 1 */
      port_descriptors[MusicalDelay_DELAY1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_DELAY1] = "Subdivision 1";
      port_range_hints[MusicalDelay_DELAY1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_DELAY1].LowerBound = 0;
      port_range_hints[MusicalDelay_DELAY1].UpperBound = 7;

      /* Parameters for Delay 2 */
      port_descriptors[MusicalDelay_DELAY2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_DELAY2] = "Subdivision 2";
      port_range_hints[MusicalDelay_DELAY2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_DELAY2].LowerBound = 0;
      port_range_hints[MusicalDelay_DELAY2].UpperBound = 7;

      /* Parameters for Delay 3 */
      port_descriptors[MusicalDelay_DELAY3] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_DELAY3] = "Sub Between 1 2";
      port_range_hints[MusicalDelay_DELAY3].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_DELAY3].LowerBound = 0;
      port_range_hints[MusicalDelay_DELAY3].UpperBound = 7;


      /* Parameters for Gain 1 */
      port_descriptors[MusicalDelay_GAIN1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_GAIN1] = "Gain 1";
      port_range_hints[MusicalDelay_GAIN1].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_GAIN1].LowerBound = 0.0;
      port_range_hints[MusicalDelay_GAIN1].UpperBound = 1.0;

      /* Parameters for Gain 2 */
      port_descriptors[MusicalDelay_GAIN2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_GAIN2] = "Gain 2";
      port_range_hints[MusicalDelay_GAIN2].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_GAIN2].LowerBound = 0.0;
      port_range_hints[MusicalDelay_GAIN2].UpperBound = 1.0;

      /* Parameters for FB 1 */
      port_descriptors[MusicalDelay_FB1] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_FB1] = "Feedback 1";
      port_range_hints[MusicalDelay_FB1].HintDescriptor =
      LADSPA_HINT_DEFAULT_MIDDLE |
      LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_FB1].LowerBound = 0.0;
      port_range_hints[MusicalDelay_FB1].UpperBound = 1.0;

      /* Parameters for FB 2 */
      port_descriptors[MusicalDelay_FB2] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_FB2] = "Feedback 2";
      port_range_hints[MusicalDelay_FB2].HintDescriptor =
      LADSPA_HINT_DEFAULT_MIDDLE |
      LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_FB2].LowerBound = 0.0;
      port_range_hints[MusicalDelay_FB2].UpperBound = 1.0;
      
      /* Parameters for DAMP */
      port_descriptors[MusicalDelay_DAMP] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_DAMP] = "Damp";
      port_range_hints[MusicalDelay_DAMP].HintDescriptor =
      LADSPA_HINT_DEFAULT_MIDDLE |
      LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_DAMP].LowerBound = 0.0;
      port_range_hints[MusicalDelay_DAMP].UpperBound = 1.0;

      /* Parameters for LR cross */
      port_descriptors[MusicalDelay_LRCROSS] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[MusicalDelay_LRCROSS] = "LR Cross";
      port_range_hints[MusicalDelay_LRCROSS].HintDescriptor =
      LADSPA_HINT_DEFAULT_MIDDLE |
      LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[MusicalDelay_LRCROSS].LowerBound = 0.0;
      port_range_hints[MusicalDelay_LRCROSS].UpperBound = 1.0;

      
      MusicalDelayLDescriptor->instantiate = instantiateMusicalDelay;
      MusicalDelayLDescriptor->connect_port = connectPortMusicalDelay;
      MusicalDelayLDescriptor->activate = activateMusicalDelay;
      MusicalDelayLDescriptor->run = runMusicalDelayWrapper;
      MusicalDelayLDescriptor->run_adding = NULL;
      MusicalDelayLDescriptor->set_run_adding_gain = NULL;
      MusicalDelayLDescriptor->deactivate = MusicalDelay_deactivate;
      MusicalDelayLDescriptor->cleanup = cleanupMusicalDelay;
    }



}


void __attribute__((destructor))
musicaldelay_fini()
{
  if (MusicalDelayLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) MusicalDelayLDescriptor->PortDescriptors);
      free ((char **) MusicalDelayLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) MusicalDelayLDescriptor->PortRangeHints);
      free (MusicalDelayLDescriptor);
    }
}



