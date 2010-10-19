#ifndef _DEBUG_H_
#define _DEBUG_H_

enum debug_cats_e {
  DEBUG_CAT_MEM_REGION = 1, // 2^0
  DEBUG_CAT_ALLOC      = 2, // 2^1
  DEBUG_CAT_MUTEX      = 4, // 2^2 ...
  DEBUG_CAT_GROUPS     = 8
};

#define MAX_DEBUG_LABEL_LENGTH 20
extern char debug_cat_labels[][MAX_DEBUG_LABEL_LENGTH];

extern unsigned DEBUG_CATS_ENABLED;


#ifdef NO_SEATBELTS
#define assert(X) while(0)
#else
#include <assert.h>
#endif


#ifdef NO_SEATBELTS
#define DEBUG_CAT_ENABLED(X) 0
#else
#define DEBUG_CAT_ENABLED(X) (DEBUG_CATS_ENABLED & (X))
#endif


#ifdef NO_SEATBELTS
static inline void dprint(unsigned category, const char *func, char *format, ...) {
  return;
}

#else
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mpi.h>

#include "armci_internals.h"

static inline void dprint(unsigned category, const char *func, char *format, ...) {
  va_list ap;
  int     rank, disp;
  char    string[500];

  if ((category & DEBUG_CATS_ENABLED) == 0)
    return;

  MPI_Comm_rank(ARMCI_GROUP_WORLD.comm, &rank);

  disp  = 0;
  disp += snprintf(string, 500, "[%4d] %s: ", rank, func);
  va_start(ap, format);
  disp += vsnprintf(string+disp, 500-disp, format, ap);
  va_end(ap);

  fprintf(stderr, "%s", string);

}
#endif /* NO_SEATBELTS */


#endif /* _DEBUG_H_ */