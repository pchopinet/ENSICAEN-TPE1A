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
 * @version 0.0.1 / 2017-12-29
 */

/**
 * @file merge.c
 */

#include <merge.h>
#include <RAG.h>

void perform_merge(RAG* rag,long double error_threshold){
  int b1;
  int b2;
  int nb_merge = 0;
  long double error = get_error(rag);
  printf("Erreur : %Lf\n", error);
  while (error>error_threshold){
    RAG_give_closest_region(rag,&b1,&b2);
    printf("Blocks : %d %d\n",b1,b2);
    RAG_merge_region(rag,b1,b2);
    error = get_error(rag);
    nb_merge++;
  }
  printf("%d\n",nb_merge );
  RAG_give_closest_region(rag,&b1,&b2);
}
