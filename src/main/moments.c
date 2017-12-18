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

/**
 * Function giving the moment of a block of an image.
 *
 * @param im the image to analyse.
 * @param p the number of the block, starting at 0.
 * @param n the number of blocks per line.
 * @param m the number of blocks per colone.
 * @parma M0 the adresse of the moment 0.
 * @parma M1 the moment 1 array.
 * @parma M2 the moment 2 array.
 */
extern void give_moments(image im, int p, int n, int m, int * M0, double * M1, double * M2){

  assert(p>=0 && p<n*m); // Si le block n'existe pas on sort de la fonction.

  int i, j, k;
  int * pixel;

  int hBlock = image_give_hauteur(im)/m;
  int lBlock = image_give_largeur(im)/n;
  int dim = image_give_dim(im);

  *M0 = 0;
  for (k=0; k<dim; k++) {
    M1[k] = 0;
    M2[k] = 0;
  }

  Point point;
  point.coordx = (p%n)*lBlock;
  point.coordy = (p/n)*hBlock;

  image_move_to(im,&point);

  for (i=0; i<hBlock; i++) {
    for (j=0; j<lBlock; j++) {
      pixel = image_lire_pixel(im);
      *M0+=1;
      for (k=0; k<dim; k++) {
        M1[k] += pixel[k];
        M2[k] += pixel[k]*pixel[k];
      }
      image_pixel_suivant(im);
    }
    MOVE_DOWN_POINT(point);
    image_move_to(im,&point);
  }
}
