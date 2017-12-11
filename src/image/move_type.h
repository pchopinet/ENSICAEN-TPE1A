#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H
#include "type_obj.h"

typedef int move_type;

#define NB_MOVE 4

#define RIGHT 0
#define UP    1
#define LEFT  2
#define DOWN  3

#ifdef __USE_MOVE_TYPE__
static int dep_move_x[4] = {+1, 0,-1, 0};
static int dep_move_y[4] = { 0,-1, 0,+1};
                                /* R   UP    L     D */
static move_type _reverse[4]   = {LEFT,DOWN,RIGHT,UP};
static move_type _next_move[4] = {UP  ,LEFT,DOWN ,RIGHT};
static bouleen   _horizontal[4]= {vrai,faux,vrai,faux};
static bouleen   _vertical[4]  = {faux,vrai,faux,vrai};

#define REVERSE_SENSE(m_)   ( _reverse[m_])
#define NEXT_DIRECTION(m_)  ( _next_move[m_])
#define IS_CLOSED(or_)      ( ( (or_)== 4) || ((or_) == -4) )
#define TEST_HORIZONTAL(m_) ( _horizontal[m_] )
#define TEST_VERTICAL(m_)   ( _vertical[m_] )

#endif


extern long move_type_compute_step_clock(move_type,move_type);
extern int  move_type_compute_dist_sigma_1(move_type,move_type);
#endif /* MOVE_TYPE_H*/


