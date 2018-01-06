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
 * @file testRAG.c
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
  int nb_blocks,n ,m;
  long double erreur_partition;
  Moments* M;
  int * father;
  Cellule* neighbors;
};

int main() {
  int i, k;
  Cellule* cel;
  image im=FAIRE_image();
  image_charger(im,"../../img/lenna.ppm");
  RAG* rag = create_RAG(im,4,4);
  printf ("Nombre de blocks : %d\n",rag->nb_blocks);
  printf ("Erreur de Partition : %Lf\n",rag->erreur_partition);
  for (i=0; i<rag->nb_blocks; i++) {
    printf("\n");
    printf("Block : %d\nListe voisins : ",rag->father[i]);
    printf("%d ",rag->neighbors[i].block);
    cel = rag->neighbors[i].next;
    while (cel!=NULL) {
      printf("%d ",cel->block);
      cel = cel->next;
    }
    printf("\n");
    int dim = image_give_dim(im);
    printf("M0 : %d\n",rag->M[i].M0);
    for (k=0; k<dim; k++) {
      printf("%d M1 : %lf - ",k, rag->M[i].M1[k]);
      printf("M2 : %lf\n",rag->M[i].M2[k]);
    }
  }

  double error;
  int b1, b2;
  error = RAG_give_closest_region(rag, &b1, &b2);
  printf("\nAugmentation de l'erreur de partition : %lf\n",error);
  printf("Blocks à fusioner: %d %d\n\n",b1,b2);

  RAG_merge_region(rag,4,5);
  printf("\n");
  error = RAG_give_closest_region(rag, &b1, &b2);
  for (i=0; i<rag->nb_blocks; i++) {
    printf("Block : %d\nListe voisins : ",rag->father[i]);
    printf("%d ",rag->neighbors[i].block);
    cel = rag->neighbors[i].next;
    while (cel!=NULL) {
      printf("%d ",cel->block);
      cel = cel->next;
    }
    printf("\n");
  }
  RAG_merge_region(rag,1,5);

  error = RAG_give_closest_region(rag, &b1, &b2);
  for (i=0; i<rag->nb_blocks; i++) {
    printf("Block : %d\nListe voisins : ",rag->father[i]);
    printf("%d ",rag->neighbors[i].block);
    cel = rag->neighbors[i].next;
    while (cel!=NULL) {
      printf("%d ",cel->block);
      cel = cel->next;
    }
    printf("\n");
  }


  return 0;
}
