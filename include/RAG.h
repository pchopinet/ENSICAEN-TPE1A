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
 * @version 0.0.1 / 2017-12-17
 */

/**
 * @file RAG.h
 */

#ifndef __RAG_H
#define __RAG_H

#include <image.h>

typedef struct RAG_t RAG;
typedef struct cellule_t Cellule;

extern RAG* create_RAG(image im, int n, int m);

extern long double RAG_give_closest_region(RAG* rag, int*, int*);

void RAG_merge_region(RAG * rag, int i, int j);

extern long double get_error(RAG * rag);
extern image get_image(RAG * rag);

extern void RAG_normalize_parents(RAG* rag);
extern void RAG_give_mean_color(RAG * rag, int block, int* colMoy);
extern int get_nb_blocks(RAG * rag);
extern int get_n(RAG * rag);
extern int get_m(RAG * rag);

#endif
