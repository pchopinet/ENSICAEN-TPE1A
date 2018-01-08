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
* @file RAG.c
*/
#include <RAG.h>
#include <moments.h>
#include <float.h>

typedef struct moments{
  int M0;
  double M1[3];
  double M2[3];
} Moments;

struct cellule_t{
  int block;
  Cellule* next;
};

struct RAG_t{
  image im;
  int nb_blocks, n, m;
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
  Cellule* temp;
  rag->neighbors = malloc(sizeof(Cellule)*n*m);
  for (i=0;i<n*m;i++){
    rag->neighbors[i].block = i;
    temp = &rag->neighbors[i];
    if ((i+1)%n!=0){
      temp->next = malloc(sizeof(Cellule));
      temp = temp->next;
      temp->block = i+1;
    }
    if (i<n*(m-1)){
      temp->next = malloc(sizeof(Cellule));
      temp = temp->next;
      temp->block = i+n;
    }
    temp->next = NULL;
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
  long double M1, M2;
  int dim = image_give_dim(rag->im);
  rag->erreur_partition = 0;
  for (i=0; i<rag->nb_blocks; i++) {
    M0 = rag->M[i].M0;
    for (k=0; k<dim; k++) {
      M1 = rag->M[i].M1[k];
      M2 = rag->M[i].M2[k];
      rag->erreur_partition += M2-((M1/M0)*(M1/M0));
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
  rag->n = n;
  rag->m = m;
  init_moments_priv(rag, n, m);
  init_father_priv(rag);
  init_neighbors_priv(rag, n, m);
  init_partition_error_priv(rag);
  return rag;
}

/**
* Function giving the both regions to merge and the partition error added by the merge.
*
* @param rag is a RAG structure pointeur.
* @param b1 is a pointeur to the index of the first region to merge.
* @param b2 is a pointeur to the index of the second region to merge.
*
* @return errorMin the error added to the partition error.
*/
extern long double RAG_give_closest_region(RAG * rag, int* b1, int* b2) {
  assert(rag->nb_blocks>1);

  int i, k, n;
  long double error;
  long double errorMin;
  errorMin = DBL_MAX;
  Cellule* cel;
  Cellule* tmp;
  int M0, M0n;
  double *M1, *M1n;
  int dim = image_give_dim(rag->im);

  for (i=0; i<rag->nb_blocks; i++) {
    if (rag->father[i]==i) {
      M0 = rag->M[i].M0;
      M1 = rag->M[i].M1;
      cel = rag->neighbors[i].next;

      while(cel!=NULL){
        error = 0;
        n = cel->block;
        if (rag->father[n]!=n) {
          cel->block = rag->father[n];
          tmp = cel;
          while (tmp->next!=NULL && tmp->block>tmp->next->block) {
            n = tmp->block;
            tmp->block = tmp->next->block;
            tmp->next->block = n;
            tmp = tmp->next;
          }
          if (tmp->next!=NULL && tmp->block==tmp->next->block) {
            tmp->next = tmp->next->next;
          }
        }

        M0n = rag->M[n].M0;
        M1n = rag->M[n].M1;
        for (k=0; k<dim; k++) {
          error += (M1[k]/M0-M1n[k]/M0n)*(M1[k]/M0-M1n[k]/M0n);
        }
        error = error*M0*M0n/(M0+M0n);

        if (error<errorMin) {
          errorMin = error;
          *b1 = i;
          *b2 = n;
        }
        cel = cel->next;
      }
    }
  }

  return errorMin;
}

/**
* Function calculating the moment of the new region.
*
* @param rag is a RAG structure pointeur.
* @param i is the index of the first region to merge.
* @param j is the index of the second region to merge.
*/
static void RAG_merge_moments(RAG * rag, int i, int j){
  int dim = image_give_dim(rag->im);
  int k;
  //update moments
  rag->M[j].M0 += rag->M[i].M0;
  for(k=0;k<dim;k++){
    rag->M[j].M1[k] += rag->M[i].M1[k];
    rag->M[j].M2[k] += rag->M[i].M2[k];
  }
}

/**
* Function associating the new right neighbor to the blocks of the new region.
*
* @param rag is a RAG structure pointeur.
* @param i is the index of the first region to merge.
* @param j is the index of the second region to merge.
*/
static void RAG_merge_neighbors(RAG * rag, int i, int j){
  Cellule * ci = &(rag->neighbors[i]);
  Cellule * cj = &(rag->neighbors[j]);

  Cellule * temp1;
  Cellule * temp2;


  while(ci!=NULL && cj!=NULL){
    if(ci->block<=j){
      ci = ci->next;
    }else if(cj->next !=NULL && ci->block<cj->next->block){
      temp1 = cj->next;
      temp2 = ci->next;
      cj->next = ci;
      ci->next = temp1;
      ci = temp2;
    }else if(cj->next == NULL && cj->block<ci->block){
      cj->next = ci;
    }
    cj = cj->next;
  }
  rag->neighbors[i].next = NULL;
}

/**
* Function merging two regions.
*
* @param rag is a RAG structure pointeur.
* @param i is the index of the first region to merge.
* @param j is the index of the second region to merge.
*/
void RAG_merge_region(RAG * rag, int i, int j){

  assert(i<j);

  int k;
  long double error = 0;
  int dim = image_give_dim(rag->im);
  RAG_merge_moments(rag,i,j);
  RAG_merge_neighbors(rag,i,j);

  int M0 = rag->M[i].M0;
  double* M1 = rag->M[i].M1;
  int M0j = rag->M[j].M0;
  double* M1j = rag->M[j].M1;
  for (k=0; k<dim; k++) {
    error += (M1[k]/M0-M1j[k]/M0j)*(M1[k]/M0-M1j[k]/M0j);
  }
  error = error*M0*M0j/(M0+M0j);
  rag->erreur_partition += error;

  rag->father[i] = j;
}

/**
* Function associating the good father to each block.
*
* @param rag is a RAG structure pointeur.
*/
extern void RAG_normalize_parents(RAG* rag) {
  int k;
  for (k=rag->nb_blocks-1;k>=0;k--) {
    printf("%d\n", rag->father[k]);
    rag->father[k] = rag->father[rag->father[k]];
  }
}

/**
* Function giving the average color of a block.
*
* @param rag is a RAG structure pointeur.
* @param block is the index of the block.
* @param colMoy is a pointer to the average color array.
*/
extern void RAG_give_mean_color(RAG * rag, int block, int* colMoy) {
  int dim = image_give_dim(rag->im);
  int i;
  for (i=0;i<dim;i++) {
    colMoy[i] = rag->M[rag->father[block]].M1[i]/rag->M[rag->father[block]].M0;
  }
}

/**
* Function giving the partition error.
*
* @param rag is a RAG structure pointeur.
* @return rag->erreur_partition is the partition error.
*/
extern long double get_error(RAG * rag){
  return rag->erreur_partition;
}

/**
* Function giving the image.
*
* @param rag is a RAG structure pointeur.
* @return rag->im is the image.
*/
extern image get_image(RAG * rag) {
  return rag->im;
}

/**
* Function giving the number of blocks.
*
* @param rag is a RAG structure pointeur.
* @return rag->nb_blocks is the number of blocks.
*/
extern int get_nb_blocks(RAG * rag) {
  return rag->nb_blocks;
}

/**
* Function giving the number of blocks per row.
*
* @param rag is a RAG structure pointeur.
* @return rag->n is a RAG structure pointeur row.
*/
extern int get_n(RAG * rag) {
  return rag->n;
}

/**
* Function giving the number of blocks per column.
*
* @param rag is a RAG structure pointeur.
* @return rag->m is a RAG structure pointeur column.
*/
extern int get_m(RAG * rag) {
  return rag->m;
}
