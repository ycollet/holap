
/*
  goomf - a FM mono synth plugin software

  main_ui.h  -  Variable Definitions and functions
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
 (version2)  along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/


#ifndef _MAIN_UI_H
#define _MAIN_UI_H

#include <lo/lo.h>
#include "../config.h"



typedef struct
{
  char Name[64];
  float master_volume;
  float tune;
  int algo;
  float portamento;
  float LFO_Volume;
  float LFO_Frequency;
  float LFO_Delay;
  int LFO_Wave;
  int wave[6];
  int H[6];
  float HF[6];
  float Ovol[6];
  float attack[6];
  float decay[6];
  float sustain[6];
  float release[6];
  float pLFO[6];
  int Ftype;
  float Fgain;
  float Fcutoff;
  float Fq;
  float FLFO;
  int FADSR;
  int Fstages;
  int Fvelocity;

} Todolo;

char Name[64];
float master_volume;
float tune;
int algo;
float portamento;
float LFO_Volume;
float LFO_Frequency;
float LFO_Delay;
int LFO_Wave;
int wave[6];
int H[6];
float HF[6];
float Ovol[6];
float attack[6];
float decay[6];
float sustain[6];
float release[6];
float pLFO[6];
int Ftype;
float Fgain;
float Fcutoff;
float Fq;
float FLFO;
int FADSR;
int Fstages;
int Fvelocity;


char BankFilename[128];


Todolo Banco[80];

char laristra[5300];

goomf_ui gui;
char elpath[256];
lo_server osc_server;
lo_address m_host;
char *osc_configure_path;
char *osc_control_path;
char *osc_exiting_path;
char *osc_hide_path;
char *osc_midi_path;
char *osc_program_path;
char *osc_quit_path;
char *osc_show_path;
char *osc_update_path;
char *myurl;
int funcion;

pthread_t thr1;

void read_stdin (void);
void update_widgets (int port, float value);
void New ();
void New_Bank ();
int loadbank (const char *filename);
int savebank (const char *filename);
void Put_Combi (int i);
void Update_Operator_Widgets (int op);
void Update_Main_Widgets ();
void Send_Values ();
void preset_to_bank (int j);
void make_window_banks ();
void Put_Loaded_Bank ();
void Haz_laristra ();
void Send_laristra ();
void set_FilterType (int type);
#endif
