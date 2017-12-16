#ifndef POINT_H
#define POINT_H

#include "classe.h"
#include "move_type.h"
#include "type_obj.h"

struct point
{
  int coordx;
  int coordy;
};

CLASSE(point);

typedef struct point Point;

#define EGAL_POINT(P1,P2) \
( ( (P1).coordx == (P2).coordx )&&( (P1).coordy == (P2).coordy ) )

#define COORDX(P) ((P).coordx)
#define COORDY(P) ((P).coordy)
#define IS_RIGHTER_POINT(p1,p2)\
((p1).coordx > (p2).coordx)
#define AFFECTE_POINT(p1,p2) ((p1).coordx = (p2).coordx,\
			      (p1).coordy = (p2).coordy)

#define MOVE_RIGHT_POINT(p) ( (p).coordx++ )
#define MOVE_LEFT_POINT(p) ( (p).coordx--)
#define MOVE_UP_POINT(p) ( (p).coordy--)
#define MOVE_DOWN_POINT(p) ( (p).coordy++)

#define NULL_POINT(p) (p == NULL)

#define INFINITE_POINT(p) \
( ( (p).coordx == INFINITE ) && ( (p).coordy == INFINITE ) )

#endif /* POINT_H*/




