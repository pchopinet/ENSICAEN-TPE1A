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
* @version 0.0.1 / 2017-12-29
*/

/**
* @file merge.c
*/

#include <merge.h>
#include <RAG.h>


/**
* Function merging regions until the error reach a threshold.
*
* @param rag is a RAG structure pointeur.
* @param error_threshold is the threshold the error have to reach to stop the mergers.
*/
void perform_merge(RAG* rag,double error_threshold){
  int b1, b1_old = -1;
  int b2, b2_old = -1;
  int nb_merge = 0;
  long double error = get_error(rag);
  long double error_limit = error * (long double)error_threshold;
  while (error<error_limit){
    RAG_give_closest_region(rag,&b1,&b2);
    if (b1==b1_old && b2 == b2_old) {
      break;
    }
    RAG_merge_region(rag,b1,b2);
    error = get_error(rag);
    nb_merge++;
    b1_old = b1;
    b2_old = b2;
  }
  printf("Number of merge : %d\n",nb_merge );
  RAG_give_closest_region(rag,&b1,&b2);
  RAG_normalize_parents(rag);
}

/**
* Function creating an image with the regions and their average color.
*
* @param rag is a RAG structure pointeur.
* @param filename is the adress of the new image.
*
* @return im the new image.
*/
image create_output_image(RAG * rag, char * filename){
  image old = get_image(rag);
  int dim = image_give_dim(old);
  int nb_blocks = get_nb_blocks(rag);
  int n = get_n(rag);
  int m = get_m(rag);
  int hBlock = image_give_hauteur(old)/m;
  int lBlock = image_give_largeur(old)/n;
  int k,j,i;

  int * temp = malloc(sizeof(int)*dim);

  image im = FAIRE_image();
  image_initialize(im,dim,image_give_largeur(old), image_give_hauteur(old));

  Point point;

  image_move_to(im,&point);

  for(k=0;k<nb_blocks;k++){
    point.coordx = (k%n)*lBlock;
    point.coordy = (k/n)*hBlock;
    image_move_to(im,&point);
    RAG_give_mean_color(rag,k,temp);
    for(i=0;i<hBlock;i++){
      for(j=0;j<lBlock;j++){
        image_ecrire_pixel(im,temp);
        image_pixel_suivant(im);
      }
      MOVE_DOWN_POINT(point);
      image_move_to(im,&point);
    }
  }
  free(temp);
  image_sauvegarder(im, filename);
  return im;
}
