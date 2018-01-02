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
 * @file merge.h
 */
#ifndef __MERGE_H
#define __MERGE_H

#include <RAG.h>

void perform_merge(RAG * rag, double error_threshold);

image create_output_image(RAG * rag, char * filename);

#endif
