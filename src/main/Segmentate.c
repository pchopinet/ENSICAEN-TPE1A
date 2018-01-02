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
 * @version 0.0.1 / 2017-12-17
 */

/**
 * @file testRAG.c
 */
#include <stdlib.h>
#include <merge.h>
#include <RAG.h>

int main(int argc, char const *argv[]) {
  int i;
  double threshold = 1.01;
  printf("%d\n",argc );
	if (argc < 5) {
		printf("Usage: %s <filename> <filename_output> <Width_nb_of_block> <Height_nb_of_block> [<threshold> : 1.01]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
  if(argc==6){
    threshold = atof(argv[5]);
  }

  int n = atoi(argv[3]);
  int m = atoi(argv[4]);

  for (i = 0; i < argc; i++) {
		printf("argv[%d] : %s\n", i, argv[i]);
	}

  image im = FAIRE_image();
  image_charger(im,argv[1]);
  RAG* rag = create_RAG(im,n,m);
  perform_merge(rag,threshold);
  create_output_image(rag, argv[2]);

  exit(EXIT_SUCCESS);
}
