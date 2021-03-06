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
 * @file testMerge.c
 */

#include <merge.h>

int main(int argc, char const *argv[]) {
  image im = FAIRE_image();
  image_charger(im,"../../img/norvege.ppm");
  RAG* rag = create_RAG(im,128,128);
  perform_merge(rag,1.01);
  create_output_image(rag, "test.ppm");
  return 0;
}
