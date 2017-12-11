#include <stdio.h>
#include <time.h>

#include "image.h"
#include "point.h"
#include "type_obj.h"

#define Debut     SELF(debut)
#define Courant   SELF(courant)
#define Largeur   SELF(largeur)
#define Hauteur   SELF(hauteur)
#define Longueur  SELF(longueur)
#define Taille    SELF(taille)
#define Distance  SELF(distance)
#define Dim       SELF(dim)


#define DEPLACEMENT(i,j) ( Dim*( (i)*Largeur+(j) ) )
#define MODE_LECTURE  "r"
#define MODE_ECRITURE "w"


/* types a priori prive a image */
typedef unsigned char Comp_rgb;
typedef int (*PFsauvegarde)(image,FILE*);

/* Declaration des fonctions prives au module image */
static booleen private_valide(image,int,int);
static void    private_recopie(Comp_rgb*,int*);
static double  private_default_distance_dim3(int*,int*,booleen);
static double  private_default_distance_dim1(int*,int*,booleen);
static int     image_dim_1_to_stream_priv(image,FILE*);
static int     image_dim_3_to_stream_priv(image,FILE*);
static void    lire_commentaires_priv(FILE*);
static void    read_ascii_priv(image,FILE*);
static PFdist default_distance[] = 
{
  NULL,
  private_default_distance_dim1,
  NULL,
  private_default_distance_dim3,
};

struct image
{

  Comp_rgb *  debut;
  Comp_rgb *  courant;
  int         hauteur;
  int         largeur;
  int         longueur;/* nb de cellules d'une ligne : Largeur*Dim */
  long        taille; /* nb de cellules de l'image */
  int         dim; /* dimension de l'image 1 ou 3 */
  PFdist      distance; /* Fonction de distance inter pixel*/
};

image FAIRE_image()
{

  INSTANCIER(image);

  return self;
}

void DEFAIRE_image(image self)
{
  free(Debut);
}
  /****************************/
 /*Initialise une image vide */
/****************************/
void image_initialize(image self,int dim,int largeur,int hauteur)
{
  assert(dim >0 && dim <=3);

  Dim = dim;
  Hauteur = hauteur;
  Largeur = largeur;
  Longueur = Dim*Largeur;
  Taille  = Longueur*Hauteur;
  Debut = (Comp_rgb*)calloc(Taille,sizeof(Comp_rgb));
  Courant = Debut;

  if( (Dim <=3) && (Dim >=0) )
    Distance = default_distance[Dim];
}
  /*******************************************/
 /* Fonctions de chargements et Sauvegarde */
/*****************************************/
  /*******************************************/
 /* Fonctions de chargements et Sauvegarde */
/*****************************************/
int image_charger(image self,char* nom)
{
  FILE * id;
  char   MagicNumber[3];
  int    valeur_max;
  size_t taille_comp_RGB = sizeof(Comp_rgb);
  char   retour_chariot;
  
  
  
  id = fopen(nom,MODE_LECTURE);
  
  if(id == NULL)
    return (-1);
  
  
  fscanf(id,"%3s",MagicNumber);
  
  if((MagicNumber[0] != 'P') || (
				 (MagicNumber[1] != '6')&&
				 (MagicNumber[1] != '5')&&
				 (MagicNumber[1] != '2')
				 )
     )
    {
      fprintf(stderr,"le fichier semble ne pas etre un fichier pnm\n");
      fclose(id);
      return(-2);
    }
  if(MagicNumber[1] == '6')
    Dim = 3;  
  else
    Dim =1;
  
  lire_commentaires_priv(id);
  
  
  fscanf(id,"%d",&Largeur);
  fscanf(id,"%d",&Hauteur);
  Longueur = Dim*Largeur;
  Taille  = Longueur*Hauteur;
  
  fscanf(id,"%d%c",&valeur_max,&retour_chariot);
  
  if(Largeur <=0 || Hauteur <= 0 || valeur_max <=1)
    {
      fprintf(stderr," Image non lue :  Contient des valeurs aberrantes\n");
      return(-3);
    }
  Debut = (Comp_rgb *)malloc(taille_comp_RGB*Taille);
  if(MagicNumber[1]=='2')
    read_ascii_priv(self,id);
  else
    fread(Debut,taille_comp_RGB,Taille,id);
  fclose(id);
  
  if( (Dim <=3) && (Dim >=0) )
    Distance = default_distance[Dim];

  return(0);
}
int image_sauvegarder(image self,char* nom)
{
  FILE *id;
  static PFsauvegarde sauvegarde[] =
    {
      NULL,
      image_dim_1_to_stream_priv,
      NULL,
      image_dim_3_to_stream_priv,
    };

  if( (Dim != 1)&&(Dim != 3) )
    {
      fprintf(stderr,"Image Non sauvegardable\n");
      return -1;
    }

  id = fopen(nom,MODE_ECRITURE);
  
  if(id == NULL)
    return -1;

  return sauvegarde[Dim](self,id);
}
int image_to_stream(image self,FILE* id)
{
  static PFsauvegarde sauvegarde[] =
    {
      NULL,
      image_dim_1_to_stream_priv,
      NULL,
      image_dim_3_to_stream_priv,
    };
  
  if( (Dim != 1)&&(Dim != 3) )
    {
      fprintf(stderr,"Image Non sauvegardable\n");
      return -1;
    }

  return sauvegarde[Dim](self,id);
}
int* image_lire_pixel(image self)
{
  static int tab[DIM_MAX];
  int i;
  
  for(i=0;i<Dim;i++)
    tab[i] = (int)(*(Courant+i));

  return tab;
}
int image_get_comp(image self,point p,int comp)
{
  int i = COORDY(*p);
  int j = COORDX(*p);


  assert(private_valide(self,i,j));
  assert(comp>=0&& comp<3);

  return *(Debut+DEPLACEMENT(i,j)+comp);
}
void image_ecrire_pixel(image self,int* tab)
{
  int i;
  
  for(i=0;i<Dim;i++)
    *(Courant+i) = (Comp_rgb)tab[i];
}

