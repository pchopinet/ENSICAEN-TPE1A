#ifndef IMAGE_H
#define IMAGE_H

#include "objet.h"
#include "classe.h"

#include "point.h"
#include "move_type.h"

/* Type de fonction a utiliser pour calculer la distance entre 2 pixel */
typedef double (*PFdist)(int*,int*,booleen);
 
CLASSE(image);

extern image     FAIRE_image();
extern void      DEFAIRE_image(image);

/* Return a negative value in case of error */
extern int       image_charger    (image,char*);
extern int       image_sauvegarder(image,char*);
extern int       image_to_stream  (image,FILE*);

extern int*      image_lire_pixel  (image);
extern void      image_ecrire_pixel(image,int*);

extern void image_read_pixel(image,int,int,unsigned  char*);
extern void image_write_pixel(image,int,int, unsigned char*);

extern booleen   image_pixel_suivant(image);
extern void      image_debut        (image);
extern booleen   image_move_to      (image,point);
extern booleen   image_pixel_dessus (image);
extern booleen   image_pixel_dessous(image);
extern booleen   image_pixel_gauche (image);
extern booleen   image_pixel_droite (image);

extern void      image_set_distance(image,PFdist);
/* img,dim,largeur,hauteur */
extern void      image_initialize(image,int,int,int);
extern double    image_distance(image,point,move_type);

extern int       image_give_dim    (image);
extern int       image_give_hauteur(image);
extern int       image_give_largeur(image);
#endif /* IMAGE_H */



