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
 * @file moments.h
 */

#ifndef __MOMENTS_H
#define __MOMENTS_H

#include <image.h>

extern void give_moments(image im, int nb_block, int n, int m, int * M0, double * M1, double * M2);

#endif
