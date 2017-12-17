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

#include <RAG.h>

int main(){
  image im=FAIRE_image();
  image_charger(im,"../../img/lenna.ppm");
  create_RAG(im,1,1);

}