booleen image_pixel_suivant(image self)
{
  Comp_rgb *pixel = Courant+Dim;

  /* On est a la fin du tableau */
  if( (pixel - Debut) >= Taille )
    return faux;

  Courant = pixel;
  return vrai;
}
void image_debut(image self)
{
  Courant = Debut;
}
booleen image_move_to(image self,point p)
{
  int i = COORDY(*p);
  int j = COORDX(*p);


  if(!private_valide(self,i,j))
    return faux;
  
  Courant = Debut + DEPLACEMENT(i,j);

  return vrai;
}
void  image_read_pixel(image self,int i,int j,Comp_rgb* buf)
{
  Comp_rgb* ptr;

  assert(self);
  assert(buf);
  assert(private_valide(self,i,j));

  
 ptr = Debut + DEPLACEMENT(i,j);
 for(i=0;i<Dim;i++)
   buf[i]=ptr[i];
}
void  image_write_pixel(image self,int i,int j,Comp_rgb* buf)
{
  Comp_rgb* ptr;

  assert(self);
  assert(buf);  
  assert(private_valide(self,i,j));

  
 ptr = Debut + DEPLACEMENT(i,j);
 for(i=0;i<Dim;i++)
   ptr[i]=buf[i];
}
/* On passe au pixel de la ligne du dessus meme collone */
booleen image_pixel_dessus(image self)
{
  Comp_rgb *pixel = Courant-Longueur;

  if(pixel -Debut < 0)
    return faux;
  
  Courant = pixel;
  return vrai;
}

/* On passe au pixel de la ligne du dessous meme collone */
booleen image_pixel_dessous(image self)
{
  Comp_rgb *pixel = Courant+Longueur;

  if(pixel -Debut >= Taille)
    return faux;
  
  Courant = pixel;
  return vrai;
}

booleen image_pixel_gauche(image self)
{
  int nb_pixel = (int)(Courant - Debut);
  
  if( nb_pixel%Longueur == 0 )
    return faux;

  Courant -= Dim;
  return vrai;
}

