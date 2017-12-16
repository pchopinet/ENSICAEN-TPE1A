#ifndef CLASSE_H
#define CLASSE_H

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "exit_if.h"

#define INSTANCIER(c_ident) \
c_ident self = malloc(sizeof(struct c_ident));assert(self);

#define SELF(champ) (self->champ)

#endif /* CLASSE_H */
