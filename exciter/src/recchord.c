/*
   exciter - a MIDI helped exciter DSSI plugin software                                          
   
   recchord.c  -  Recognize Chord functions.
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


#include "exciter.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <math.h>


void
RecChord_Init (exciter_t * s)
{
  int i;

  bzero (s->rc.Ch, sizeof (s->rc.Ch));

  s->rc.Ch[0][0] = 1;
  s->rc.Ch[6][0] = 1;
  s->rc.Ch[12][0] = 1;
  s->rc.Ch[18][0] = 1;
  s->rc.Ch[24][0] = 1;
  s->rc.Ch[31][0] = 1;
  s->rc.Ch[0][2] = 2;
  s->rc.Ch[6][2] = 1;
  s->rc.Ch[12][2] = 1;
  s->rc.Ch[18][2] = 2;
  s->rc.Ch[24][2] = 2;
  s->rc.Ch[31][7] = 1;
  s->rc.Ch[0][4] = 1;
  s->rc.Ch[6][4] = 1;
  s->rc.Ch[12][3] = 1;
  s->rc.Ch[18][3] = 1;
  s->rc.Ch[24][4] = 1;
  s->rc.Ch[0][7] = 1;
  s->rc.Ch[6][7] = 1;
  s->rc.Ch[12][5] = 2;
  s->rc.Ch[18][5] = 2;
  s->rc.Ch[24][7] = 1;
  s->rc.Ch[0][9] = 2;
  s->rc.Ch[6][9] = 1;
  s->rc.Ch[12][7] = 1;
  s->rc.Ch[18][6] = 1;
  s->rc.Ch[24][9] = 1;
  s->rc.Ch[30][0] = 1;
  s->rc.Ch[6][11] = 2;
  s->rc.Ch[18][8] = 2;
  s->rc.Ch[24][10] = 1;
  s->rc.Ch[18][9] = 1;
  s->rc.Ch[18][11] = 2;
  s->rc.Ch[1][0] = 1;
  s->rc.Ch[7][0] = 1;
  s->rc.Ch[13][0] = 1;
  s->rc.Ch[19][0] = 1;
  s->rc.Ch[25][0] = 1;
  s->rc.Ch[32][0] = 1;
  s->rc.Ch[1][2] = 2;
  s->rc.Ch[7][2] = 2;
  s->rc.Ch[13][2] = 1;
  s->rc.Ch[19][4] = 1;
  s->rc.Ch[25][1] = 1;
  s->rc.Ch[32][2] = 2;
  s->rc.Ch[1][4] = 1;
  s->rc.Ch[7][4] = 1;
  s->rc.Ch[13][3] = 1;
  s->rc.Ch[19][7] = 1;
  s->rc.Ch[25][4] = 1;
  s->rc.Ch[32][5] = 1;
  s->rc.Ch[1][7] = 1;
  s->rc.Ch[7][6] = 2;
  s->rc.Ch[13][5] = 2;
  s->rc.Ch[19][10] = 1;
  s->rc.Ch[25][7] = 1;
  s->rc.Ch[32][7] = 1;
  s->rc.Ch[1][9] = 1;
  s->rc.Ch[7][8] = 1;
  s->rc.Ch[13][7] = 1;
  s->rc.Ch[25][10] = 1;
  s->rc.Ch[1][11] = 2;
  s->rc.Ch[7][10] = 2;
  s->rc.Ch[13][10] = 1;
  s->rc.Ch[2][0] = 1;
  s->rc.Ch[8][0] = 1;
  s->rc.Ch[14][0] = 1;
  s->rc.Ch[20][0] = 1;
  s->rc.Ch[26][0] = 1;
  s->rc.Ch[33][0] = 1;
  s->rc.Ch[2][2] = 2;
  s->rc.Ch[8][2] = 2;
  s->rc.Ch[14][2] = 2;
  s->rc.Ch[20][5] = 1;
  s->rc.Ch[26][1] = 2;
  s->rc.Ch[33][2] = 1;
  s->rc.Ch[2][4] = 1;
  s->rc.Ch[8][3] = 1;
  s->rc.Ch[14][3] = 1;
  s->rc.Ch[20][7] = 1;
  s->rc.Ch[26][4] = 1;
  s->rc.Ch[33][4] = 2;
  s->rc.Ch[2][7] = 1;
  s->rc.Ch[8][5] = 2;
  s->rc.Ch[14][5] = 1;
  s->rc.Ch[20][10] = 1;
  s->rc.Ch[26][5] = 2;
  s->rc.Ch[33][7] = 1;
  s->rc.Ch[2][9] = 2;
  s->rc.Ch[8][7] = 1;
  s->rc.Ch[14][7] = 1;
  s->rc.Ch[26][7] = 1;
  s->rc.Ch[33][9] = 2;
  s->rc.Ch[2][11] = 1;
  s->rc.Ch[14][10] = 1;
  s->rc.Ch[26][8] = 1;
  s->rc.Ch[26][10] = 1;
  s->rc.Ch[3][0] = 1;
  s->rc.Ch[9][0] = 1;
  s->rc.Ch[15][0] = 1;
  s->rc.Ch[21][0] = 1;
  s->rc.Ch[27][0] = 1;
  s->rc.Ch[3][2] = 2;
  s->rc.Ch[9][2] = 2;
  s->rc.Ch[15][2] = 2;
  s->rc.Ch[21][2] = 2;
  s->rc.Ch[27][1] = 2;
  s->rc.Ch[3][4] = 1;
  s->rc.Ch[9][3] = 1;
  s->rc.Ch[15][3] = 1;
  s->rc.Ch[21][4] = 1;
  s->rc.Ch[27][3] = 1;
  s->rc.Ch[3][6] = 1;
  s->rc.Ch[9][5] = 2;
  s->rc.Ch[15][5] = 2;
  s->rc.Ch[21][6] = 1;
  s->rc.Ch[27][4] = 1;
  s->rc.Ch[3][7] = 1;
  s->rc.Ch[9][7] = 1;
  s->rc.Ch[15][7] = 1;
  s->rc.Ch[21][10] = 1;
  s->rc.Ch[27][6] = 2;
  s->rc.Ch[3][9] = 2;
  s->rc.Ch[9][9] = 1;
  s->rc.Ch[15][11] = 1;
  s->rc.Ch[27][7] = 1;
  s->rc.Ch[3][11] = 1;
  s->rc.Ch[27][9] = 2;
  s->rc.Ch[27][10] = 1;
  s->rc.Ch[4][0] = 1;
  s->rc.Ch[10][0] = 1;
  s->rc.Ch[16][0] = 1;
  s->rc.Ch[22][0] = 1;
  s->rc.Ch[28][0] = 1;
  s->rc.Ch[4][2] = 1;
  s->rc.Ch[10][3] = 1;
  s->rc.Ch[16][2] = 1;
  s->rc.Ch[22][2] = 1;
  s->rc.Ch[28][2] = 2;
  s->rc.Ch[4][4] = 1;
  s->rc.Ch[10][5] = 2;
  s->rc.Ch[16][3] = 1;
  s->rc.Ch[22][4] = 1;
  s->rc.Ch[28][4] = 1;
  s->rc.Ch[4][7] = 1;
  s->rc.Ch[10][7] = 1;
  s->rc.Ch[16][5] = 2;
  s->rc.Ch[22][7] = 1;
  s->rc.Ch[28][5] = 2;
  s->rc.Ch[4][9] = 2;
  s->rc.Ch[10][10] = 1;
  s->rc.Ch[16][7] = 1;
  s->rc.Ch[22][9] = 2;
  s->rc.Ch[28][8] = 1;
  s->rc.Ch[16][11] = 1;
  s->rc.Ch[22][10] = 1;
  s->rc.Ch[28][9] = 2;
  s->rc.Ch[28][11] = 1;
  s->rc.Ch[5][0] = 1;
  s->rc.Ch[11][0] = 1;
  s->rc.Ch[17][0] = 1;
  s->rc.Ch[23][0] = 1;
  s->rc.Ch[29][0] = 1;
  s->rc.Ch[5][2] = 1;
  s->rc.Ch[11][1] = 2;
  s->rc.Ch[17][3] = 1;
  s->rc.Ch[23][2] = 2;
  s->rc.Ch[29][2] = 2;
  s->rc.Ch[5][4] = 1;
  s->rc.Ch[11][3] = 1;
  s->rc.Ch[17][5] = 2;
  s->rc.Ch[23][4] = 1;
  s->rc.Ch[29][4] = 1;
  s->rc.Ch[5][7] = 1;
  s->rc.Ch[11][5] = 2;
  s->rc.Ch[17][6] = 1;
  s->rc.Ch[23][6] = 1;
  s->rc.Ch[29][6] = 2;
  s->rc.Ch[5][9] = 2;
  s->rc.Ch[11][6] = 1;
  s->rc.Ch[17][8] = 2;
  s->rc.Ch[23][7] = 1;
  s->rc.Ch[29][8] = 1;
  s->rc.Ch[5][11] = 1;
  s->rc.Ch[11][8] = 2;
  s->rc.Ch[23][9] = 2;
  s->rc.Ch[29][10] = 1;
  s->rc.Ch[11][10] = 1;
  s->rc.Ch[23][10] = 1;



  bzero (s->rc.ChN, sizeof (s->rc.ChN));


  static const char *stnom[] =
    { "", "6", "Maj7", "lyd", "Maj(9)", "Maj7(9)", "6/9", "+", "m",
    "m6", "m7", "m7(b5)", "m9", "m7(9)", "m7(11)", "m(Maj7)",
    "m(Maj7)(9)", "dim", "dim7", "7", "7(Sus4)", "7(b5)", "7(9)",
    "7(#11)", "7(13)", "7(b9)", "7(b13)", "7(#9)", "+Maj7", "+7",
    "1+8", "1+5", "(Sus4)", "2"
  };
  for (i = 0; i <= 33; i++)
    {
      strcpy (s->rc.ChN[i].Nom, stnom[i]);
      s->rc.ChN[i].tipo = i;
    }


  s->rc.ChN[1].num = 1;
  s->rc.ChN[6].num = 1;
  s->rc.ChN[9].num = 1;
  s->rc.ChN[32].num = 1;


  {
    static const char *stnom[] =
      { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb",
      "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };

    for (i = 0; i <= 11; i++)
      strcpy (s->rc.NC[i].Nom, stnom[i]);
    for (i = 0; i <= 23; i++)
      strcpy (s->rc.NCE[i].Nom, stnom[i]);
  }

  s->rc.NCE[0].note = 0;
  s->rc.NCE[1].note = 1;
  s->rc.NCE[2].note = 2;
  s->rc.NCE[3].note = 3;
  s->rc.NCE[4].note = 4;
  s->rc.NCE[5].note = 5;
  s->rc.NCE[6].note = 6;
  s->rc.NCE[7].note = -5;
  s->rc.NCE[8].note = -4;
  s->rc.NCE[9].note = -3;
  s->rc.NCE[10].note = -2;
  s->rc.NCE[11].note = -1;
  s->rc.NCE[12].note = 0;
  s->rc.NCE[13].note = 1;
  s->rc.NCE[14].note = 2;
  s->rc.NCE[15].note = 3;
  s->rc.NCE[16].note = 4;
  s->rc.NCE[17].note = 5;
  s->rc.NCE[18].note = 6;
  s->rc.NCE[19].note = -5;
  s->rc.NCE[20].note = -4;
  s->rc.NCE[21].note = -3;
  s->rc.NCE[22].note = -2;
  s->rc.NCE[23].note = -1;


  IniciaChords (s);
  bzero (s->rc.NombreAcorde, sizeof (s->rc.NombreAcorde));
  RecChord_Cleanup(s);


}


void
RecChord_Cleanup (exciter_t * s)
{
  int i;
  bzero (s->rc.NombreAcorde, sizeof (s->rc.NombreAcorde));
  for (i = 0; i < POLY; i++)
    {
      s->rc.note_active[i] = 0;
      s->rc.rnote[i] = 0;
      s->rc.gate[i] = 0;
    }

  s->rc.cc = 1;
  s->rc.ctipo = 34;


}

void
IniciaChords (exciter_t * s)
{
  int i, j;
  int notas = 1;
  int numno[6];

  s->rc.NumChord3 = 0;
  s->rc.NumChord4 = 0;
  s->rc.NumChord5 = 0;

  bzero (s->rc.Chord3, sizeof (s->rc.Chord3));
  bzero (s->rc.Chord4, sizeof (s->rc.Chord4));
  bzero (s->rc.Chord5, sizeof (s->rc.Chord5));

  for (i = 0; i <= 33; i++)
    {
      notas = 1;
      bzero (numno, sizeof (numno));

      for (j = 1; j <= 11; j++)
	{
	  if (s->rc.Ch[i][j] == 1)
	    {
	      notas++;
	      numno[notas] = j;
	    }
	}



      switch (notas)
	{
	case 3:
	  strcpy (s->rc.Chord3[s->rc.NumChord3].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord3[s->rc.NumChord3].di1 = numno[2];
	  s->rc.Chord3[s->rc.NumChord3].di2 = numno[3] - numno[2];
	  s->rc.Chord3[s->rc.NumChord3].fund = 1;
	  s->rc.Chord3[s->rc.NumChord3].tipo = i;
	  s->rc.NumChord3++;
	  if (s->rc.ChN[i].num == 1)
	    break;
	  strcpy (s->rc.Chord3[s->rc.NumChord3].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord3[s->rc.NumChord3].di1 = numno[3] - numno[2];
	  s->rc.Chord3[s->rc.NumChord3].di2 = 12 - numno[3];
	  s->rc.Chord3[s->rc.NumChord3].fund = 3;
	  s->rc.Chord3[s->rc.NumChord3].tipo = i;
	  s->rc.NumChord3++;
	  strcpy (s->rc.Chord3[s->rc.NumChord3].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord3[s->rc.NumChord3].di1 = 12 - numno[3];
	  s->rc.Chord3[s->rc.NumChord3].di2 = numno[2];
	  s->rc.Chord3[s->rc.NumChord3].fund = 2;
	  s->rc.Chord3[s->rc.NumChord3].tipo = i;
	  s->rc.NumChord3++;
	  break;
	case 4:
	  strcpy (s->rc.Chord4[s->rc.NumChord4].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord4[s->rc.NumChord4].di1 = numno[2];
	  s->rc.Chord4[s->rc.NumChord4].di2 = numno[3] - numno[2];
	  s->rc.Chord4[s->rc.NumChord4].di3 = numno[4] - numno[3];
	  s->rc.Chord4[s->rc.NumChord4].fund = 1;
	  s->rc.Chord4[s->rc.NumChord4].tipo = i;
	  s->rc.NumChord4++;
	  if (s->rc.ChN[i].num == 1)
	    break;
	  strcpy (s->rc.Chord4[s->rc.NumChord4].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord4[s->rc.NumChord4].di1 = numno[3] - numno[2];
	  s->rc.Chord4[s->rc.NumChord4].di2 = numno[4] - numno[3];
	  s->rc.Chord4[s->rc.NumChord4].di3 = 12 - numno[4];
	  s->rc.Chord4[s->rc.NumChord4].fund = 4;
	  s->rc.Chord4[s->rc.NumChord4].tipo = i;
	  s->rc.NumChord4++;
	  strcpy (s->rc.Chord4[s->rc.NumChord4].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord4[s->rc.NumChord4].di1 = numno[4] - numno[3];
	  s->rc.Chord4[s->rc.NumChord4].di2 = 12 - numno[4];
	  s->rc.Chord4[s->rc.NumChord4].di3 = numno[2];
	  s->rc.Chord4[s->rc.NumChord4].fund = 3;
	  s->rc.Chord4[s->rc.NumChord4].tipo = i;
	  s->rc.NumChord4++;
	  strcpy (s->rc.Chord4[s->rc.NumChord4].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord4[s->rc.NumChord4].di1 = 12 - numno[4];
	  s->rc.Chord4[s->rc.NumChord4].di2 = numno[2];
	  s->rc.Chord4[s->rc.NumChord4].di3 = numno[3] - numno[2];
	  s->rc.Chord4[s->rc.NumChord4].fund = 2;
	  s->rc.Chord4[s->rc.NumChord4].tipo = i;
	  s->rc.NumChord4++;
	  strcpy (s->rc.Chord4[s->rc.NumChord4].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord4[s->rc.NumChord4].di1 = numno[2] + (12 - numno[4]);
	  s->rc.Chord4[s->rc.NumChord4].di2 = numno[3] - numno[2];
	  s->rc.Chord4[s->rc.NumChord4].di3 = 12 - numno[3];
	  s->rc.Chord4[s->rc.NumChord4].fund = 4;
	  s->rc.Chord4[s->rc.NumChord4].tipo = i;
	  s->rc.NumChord4++;
	  break;

	case 5:
	  strcpy (s->rc.Chord5[s->rc.NumChord5].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord5[s->rc.NumChord5].di1 = numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di2 = numno[3] - numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di3 = numno[4] - numno[3];
	  s->rc.Chord5[s->rc.NumChord5].di4 = numno[5] - numno[4];
	  s->rc.Chord5[s->rc.NumChord5].fund = 1;
	  s->rc.Chord5[s->rc.NumChord5].tipo = i;
	  s->rc.NumChord5++;
	  if (s->rc.ChN[i].num == 1)
	    break;
	  strcpy (s->rc.Chord5[s->rc.NumChord5].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord5[s->rc.NumChord5].di1 = numno[3] - numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di2 = numno[4] - numno[3];
	  s->rc.Chord5[s->rc.NumChord5].di3 = numno[5] - numno[4];
	  s->rc.Chord5[s->rc.NumChord5].di4 = 12 - numno[5];

	  s->rc.Chord5[s->rc.NumChord5].fund = 5;
	  s->rc.Chord5[s->rc.NumChord5].tipo = i;
	  s->rc.NumChord5++;
	  strcpy (s->rc.Chord5[s->rc.NumChord5].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord5[s->rc.NumChord5].di1 = numno[4] - numno[3];
	  s->rc.Chord5[s->rc.NumChord5].di2 = numno[5] - numno[4];
	  s->rc.Chord5[s->rc.NumChord5].di3 = 12 - numno[5];
	  s->rc.Chord5[s->rc.NumChord5].di4 = numno[2];
	  s->rc.Chord5[s->rc.NumChord5].fund = 4;
	  s->rc.Chord5[s->rc.NumChord5].tipo = i;
	  s->rc.NumChord5++;
	  strcpy (s->rc.Chord5[s->rc.NumChord5].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord5[s->rc.NumChord5].di1 = numno[5] - numno[4];
	  s->rc.Chord5[s->rc.NumChord5].di2 = 12 - numno[5];
	  s->rc.Chord5[s->rc.NumChord5].di3 = numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di4 = numno[3] - numno[2];
	  s->rc.Chord5[s->rc.NumChord5].fund = 3;
	  s->rc.Chord5[s->rc.NumChord5].tipo = i;
	  s->rc.NumChord5++;
	  strcpy (s->rc.Chord5[s->rc.NumChord5].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord5[s->rc.NumChord5].di1 = 12 - numno[5];
	  s->rc.Chord5[s->rc.NumChord5].di2 = numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di3 = numno[3] - numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di4 = numno[4] - numno[3];
	  s->rc.Chord5[s->rc.NumChord5].fund = 2;
	  s->rc.Chord5[s->rc.NumChord5].tipo = i;
	  s->rc.NumChord5++;
	  strcpy (s->rc.Chord5[s->rc.NumChord5].Nom, s->rc.ChN[i].Nom);
	  s->rc.Chord5[s->rc.NumChord5].di1 = numno[2] + (12 - numno[5]);
	  s->rc.Chord5[s->rc.NumChord5].di2 = numno[3] - numno[2];
	  s->rc.Chord5[s->rc.NumChord5].di3 = numno[4] - numno[3];
	  s->rc.Chord5[s->rc.NumChord5].di4 = 12 - numno[4];
	  s->rc.Chord5[s->rc.NumChord5].fund = 5;
	  s->rc.Chord5[s->rc.NumChord5].tipo = i;
	  s->rc.NumChord5++;
	  break;

	}

    }





};



void
MiraChord (exciter_t * s)
{

  int i, j;
  int anote[POLY];
  int nnotes = 0;
  char AName[20];
  int temp;
  int di1, di2, di3, di4;
  int tienebass = 0;
  int labaja;

  for (i = 0; i < POLY; i++)
    {
      if (s->rc.note_active[i])
	{
	  anote[nnotes] = s->rc.rnote[i];
	  nnotes++;
	}
    }



  if ((nnotes < 3) || (nnotes > 5))
    return;

  labaja = anote[0];
  for (i = 1; i < nnotes - 1; i++)
    if (anote[i] < labaja)
      labaja = anote[i];
  for (i = 0; i < nnotes; i++)
    if (anote[i] - labaja > 12)
      anote[i] -= 12;
  for (i = 1; i < nnotes; i++)
    {
      for (j = 0; j < nnotes - 1; j++)
	{

	  if (anote[j] > anote[j + 1])
	    {
	      temp = anote[j];
	      anote[j] = anote[j + 1];
	      anote[j + 1] = temp;
	    }
	}
    }



  s->rc.bass = anote[0] % 12;

sigue:

  if (nnotes == 3)
    {

      di1 = anote[1] - anote[0];
      di2 = anote[2] - anote[1];

      j = -1;

      while (j <= s->rc.NumChord3)

	{
	  j++;
	  if ((s->rc.Chord3[j].di1 == di1) && (s->rc.Chord3[j].di2 == di2))
	    {
	      s->rc.ctipo = s->rc.Chord3[j].tipo;
	      int elke = anote[s->rc.Chord3[j].fund - 1];
	      s->rc.fundi = elke % 12;
	      sprintf (AName, "%s%s", s->rc.NC[s->rc.fundi].Nom,
		       s->rc.Chord3[j].Nom);

	      if (s->rc.bass != s->rc.fundi)
		{

		  bzero (AName, sizeof (AName));
		  sprintf (AName, "%s%s/%s", s->rc.NC[s->rc.fundi].Nom,
			   s->rc.Chord3[j].Nom,
			   s->rc.NCE[s->rc.bass + plus (s, s->rc.fundi)].Nom);
		}

	      if (strcmp (AName, s->rc.NombreAcorde) != 0)
		{
		  strcpy (s->rc.NombreAcorde, AName);
		  s->rc.cc = 1;
		}
	      return;
	    }
	}

    }

  if (nnotes == 4)
    {

      di1 = anote[1] - anote[0];
      di2 = anote[2] - anote[1];
      di3 = anote[3] - anote[2];
      j = -1;

      while (j <= s->rc.NumChord4)

	{
	  j++;
	  if ((s->rc.Chord4[j].di1 == di1) && (s->rc.Chord4[j].di2 == di2)
	      && (s->rc.Chord4[j].di3 == di3))
	    {
	      s->rc.ctipo = s->rc.Chord4[j].tipo;
	      int elke = anote[s->rc.Chord4[j].fund - 1];
	      s->rc.fundi = elke % 12;
	      sprintf (AName, "%s%s", s->rc.NC[s->rc.fundi].Nom,
		       s->rc.Chord4[j].Nom);
	      if (s->rc.bass != s->rc.fundi)
		{
		  bzero (AName, sizeof (AName));
		  sprintf (AName, "%s%s/%s", s->rc.NC[s->rc.fundi].Nom,
			   s->rc.Chord4[j].Nom,
			   s->rc.NCE[s->rc.bass + plus (s, s->rc.fundi)].Nom);
		}

	      if (strcmp (AName, s->rc.NombreAcorde) != 0)
		{
		  strcpy (s->rc.NombreAcorde, AName);
		  s->rc.cc = 1;
		}
	      return;
	    }

	}



    }

  if (nnotes == 5)
    {

      di1 = anote[1] - anote[0];
      di2 = anote[2] - anote[1];
      di3 = anote[3] - anote[2];
      di4 = anote[4] - anote[3];
      j = -1;

      while (j < s->rc.NumChord5)

	{
	  j++;
	  if ((s->rc.Chord5[j].di1 == di1) && (s->rc.Chord5[j].di2 == di2)
	      && (s->rc.Chord5[j].di3 == di3) && (s->rc.Chord5[j].di4 == di4))
	    {
	      s->rc.ctipo = s->rc.Chord5[j].tipo;
	      int elke = anote[s->rc.Chord5[j].fund - 1];
	      s->rc.fundi = elke % 12;
	      sprintf (AName, "%s%s", s->rc.NC[s->rc.fundi].Nom,
		       s->rc.Chord5[j].Nom);
	      if (s->rc.bass != s->rc.fundi)
		{
		  bzero (AName, sizeof (AName));
		  sprintf (AName, "%s%s/%s", s->rc.NC[s->rc.fundi].Nom,
			   s->rc.Chord5[j].Nom,
			   s->rc.NCE[s->rc.bass + plus (s, s->rc.fundi)].Nom);
		}

	      if (strcmp (AName, s->rc.NombreAcorde) != 0)
		{
		  strcpy (s->rc.NombreAcorde, AName);
		  s->rc.cc = 1;
		}
	      return;

	    }

	}

    }



  if (nnotes == 5)
    {
      tienebass = 1;
      s->rc.bass = anote[0] % 12;

      for (i = 1; i <= 4; i++)
	anote[i - 1] = anote[i];
      nnotes = 4;
      goto sigue;
    }



  if (nnotes == 5)
    {
      nnotes = 4;
      goto sigue;
    }

  if (nnotes == 4)
    {
      nnotes = 3;
      goto sigue;
    }



};




int
plus (exciter_t * s, int nota)
{
  switch (nota)
    {
    case 2:
      return (12);
      break;
    case 4:
      return (12);
      break;
    case 7:
      return (12);
      break;
    case 9:
      return (12);
      break;
    case 11:
      return (12);
      break;
    default:
      return (0);
    }

};