booleen  image_pixel_droite(image self)
{
  int nb_pixel = (int) (Courant - Debut);

  if( (nb_pixel+Dim)%Longueur == 0 )
    return faux;

  Courant += Dim;
  return vrai;
}
/* Entree : un point 1/2 et une direction (un move) */
/* Sortie : la distance entre les deux points entiers definis par le */
/* point un demis et la direction */
/* Le point un demis est le point defini par la translation */
/* (.5,.5) a partir du point entier entre */
double image_distance(image self,point p,move_type move)
{
  int pixel_1[3],pixel_2[3];
  Comp_rgb *pix1,*pix2;
  booleen valide1,valide2;
  int x = COORDX(*p);
  int y = COORDY(*p);

/* On recupere les deux pixels definis par le point un demis et la */
/* direction tout en verifiant leur validite */
  switch(move)
    {
    case RIGHT:
      pix1 = Debut + DEPLACEMENT(y,x+1);
      pix2 = pix1 + Longueur;
      valide1 = private_valide(self,y,x+1);
      valide2 = private_valide(self,y+1,x+1);
      break;
   
    case UP:
      pix1 = Debut + DEPLACEMENT(y,x);
      pix2 = pix1+Dim;
      valide1 = private_valide(self,y,x);
      valide2 = private_valide(self,y,x+1);
      break;
      
    case LEFT:
      pix1 = Debut + DEPLACEMENT(y,x);
      pix2 = pix1+ Longueur;
      valide1 = private_valide(self,y,x);
      valide2 = private_valide(self,y+1,x);
      break;
      
    case DOWN:
      pix1 = Debut + DEPLACEMENT(y+1,x);
      pix2 = pix1+Dim;
      valide1 = private_valide(self,y+1,x);
      valide2 = private_valide(self,y+1,x+1);
      break;

    default:
      fprintf(stderr,"distance, image.c : move incorect");
      break;
    }

/* On peut se passer de ces deux recopies mais la fonction distance */
/* est fournies par l'utilisateur et n'a donc pas a acceder */
/* directement au bitmap*/


  if(valide1)
    private_recopie(pix1,pixel_1);

  if(valide2)
    private_recopie(pix2,pixel_2);

  if(!valide1 && !valide2)
    return(-1.0);

  if(valide1&&valide2)
    return Distance(pixel_1,pixel_2,vrai);

  if(valide1)
    return Distance(pixel_1,pixel_2,faux);


  return Distance(pixel_2,pixel_1,faux);


}
void image_set_distance(image self,PFdist fct_dist)
{
  Distance = fct_dist;
}
int image_give_dim(image self)
{
  return Dim;
}
int image_give_largeur(image self)
{
  return Largeur;
}
int image_give_hauteur(image self)
{
  return Hauteur;
}
static booleen private_valide(image self,int i,int j)
{
  booleen valide;
  
  valide = (i>=0)&&(j>=0);
  valide = valide&&(i<Hauteur)&&(j<Largeur);

  return(valide);
}
static void private_recopie(Comp_rgb* pix,int* tab)
{
  int i;

  for(i=0;i<3;i++)
    tab[i] = (int)( pix[i] );
}
/****************************************************************************/
/* Distance par defaut pour des images en niveaux de gris */
/****************************************************************************/
static double private_default_distance_dim1(int* pix1,int*
					    pix2,booleen valide)
{
  double ret;

  ret = (valide?*pix1- *pix2:*pix1);

  if(ret <0) ret = -ret;

  return ret;
}
/****************************************************************************/
/* Distance par defaut pour des images couleur */
/****************************************************************************/
static double private_default_distance_dim3(int* pix1,int*
					    pix2,booleen valide)
{
  double ret=0;
  int i;

  if(valide)
    {
      for(i=0;i<3;i++)
	{
	  ret += ( (*pix1)-(*pix2) )*( (*pix1)-(*pix2) );
	  pix1++;
	  pix2++;
	}
    }
  else
    {
      for(i=0;i<3;i++)
	{
	  ret += (*pix1)*(*pix1);
	  pix1++;
	}
    }
  return ret;
}


  
/*
 * Sauvegarde au format pgm
 */
static int image_dim_1_to_stream_priv(image self,FILE* id)
{
  size_t taille_ind = sizeof(Comp_rgb);
  char date_format[256];
  time_t temps = time(NULL);
  struct tm *temps_local = localtime(&temps);
  int var_max = 255;
  int nb_composantes_ecrites;
  
  strftime(date_format,sizeof date_format, "%x %X", temps_local);
  
  
  fprintf(id,"P5\n");
  fprintf(id,"# Derniere  sauvegarde : %s\n",date_format);
  fprintf(id,"%d %d\n",Largeur,Hauteur);
  fprintf(id,"%d\n",var_max);
  
  nb_composantes_ecrites = fwrite(Debut,taille_ind,Taille,id);
  
  if(nb_composantes_ecrites < Taille)
    fprintf(stderr,"ERREUR SYSTEME : Plus de place sur disque"); 
  
  fclose(id);
  
  return 0;
}
/****************************************************************************/
/* Sauvegarde au format ppm */
/****************************************************************************/
static int image_dim_3_to_stream_priv(image self,FILE* id)
{
  size_t taille_comp_RGB = sizeof(Comp_rgb);
  int var_max =255;
  char date_format[256];
  time_t temps = time(NULL);
  struct tm *temps_local = localtime(&temps);
  int nb_comp_ecrites;
  
  strftime(date_format,sizeof date_format, "%x %X", temps_local);
    
  
  fprintf(id,"P6\n");
  fprintf(id,"# Derniere  sauvegarde : %s\n",date_format);
  fprintf(id,"%d %d\n",Largeur,Hauteur);
  fprintf(id,"%d\n",var_max);
  
  nb_comp_ecrites = fwrite(Debut,taille_comp_RGB,Taille,id);
  
  if(nb_comp_ecrites < Taille)
    fprintf(stderr,"ERREUR SYSTEME : Plus de place sur disque"); 
  
  fclose(id);
  
  return (0);
}
/* Explicite */
static void lire_commentaires_priv(FILE* id)
{
  char c1,c2;
  char   commentaire[100];
  
  fscanf(id,"%c%c",&c1,&c2);
  while(c2 =='#')
    {
      do
	{
	  fscanf(id,"%s",commentaire);
	  fprintf(stderr,"%s ",commentaire);
	  fscanf(id,"%c",&c1);
	}
      while(c1 != '\n');
      fscanf(id,"%c",&c2);
      fprintf(stderr,"\n");
    }
  fseek(id,-1L,SEEK_CUR);
}

static void read_ascii_priv(image self,FILE* id)
{
  Comp_rgb *ptr=Debut;
  int val;

  while(fscanf(id,"%d",&val) != EOF)
    {
      *ptr = val;    
      ptr++;
    }
}



