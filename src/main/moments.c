/* -*- c-basic-offset: 3 -*-
*
* ENSICAEN
* 6 Boulevard Marechal Juin
* F-14050 Caen Cedex
*
* This file is owned by ENSICAEN students.
* No portion of this document may be reproduced, copied
* or revised without written permission of the authors.
*/

/**
* @author Pierre Chopinet <pierre.chopinet@ecole.ensicaen.fr>
* @version 0.0.1 / 2017-12-16
*/

/**
* @file moments.c
*/

#include <moments.h>

void give_moments(image * im, int p, int n, int m, int * M0, double * M1, double * M2){
  int i, j, k;

  //int* pixels = im->debut;
  int pixel;
  int hBlock = im->hauteur/m;
  int lBlock = im->largeur/n;

  int x = (p%n)*lBlock;
  int y = (p/n)*hBlock;

  for (i=0; i<hBlock; i++) {
    for (j=0; j<lBlock;j++) {
      M0+=1;
      for (k=0; k<im->dim; k++) {
        pixel = im->debut[im->dim*((y+i)*hBlock+x+j)+k];
        M1 += pixel;
        M2 += pixel*pixel;
      }
    }
  }
}
