#ifndef EXIT_IF_H
#define EXIT_IF_H
#include <stdio.h>

#define EXIT_IF(expr,mess,type) \
((expr)\
? (fprintf(stderr,">EXIT:%s:%d: %s %s\n",__FILE__,__LINE__,mess,type),exit(1)) :0)

#endif /* EXIT_IF_H */
