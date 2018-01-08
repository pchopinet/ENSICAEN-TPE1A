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
 * @author Clement Labonne <clement.labonne@ecole.ensicaen.fr>
 * @version 0.0.1 / 2017-12-16
 */

/**
 * @file testMoments.c
 */
#include <image.h>
#include <moments.h>

int main(){
  image im=FAIRE_image();
  image_charger(im,"../../img/lenna.ppm");

  int i;
  int n = 2;
  int m = 2;
  int M0 = 0;
  double M1[3] = {0,0,0};
  double M2[3] = {0,0,0};
  for (i=0; i<n*m; i++) {
    give_moments(im,i,n,m,&M0,M1,M2);
    printf("%d :\nM0 : %d\n",i,M0);
    printf("M1 : %f\n",M1[0]);
    printf("M2 : %f\n\n",M2[0]);
  }
}
