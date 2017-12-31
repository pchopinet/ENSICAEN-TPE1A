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
  double M1, M2;
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
  init_moments_priv(rag, n, m);
  init_father_priv(rag);
  init_neighbors_priv(rag, n, m);
  init_partition_error_priv(rag);
  return rag;
}

extern long double get_error(RAG * rag){
  return rag->erreur_partition;
}

extern double RAG_give_closest_region(RAG * rag, int* b1, int* b2) {
  assert(rag->nb_blocks>1);

  int i, k, n;
  double error;
  double errorMin;
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
        printf("%d \n",n);
        fflush(stdout);
        if (rag->father[n]!=n) {
          cel->block = rag->father[n];
          tmp = cel;
          printf("%d %p\n",tmp->block,tmp->next);
          fflush(stdout);
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

static void RAG_merge_neighbors(RAG * rag, int i, int j){
  //int k;
  Cellule * ci = &(rag->neighbors[i]);
  Cellule * cj = &(rag->neighbors[j]);

  Cellule * temp1;
  Cellule * temp2;


  while(ci!=NULL && cj!=NULL){
    if(ci->block<=j){
      ci = ci->next;
    }else if(ci->block<cj->next->block){
      temp1 = cj->next;
      temp2 = ci->next;
      cj->next = ci;
      ci->next = temp1;//cj->next->next = temp1
      ci = temp2;
    }
    cj = cj->next;
  }
  rag->neighbors[i].next = NULL;

  // Cellule * debut = malloc(sizeof(Cellule));
  // Cellule * fusion = debut;
  // *fusion = rag->neighbors[j];
  //
  // //printf("debut block : %d\n",debut->block);
  // while(ci!=NULL && cj!=NULL) { //fusion des deux listes dans une troisieme
  //   if(ci->block<debut->block) {
  //     ci = ci->next;
  //   } else if(ci->block<cj->block){
  //     fusion->block = ci->block;
  //     fusion->next = malloc(sizeof(Cellule));
  //     fusion = fusion->next;
  //     printf("Ci : %d\n", ci->block);
  //     ci = ci->next;
  //   } else if(ci->block>cj->block){
  //     fusion->block = cj->block;
  //     fusion->next = malloc(sizeof(Cellule));
  //     fusion = fusion->next;
  //     printf("Cj : %d\n", cj->block);
  //     cj = cj->next;
  //   } else {
  //     fusion->block = ci->block;
  //     //if (!(ci->next==NULL || cj->next==NULL)) {
  //       printf("test\n");
  //       fusion->next = malloc(sizeof(Cellule));
  //       fusion = fusion->next;
  //     //}
  //     printf("ij : %d\n", ci->block);
  //     ci = ci->next;
  //     cj = cj->next;
  //   }
  // }
  //
  // while(ci!=NULL){
  //   fusion->block = ci->block;
  //   if(ci->next!=NULL){
  //     fusion->next = malloc(sizeof(Cellule));
  //     fusion = fusion->next;
  //   }
  //   printf("Ci : %d\n", ci->block);
  //   ci = ci->next;
  // }
  //
  // while(cj!=NULL){
  //   fusion->block = cj->block;
  //   if(cj->next!=NULL){
  //     fusion->next = malloc(sizeof(Cellule));
  //     fusion = fusion->next;
  //   }
  //   printf("Cj : %d\n", cj->block);
  //   cj = cj->next;
  // }
  //
  // fusion->next = NULL;
  // printf("\n");
  //
  // rag->neighbors[j] = *debut;
  // rag->neighbors[i].next = NULL;
  //
  // Cellule* cell;
  //
  // /*for (k=0; k<rag->nb_blocks; k++) {
  //   cell = rag->neighbors[k].next;
  //   while (cell!=NULL) {
  //     if (cell->block==i) {
  //       cell->block = j;
  //     }
  //     cell = cell->next;
  //   }
  // }*/
}

void RAG_merge_region(RAG * rag, int i, int j){

  assert(i<j);

  int k;
  double error = 0;
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

extern void RAG_normalize_parents(RAG* rag){
  int k;
  for (k=rag->nb_blocks;k>=0;k--){
    rag->father[k] = rag->father[rag->father[k]];
  }
}

extern RAG_give_mean_color(RAG * rag, int block, int* colMoy){
  
}
