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
* @file RAG.c
*/
#include <RAG.h>
#include <moments.h>

typedef struct moments{
  int M0;
  double M1[3];
  double M2[3];
} Moments;

typedef struct cellule_t Cellule;

struct cellule_t{
  int block;
  Cellule* next;
};

struct RAG_t{
  image im;
  int nb_blocks;
  long double erreur_partition;
  Moments* M;
  int * father;
  Cellule* neighbors;
};

/**
 * Function initializing the moments of all the blocks of the image.
 *
 * @param rag is a RAG structure array.
 * @param n is the number of block per line.
 * @param m is the number of block per colone.
 */
static void init_moments_priv(RAG* rag, int n, int m) {
  rag->M = malloc(sizeof(Moments)*n*m);
  int i;
  for (i=0; i<n*m; i++) {
    give_moments(rag->im, i, n, m, &(rag->M[i].M0), rag->M[i].M1, rag->M[i].M2);
  }
}

/**
 * Function initializing the father of each block to it self.
 *
 * @param rag is a RAG structure array.
 */
static void init_father_priv(RAG* rag) {
  rag->father = malloc(sizeof(int)*rag->nb_blocks);
  int i;
  for (i=0; i<rag->nb_blocks; i++) {
    rag->father[i]=i;
  }
}

/**
 * Function initializing the neighbors of each block. *
 * @param rag is a RAG structure array.
 * @param n is the number of block per line.
 * @param m is the number of block per colone.
 */
static void init_neighbors_priv(RAG* rag, int n, int m) {
  int i;
  Cellule temp;
  rag->neighbors = malloc(sizeof(Cellule)*n*m);
  for (i=0;i<n*m;i++){
    rag->neighbors[i].block = i;
    temp = rag->neighbors[i];
    if (i%n!=0){
      temp.next = malloc(sizeof(Cellule));
      temp = *temp.next;
      temp.block = i+1;
    }
    if (i<n*(m-1)){
      temp.next = malloc(sizeof(Cellule));
      temp = *temp.next;
      temp.block = i+n;
    }
    temp.next = NULL;
  }
}

/**
 * Function calculating the partition error of a block.
 *
 * @param rag is a RAG structure array.
 */
static void init_partition_error_priv(RAG * rag) {
  int i, k;
  int M0;
  double M1, M2;
  int dim = image_give_dim(rag->im);
  rag->erreur_partition = 0;
  for (i=0; i<rag->nb_blocks; i++) {
    M0 = rag->M[i].M0;
    for (k=0; k<dim; i++) {
      M1 = rag->M[i].M1[k];
      M2 = rag->M[i].M2[k];
      rag->erreur_partition += M2-(M1*M1*M0*M0);
    }
  }
}

/**
 * Function creating a RAG array contening the data of eahc block.
 *
 * @param im the image to analize.
 * @param n is the number of block per line.
 * @param m is the number of block per colone.
 */
extern RAG* create_RAG(image im, int n, int m) {
  RAG * rag = malloc(sizeof(RAG));
  rag->im = im;
  rag->nb_blocks = n*m;
  init_moments_priv(rag, n, m);
  init_father_priv(rag);
  init_neighbors_priv(rag, n, m);
  init_partition_error_priv(rag);
  return rag;
}
