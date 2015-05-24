/*
   holharm - a inteligent harmonizer DSSI plugin software                                          
   
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


#include "harmonizer.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <math.h>

void
RecChord_init (holharm_t * s)
{
  int i;

  s->rc.reconota = -1;

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


}

void
Vamos (holharm_t * s, int canal, int interval)
{

  int n1, h1;
  int hm1, hp1;
  int hm2, hp2;
  int hm3, hp3;
  int ninterval = interval;
  int tengo;
  int nota;
  int harmo;
  int typo;

  nota = s->rc.reconota % 12;

  nota -= s->rc.fundi;
  if (nota < 0)
    nota += 12;

 

  harmo = (12 + nota + interval) % 12;
  if (harmo > 12)
    harmo %= 12;

  hm1 = harmo - 1;
  if (hm1 < 0)
    hm1 += 12;
  hp1 = harmo + 1;
  if (hp1 > 12)
    hp1 %= 12;

  hm2 = harmo - 2;
  if (hm2 < 0)
    hm2 += 12;
  hp2 = harmo + 2;
  if (hp2 > 12)
    hp2 %= 12;

  hm3 = harmo - 3;
  if (hm3 < 0)
    hm3 += 12;
  hp3 = harmo + 3;
  if (hp3 > 12)
    hp3 %= 12;



  n1 = s->rc.Ch[s->rc.ctipo][nota];
  h1 = s->rc.Ch[s->rc.ctipo][harmo];

  typo = 3;

  if ((n1 == 1) && (h1 == 1))
    typo = 0;
  if ((n1 == 2) && (h1 == 2))
    typo = 1;
  if (h1 == 1)
    typo = 2;



  switch (typo)

    {
    case 0:
      ninterval = interval;
      break;
    case 1:
      ninterval = interval;
      break;
    case 2:
      ninterval = interval;
      break;
    case 3:
      tengo = 0;

      if ((s->rc.Ch[s->rc.ctipo][hp1] != 0) && (tengo == 0))
	{
	  ninterval = interval + 1;
	  tengo = 1;
	}
      if ((s->rc.Ch[s->rc.ctipo][hm1] != 0) && (tengo == 0))
	{
	  ninterval = interval - 1;
	  tengo = 1;
	}

      if ((s->rc.Ch[s->rc.ctipo][hp2] != 0) && (tengo == 0))
	{
	  ninterval = interval + 2;
	  tengo = 1;
	}
      if ((s->rc.Ch[s->rc.ctipo][hm2] != 0) && (tengo == 0))
	{
	  ninterval = interval - 2;
	  tengo = 1;
	}

      if ((s->rc.Ch[s->rc.ctipo][hp3] != 0) && (tengo == 0))
	{
	  ninterval = interval + 3;
	  tengo = 1;
	}
      if ((s->rc.Ch[s->rc.ctipo][hm3] != 0) && (tengo == 0))
	{
	  ninterval = interval - 1;
	  tengo = 1;
	}
      break;
    }

  if (ninterval < -12)
    ninterval += 12;
  if (ninterval > 12)
    ninterval %= 12;





  switch (canal)
    {

    case 1:
      s->ratiol = powf (2, (float) (ninterval / 12.0));
      break;
    case 2:
      s->ratior = powf (2, (float) (ninterval / 12.0));
      break;
    }



}
